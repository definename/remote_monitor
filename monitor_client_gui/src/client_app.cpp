#include "client_app.h"
#include "client_frame.h"

namespace gui
{

//! Macro will allow wxWidgets to create the application object during program execution.
wxIMPLEMENT_APP(client_app);

int client_app::OnExit() {
	return wxApp::OnExit();
}

bool client_app::OnInit(void) {
	if (!wxApp::OnInit()) {
		return false;
	}

	wxImage::AddHandler(new wxPNGHandler);

	frame_ = new client_frame();
	return frame_->Show(true);
}

}
