#include "drawpanel.h"

DrawPanel::DrawPanel(wxWindow* parent, wxBitmap img) : wxPanel(parent){
	this->AssignImage(img);
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(DrawPanel::Draw));
}

void DrawPanel::AssignImage(wxBitmap img){
	image = img;
	this->SetSize(image.GetSize());
	this->SetMinSize(image.GetSize());
}

void DrawPanel::Draw(wxPaintEvent& e) {
	wxPaintDC painter(this);
	if(image.IsOk());
		painter.DrawBitmap(image,0,0,true);
}