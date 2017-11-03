#ifndef LAUNCHER_H
#define LAUNCHER_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


class Launcher : public wxApp {
public:
	virtual bool OnInit();
};

//Main window frame, contains all other frame
class LauncherFrame : public wxFrame {
public:

	LauncherFrame(const wxString& title);
	
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

private:
	wxDECLARE_EVENT_TABLE();
};





enum
{
	Minimal_Quit = wxID_EXIT,
	Minimal_About = wxID_ABOUT
};

#endif