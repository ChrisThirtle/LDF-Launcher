// ============================================================================
// declarations
// ============================================================================

#include "launcher.h"
#include "wx/xrc/xmlres.h"

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(LauncherFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  LauncherFrame::OnQuit)
    EVT_MENU(Minimal_About, LauncherFrame::OnAbout)
wxEND_EVENT_TABLE()

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
	wxXmlResource::Get()->Load("Test.xrc");
	
    LauncherFrame *frame = new LauncherFrame("Minimal wxWidgets App");
    frame->Show(true);
	
    return true;
}



LauncherFrame::LauncherFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
	wxGridSizer *page;
	wxXmlResource::Get()->LoadObject(page, this,"Init","wxGridSizer");
	//SetSizerAndFit(page);
/*
	#if wxUSE_MENUS
		// create a menu bar
		wxMenu *fileMenu = new wxMenu;

		// the "About" item should be in the help menu
		wxMenu *helpMenu = new wxMenu;
		helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

		fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

		// now append the freshly created menu to the menu bar...
		wxMenuBar *menuBar = new wxMenuBar();
		menuBar->Append(fileMenu, "&File");
		menuBar->Append(helpMenu, "&Help");

		// ... and attach this menu bar to the frame
		SetMenuBar(menuBar);
	#endif // wxUSE_MENUS

	#if wxUSE_STATUSBAR
		// create a status bar just for fun (by default with 1 pane only)
		CreateStatusBar(1);
		SetStatusText("Welcome to wxWidgets!");
	#endif // wxUSE_STATUSBAR
*/
}

// event handlers

void LauncherFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void LauncherFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
                 (
                    "Welcome to %s!\n"
                    "\n"
                    "This is the minimal wxWidgets sample\n"
                    "running under %s.",
                    wxVERSION_STRING,
                    wxGetOsDescription()
                 ),
                 "About wxWidgets minimal sample",
                 wxOK | wxICON_INFORMATION,
                 this);
}