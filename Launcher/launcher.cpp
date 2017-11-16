// ============================================================================
// declarations
// ============================================================================

#include "launcher.h"

#define CONTROLS_LIST {"wxRadioButton","wxButton","wxCheckBox"}
#define XML_SOURCE "Test.xrc"
// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. Launcher and
// not wxApp)
IMPLEMENT_APP(Launcher)


bool Launcher::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;
		
	
	wxXmlResource::Get()->InitAllHandlers();
	wxXmlResource::Get()->Load(XML_SOURCE);
	
    LauncherFrame *frame = new LauncherFrame("Legendary Dwarf Fortress");
    frame->Show(true);
	
    return true;
}



LauncherFrame::LauncherFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
	wxSize framesz;
	menu = new wxPanel(this);
	options = new wxPanel(this);
	
	wxBoxSizer *layout = new wxBoxSizer(wxHORIZONTAL);
	
	layout->Add(menu,0,wxEXPAND);
	layout->AddSpacer(10);
	layout->Add(options,1,wxEXPAND);
	
	this->SetSizer(layout);
	
	wxVector <wxPanel*> pages;
	
	/*
	Left side menu
	*/
	wxBoxSizer *menulayout = new wxBoxSizer(wxVERTICAL);
	menu->SetSizer(menulayout);
	
	
	//Launch DF button
	const unsigned char iconArr[] = {0x81, 0x7E, 0x7E, 0x5A, 0x7E, 0x00, 0x24, 0x00, 0x81, 0xC7, 0xCF};
	
	const wxBitmap *dfIcon = new wxBitmap((char*)iconArr,8,11);
	wxImage dfImage = dfIcon->ConvertToImage();
	delete dfIcon;
	dfImage = dfImage.Size(wxSize(dfImage.GetWidth()+2,dfImage.GetHeight()+2),wxPoint(1,1),0,0,0);
	dfImage.Rescale(dfImage.GetWidth()*5,dfImage.GetHeight()*5);
	dfIcon = new wxBitmap(dfImage);
	
	wxBitmapButton *dfButton = new wxBitmapButton(menu,wxID_ANY,*dfIcon);
	dfButton->Bind(wxEVT_LEFT_DOWN,&LauncherFrame::LaunchDF,this);
	menulayout->Add(dfButton, wxALIGN_CENTER_HORIZONTAL);
	
	
	//Mod option pages controls
	const wxXmlNode *menuoptions = wxXmlResource::Get()->GetResourceNode("Start");
	for(menuoptions = menuoptions->GetNext();
		menuoptions != NULL;
		menuoptions = menuoptions->GetNext() )
	{
			wxButton *b = new wxButton(menu,wxID_ANY,menuoptions->GetAttribute("name","NULL"));
			menulayout->Add(b, wxALIGN_CENTER_HORIZONTAL);
			b->Show(true);
			b->Bind(wxEVT_LEFT_DOWN,&LauncherFrame::MenuButtonHandler,this);
	}
	
	
	
	
	//LDF Logo
	wxImage::AddHandler(new wxPNGHandler);
	wxBitmap ldfLogo;
	ldfLogo.LoadFile(LEGENDARY_LOGO_PATH, wxBITMAP_TYPE_PNG);
	DrawPanel *logoSpace = new DrawPanel(this, ldfLogo);
	
	menulayout->Add(logoSpace, wxALIGN_CENTER_HORIZONTAL);
	logoSpace->Show(true);
	
	
	
	menulayout->Fit(menu);
	framesz.IncTo(menulayout->ComputeFittingWindowSize(this));
	
	/*
	Right side, mod options
	*/
	menuoptions = wxXmlResource::Get()->GetResourceNode("Start");
	for(menuoptions = menuoptions->GetNext();
		menuoptions != NULL;
		menuoptions = menuoptions->GetNext() )
	{
		wxPanel *page = wxXmlResource::Get()->LoadPanel(options, menuoptions->GetAttribute("name","NULL"));
		page->Show(true);
		framesz.IncTo(page->GetSizer()->ComputeFittingWindowSize(this));
		framesz.IncTo(layout->ComputeFittingWindowSize(this));
		page->Show(false);
		pages.push_back(page);
	}
	
	/*
	Processing for all controls
	Get validators set up for everyone, basically
	*/
	wxList* modControls = Launcher::getControls(options);
	validatorSwitches = new MutexVector<HalfSwitch*>();
	
	validatorSwitches->AcquireLock();
	validatorSwitches->reserve(modControls->GetCount());
	
	std::vector<HalfSwitch*>::iterator vit = validatorSwitches->begin();
	for(wxList::iterator mit = modControls->begin();
		mit != modControls->end();
		mit++, vit++){
		if(wxCheckBox* control = wxDynamicCast((*mit),wxCheckBox)){
			(*vit) = new HalfSwitch(control->GetName().ToStdString(),control->GetValue());
			control->SetValidator(wxGenericValidator(&((*vit)->bval)));
		}
		else if(wxRadioButton* control = wxDynamicCast((*mit),wxRadioButton)){
			(*vit) = new HalfSwitch(control->GetName().ToStdString(),control->GetValue());
			control->SetValidator(wxGenericValidator(&((*vit)->bval)));
		}
		/*if(*vit) {
			//wxLogMessage(wxString((*vit)->name));
			wxLogMessage(wxString(std::to_string((*vit)->bval)));
		}*/
	}
	validatorSwitches->ReleaseLock();
	
	this->SetMinSize(framesz);
	this->SetMaxSize(framesz);
	this->SetSize(framesz);

	menu->Show(true);
	options->Show(true);
	
	pages.at(0)->Show(true);
	pages.at(0)->SetSize(options->GetClientSize());
	pages.at(0)->GetSizer()->RecalcSizes();
	

} //End LauncherFrame constructor

void LauncherFrame::MenuButtonHandler(wxEvent& e){
	wxButton *b = dynamic_cast<wxButton*>(e.GetEventObject());
	wxWindow *selectedpage = this->options->FindWindow(b->GetLabel());
	wxWindowList pages = this->options->GetChildren();
	if(selectedpage != NULL) {
		for(wxWindowList::iterator i = pages.begin();
			i != pages.end();
			i++)
			(*i)->Show(false);
		selectedpage->Show(true);
		selectedpage->SetSize(options->GetClientSize());
		selectedpage->GetSizer()->RecalcSizes();
	}
	this->Layout();
	
	validatorSwitches->AcquireLock();
	this->TransferDataFromWindow();
	validatorSwitches->ReleaseLock();
	
	e.Skip();
}

void LauncherFrame::LaunchDF(wxEvent& e){
	e.Skip();
}



wxList* Launcher::getControls(wxWindow* win) {
	wxList* list = new wxList();
	getControls(list,win);
	return list;
}
wxList* Launcher::getControls(wxSizer* win) {
	wxList* list = new wxList();
	getControls(list,win);
	return list;
}
void Launcher::getControls(wxList* list, wxSizer* win){
    if(win == NULL or list == NULL)
		return;
		
	wxSizerItemList kids = win->GetChildren();
	for(wxSizerItemList::iterator i = kids.begin();
		 i != kids.end();
		 i++) {
		if((*i)->IsWindow()) getControls(list,(*i)->GetWindow());
		else if((*i)->IsSizer()) getControls(list,(*i)->GetSizer());
		}
    return;
}
void Launcher::getControls(wxList* list, wxWindow* win){
	if(win == NULL or list == NULL) return;
    if(win->IsKindOf(wxCLASSINFO(wxControl))) {
        list->Append(win);
    }
    else {
		wxWindowList kids = win->GetChildren();
		for(wxWindowList::iterator i = kids.begin();
             i != kids.end();
             i++) 
            getControls(list,*i);
    }
    return;
}

