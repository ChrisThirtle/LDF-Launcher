#ifndef LAUNCHER_H
#define LAUNCHER_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//Include additional wxWidgets libraries
#include "wx/vector.h"
#include "wx/xrc/xmlres.h"
#include "wx/colour.h"

//Include selfmade libraries
#include "xmlparser.h"
#include "drawpanel.h"

//Include standard libraries
#include <fstream>

//Paths to important files
#define LEGENDARY_LOGO_PATH "./LDF.png"


class Launcher : public wxApp {
private:
	wxList* getControls(wxWindow* win);
	void getControls(wxList* list, wxWindow* win);
	wxList* getControls(wxSizer* win);
	void getControls(wxList* list, wxSizer* win);
	
	
public:
	
	virtual bool OnInit();
};

//Main window frame, contains all other frame
class LauncherFrame : public wxFrame {
private:
	wxPanel *menu;
	wxPanel *options;
	
	void MenuButtonHandler(wxEvent& e);
	void LaunchDF(wxEvent& e);
	
public:

	LauncherFrame(const wxString& title);
	
	
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
};




enum
{
	Minimal_Quit = wxID_EXIT,
	Minimal_About = wxID_ABOUT
};

#endif