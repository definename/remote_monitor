#include "client_panel.h"

namespace gui
{

wxBEGIN_EVENT_TABLE(client_panel, wxPanel)
	EVT_PAINT(client_panel::on_paint)
wxEND_EVENT_TABLE()

client_panel::client_panel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, parent->GetSize()) {
	// Fill bitmap with given colour and initialize image
	wxBitmap bitmap(GetSize());
	wxMemoryDC memdc;
	memdc.SelectObject(bitmap);
	memdc.SetBackground(*wxLIGHT_GREY);
	memdc.Clear();
	memdc.SelectObject(wxNullBitmap);
	image_ = bitmap.ConvertToImage();
}

void client_panel::render(wxDC& dc) {
	wxBitmap dib;
	wxSize dcSize = dc.GetSize();
	wxSize imgSize = image_.GetSize();

	dib = wxBitmap(image_.Scale(dcSize.GetWidth(), dcSize.GetHeight()));
	dc.DrawBitmap(dib, 0, 0, false);
}

void client_panel::on_paint(wxPaintEvent& WXUNUSED(e)) {
	wxPaintDC dc(this);
	render(dc);
}

void client_panel::update_screen(const wxImage& img, const wxRect& r, bool is_fullscreen) {
	if (is_fullscreen) {
		image_.Resize(r.GetSize(), r.GetPosition());
	}
// 	static int count = 0;
// 	img.SaveFile(Poco::format("img\\sub_image%d.png", count), wxBITMAP_TYPE_PNG);
	image_.Paste(img, r.GetX(), r.GetY());
	wxClientDC dc(this);
	render(dc);
//	count++;
}

}
