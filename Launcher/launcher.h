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
#include "wx/valgen.h"

//Include additional libraries
#include "cereal/archives/portable_binary.hpp"

//Include selfmade libraries
#include "xmlparser.h"
#include "drawpanel.h"
#include "threading.h"
#include "switch.h"

//Include standard libraries
#include <fstream>

//Paths to important files
#define LEGENDARY_LOGO_PATH "./LDF.png"


class Launcher : public wxApp {
private:
	
	static void getControls(wxList* list, wxWindow* win);
	
	static void getControls(wxList* list, wxSizer* win);
	
	
public:
	static wxList* getControls(wxWindow* win);
	static wxList* getControls(wxSizer* win);
	virtual bool OnInit();
};

//Main window frame, contains all other frame
class LauncherFrame : public wxFrame {
private:
	wxPanel *menu;
	wxPanel *options;
	
	MutexVector<HalfSwitch*>* validatorSwitches;
	
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