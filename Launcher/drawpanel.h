#ifndef DRAWPANEL
#define DRAWPANEL

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

class DrawPanel : public wxPanel{
private:
	wxBitmap image;
public:
DrawPanel(wxWindow* parent, wxBitmap img);
void AssignImage(wxBitmap img);
void Draw(wxPaintEvent& e);

};

#endif