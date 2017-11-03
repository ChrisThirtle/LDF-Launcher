#include "fileeditor.h"

#define RAWS_PATH
#define ADDITIONAL_FILES 

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
	
}

FileEditor::EditFile(std::string fname, EditPacket edits){
	std::string filetext;
	std::fstream file(fname, file.in);
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
void EditPacket::push(std::string input) {
	q.push(input)
}
EditPacket::Switch EditPacket::pop() {
	EditPacket::Switch sw = q.pop();
	return sw.getRegex();
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