class InitPage : public wxFrame {
	InitPage(wxWindow* parent, const wxString& title);
	
	
}



/*

----------------------------------------------------------
|                                                        |
|                                                        |
|                                                        |
|                                                        |
|                                                        |
|                                                        |
|                                                        |
|                                                        |
|                                                        |
----------------------------------------------------------

Contents:
Launch options
Save settings
Graphics & TWBT controls (leave space for later)
Embark settings
*/

InitPage::InitPage(wxWindow* parent,const wxString& title,) : wxFrame(parent, wxID_ANY, title) {



	wxGridSizer *page = new wxGridSizer(0,2,20,20);
	for(int i=0;i<10;i++){
		page->Add(
				new wxCheckBox(this,wxID_ANY,"Hi there"),
				0,
				wxEXPAND
				);
	}
	page->Show(true);
	SetSizerAndFit(page);
}





PageLayoutClass {
	

}