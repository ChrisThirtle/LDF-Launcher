#include "fileeditor.h"

#define LDFTEST 1
#define RAWS_PATH ""
#define INIT_PATH ""

#define XML_SOURCE "Filetest.xrc"

class tester : public wxApp {
	private:
	std::future<int> endManager;
	public:
	virtual bool OnInit();
};

IMPLEMENT_APP(tester);

#if LDFTEST==1


bool tester::OnInit(){
    if ( !wxApp::OnInit() )
        return false;
		
	wxFrame *frm = new wxFrame(NULL, wxID_ANY, "TEST");
	frm->Show(true);
	
	wxXmlResource::Get()->InitAllHandlers();
	wxXmlResource::Get()->Load(XML_SOURCE);
	
	wxLogMessage("Loaded at all");
	
	MutexVector<HalfSwitch> source;
	source.AcquireLock();
	source.emplace_back("red",true);
	source.emplace_back("green",false);
	source.emplace_back("blue",true);
	source.ReleaseLock();
	
	wxLogMessage("Filled the source vector");
	
	std::promise<bool> prom;
	std::future<bool> fut(prom.get_future());
	
	wxLogMessage("Created future and promise");

	endManager  = std::async(std::launch::async,
									&EditorManager::AsyncManager,
									std::move(fut), &source);
									
	std::this_thread::sleep_for(std::chrono::seconds(4));
	wxLogMessage("Spawned async thread");
	
	
	source.AcquireLock();
	source[0].bval = false;
	source[1].bval = true;
	source.ReleaseLock();
	
	wxLogMessage("Messed with the values");
	
	
	return true;
}
#endif

std::vector<std::string> EditorManager::GetFiles() {
	//Collect files for editor to deal with
	Directory::Iterator dir;
	
	std::vector<std::string> files;
	dir.changeDirectory(RAWS_PATH);
	try {
		while(true){
			std::string filepath = dir.getDirPath() + dir.getNextFilename();
			if(filepath.compare(filepath.length()-4,std::string::npos,".txt") == 0) //If filepath leads to a .txt file
				files.push_back(filepath);									 //Add to file list
		}
	}
	catch (std::out_of_range e){};
	dir.home();
	dir.changeDirectory(INIT_PATH);
	try {
		while(true){
			std::string filepath = dir.getDirPath() + dir.getNextFilename();
			if(filepath.compare(filepath.length()-4,std::string::npos,".txt") == 0) //If filepath leads to a .txt file
				files.push_back(filepath);									 //Add to file list
		}
	}
	catch (std::out_of_range e){};
	
	return files;
}

std::vector<FullSwitch> EditorManager::GetFullSwitches(MutexVector<HalfSwitch> *halfsw) {
	//Collect the full switches for the controls;
	std::vector<FullSwitch> fullsw;
	halfsw->AcquireLock();
	for(std::vector<HalfSwitch>::iterator it = halfsw->begin();
		it != halfsw->end();
		it++){
		std::string regexstring = "";
		std::string replacestring = "";
		std::string targetfile = "";
		
		wxXmlNode* metadata = XmlParser::getMeta((*it).name)->GetChildren();
		while(metadata != NULL) {
			if(metadata->GetName().Lower() == "regex")
				regexstring = metadata->GetNodeContent();
			else if(metadata->GetName().Lower() == "repstr")
				replacestring = metadata->GetNodeContent();
			else if(metadata->GetName().Lower() == "file_prefix")
				targetfile = metadata->GetNodeContent();
			metadata = metadata->GetNext();
		}
		fullsw.emplace_back((*it),targetfile,regexstring,replacestring);
	}
	halfsw->ReleaseLock();
	return fullsw;
}

#if LDFTHREADED==0 //Single threaded file editing, first implementation
int EditorManager::AsyncManager(std::future<bool> endManager, MutexVector<HalfSwitch>* editsource){
	EditorManager ed = EditorManager(std::move(endManager),editsource);
	return 1;
}

EditorManager::EditorManager(std::future<bool> endManager, MutexVector<HalfSwitch> *editsource) {
	
	std::vector<std::string> files = EditorManager::GetFiles();
	std::vector<FullSwitch> editdest = EditorManager::GetFullSwitches(editsource);

	//Begin editing files in loop
	std::vector<FullSwitch> editdiffs;
	while(endManager.wait_for(std::chrono::seconds::zero()) != std::future_status::ready){ //While you've not been told to exit, edit files
		
		editsource->AcquireLock();							//Lock switch list
		for(int it = 0; it < editdest.size(); it++){		//For each switch
			if(not (*editsource)[it].Equals(editdest[it]))		//Compare current switches to previous switches
				editdiffs.emplace_back(editdest[it]);		//Put the differences in a list
				editdest[it].ival = (*editsource)[it].ival;	//Merge differences to the old list
		}
		editsource->ReleaseLock();							//Release switch list lock
		
		EditorManager::ModifyFiles(files,editdiffs);
	}
}



void EditorManager::ModifyFiles(std::vector<std::string>& files, std::vector<FullSwitch>& diffs){
	for(std::vector<std::string>::iterator it = files.begin();	//For each file to be edited
		it != files.end();
		it++){
		std::fstream currfile;
		std::string filetext;
		
		//Read file contents
		currfile.open(*it,std::ios_base::in);
		std::copy(std::istream_iterator<char>(currfile),
				  std::istream_iterator<char>(),
				  std::back_inserter(filetext));
		currfile.close();
		
		wxString fstring(filetext);
		
		//Modify file contents
		for(std::vector<FullSwitch>::iterator control = diffs.begin();
			control != diffs.end();
			control++){
			wxString replacestring;
			int pos;
			while(std::string::npos != (pos = (*control).repstr.find("&value"))) { //Replace all "&value"s in repstr
				if((*control).type == HalfSwitch::BOOLEAN){ //For boolean controls
					if((*control).bval == true)
						replacestring = wxString( (*control).repstr.replace(pos, 6, "[") ); //If true, activate controls
					else
						replacestring = wxString( (*control).repstr.replace(pos, 6, "!") ); //If false, deactivate controls
				}
				else { //For integer controls
					replacestring = wxString( (*control).repstr.replace(pos, 6, std::string::to_string((*control).ival) ); //If integer control, replace with ival.
				}
			}
			while(std::string::npos != (pos = (*control).repstr.find("&nvalue"))) { //Replace all "&nvalue"s in repstr
				if((*control).type == HalfSwitch::BOOLEAN){ //For boolean controls
					if((*control).bval == true)
						replacestring = wxString( (*control).repstr.replace(pos, 7, "!") ); //If true, deactivate controls
					else
						replacestring = wxString( (*control).repstr.replace(pos, 7, "[") ); //If false, activate controls
				}
				else { //For integer controls, &nvalue == &value meaning-wise
					replacestring = wxString( (*control).repstr.replace(pos, 7, std::string::to_string((*control).ival) );
				}
			}
			
			(*control).GetRegex()->ReplaceAll(&fstring,replacestring);
			
		} //End for
		
		filetext = fstring.ToStdString();
		//Rewrite file
		currfile.open(*it,std::ios_base::out | std::ios_base::trunc);
		currfile << filetext;
		currfile.close();
	} //End for
}

#elif LDFTHREADED==1 //Simple multithreading, not too smart
#elif LDFTHREADED==2 //Smart multithreading
/*
EditorManager::EditorManager(std::future f) {
	futur = f;
	//Set up MutexQueues and FileEditors for each file
	ManageEdits();
}

EditorManager::addEditorFromDir() {
	queueVector.emplace_back();
	editorVector.push_back(std::async(std::launch::async,EditPacket::EditPacket,queueVector.back());
}


void EditorManager::ManageEdits(){
	//Check if 
}

int FileEditor::MainLoop(std::string fname, std::future f, MutexQueue* queue) {
	while(!(f.wait_for(0) == future_status::ready)){
		if(!queue.isEmpty()) {
			FileEditor::EditFile(fname,queue.pop());
		}
	}
	return 0
}


FileEditor::EditFile(std::string fname, EditPacket edits){
	std::string filetext;
	std::fstream file;
	int delay = 1;
	
	file.open(fname, file.in);
	
	while(file.rdstate() == std::ios_base::failbit){ //If you've maxed out your allotted open files
		std::this_thread::sleep_for(std::chrono::seconds(delay));
		if(delay<5) delay*=2;
		file.open(fname, file.in); //Wait for a free one
	}

	std::copy(std::istream_iterator<char>(file), //Read from beginning of file
		      std::istream_iterator<char>(),     //Stop reading when you equal the EOS iterator
	     	  std::back_inserter(this->filetext)); //Insert to the end of the text file
	filetext.shrink_to_fit(); //Minimize the size of the string; it has the whole file in it so it's somewhat unwieldy now
	while(!edits.isEmpty()){
		EditPacket::Switch sw = edits.pop()									//Get the data on what switch to do
		std::regex_replace(filetext, sw.getRegex(), sw.getReplacement());	//Do the switch
	}																		//Repeat until edit queue is empty
	file.close();															//Close file
	file.open(fname, file.out | file.trunc);								//Truncate (delete all content) of file
	file.write(filetext.c_str(),filetext.size());							//Write edited contents to the file.
}







//EditPacket
void EditPacket::push(EditPacket::Switch input) {
	q.push(input)
}
EditPacket::Switch EditPacket::pop() {
	return q.pop();
}
bool EditPacket::isEmpty() {
	return q.empty();
}

//EditPacket::Switch
EditPacket::Switch(std::regex regin, std::string repin, std::string prefin){
		regstr = regin;
		repstr = repin;
		filePrefix = prefin;
}
std::regex EditPacket::getRegex() {return regstr;}
std::string EditPacket::getReplacement() {return repstr;}
std::string EditPacket::getPrefix() {return filePrefix;}


//MutexQueue
//Only performs operations when the queue is not locked by another process, makes it thread-safe.
void MutexQueue::push(std::string input) {
	std::lock_guard<std::mutex> lock(q_mutex)
	q.push(input)
}
EditPacket MutexQueue::pop() {
	std::lock_guard<std::mutex> lock(q_mutex)
	return q.pop(input);
}
bool MutexQueue::isEmpty() {
	std::lock_guard<std::mutex> lock(q_mutex)
	return q.empty();
}
*/
#endif