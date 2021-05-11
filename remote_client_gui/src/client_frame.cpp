#include "client_frame.h"
#include "client_panel.h"

// #include "BabyFramePanel.h"
// #include "BabyTypes.h"

namespace gui
{

client_frame::client_frame()
	: wxFrame(NULL, wxID_ANY, wxT("Monitor GUI"), wxDefaultPosition, wxSize(550, 300), wxDEFAULT_FRAME_STYLE)
	, panel_(new client_panel(this))
{
	Bind(wxEVT_CLOSE_WINDOW, &client_frame::OnClose, this);

// 	SetIcon(wxICON(FRAME_ICON));
// 	SetMinSize(GetSize());
// 
// 	// Menu file
// 	wxMenu* menuFile = new wxMenu;
// 	wxMenu* menuBaby = new wxMenu;
// 	menuFile->AppendSubMenu(menuBaby, wxT("Baby"));
// 
// 	// Sub menu baby
// 	wxMenuItem* addItem = new wxMenuItem(menuBaby, BabyEvent::ID_ADD, BABY_ACTION_ADD_TEXT);
// 	addItem->SetBitmap(wxArtProvider::GetBitmap(wxART_NEW));
// 	menuBaby->Append(addItem);
// 	wxMenuItem* editItem = new wxMenuItem(menuBaby, BabyEvent::ID_EDIT, BABY_ACTION_EDIT_TEXT);
// 	editItem->SetBitmap(wxArtProvider::GetBitmap(wxART_EDIT));
// 	menuBaby->Append(editItem);
// 	wxMenuItem* deleteItem = new wxMenuItem(menuBaby, BabyEvent::ID_DELETE, BABY_ACTION_DELETE_TEXT);
// 	deleteItem->SetBitmap(wxArtProvider::GetBitmap(wxART_DELETE));
// 	menuBaby->Append(deleteItem);
// 
// 	menuFile->AppendSeparator();
// 
// 	wxMenuItem* exitItem = new wxMenuItem(menuFile, BabyEvent::ID_EXIT, wxT("&Exit"));
// 	exitItem->SetBitmap(wxArtProvider::GetBitmap(wxART_QUIT));
// 	menuFile->Append(exitItem);
// 
// 	// Menu help
// 	wxMenu* menuHelp = new wxMenu;
// 	wxMenuItem* aboutItem = new wxMenuItem(menuHelp, BabyEvent::ID_ABOUT, wxT("&About\tF1"));
// 	aboutItem->SetBitmap(wxArtProvider::GetBitmap(wxART_HELP_BOOK));
// 	menuHelp->Append(aboutItem);
// 
// 	// Complete menu
// 	wxMenuBar* menubar = new wxMenuBar;
// 	menubar->Append(menuFile, wxT("&File"));
// 	menubar->Append(menuHelp, wxT("&Help"));
// 	SetMenuBar(menubar);
// 
// 	Bind(wxEVT_MENU, &BabyFramePanel::OnControl, panel_, BabyEvent::ID_ADD, BabyEvent::ID_DELETE);
// 	Bind(wxEVT_MENU, &BabyFrame::OnMenuEvent, this, BabyEvent::ID_EXIT, BabyEvent::ID_ABOUT);
}

void client_frame::OnMenuEvent(wxCommandEvent& e)
{
// 	const BabyEvent menuId = static_cast<BabyEvent>(e.GetId());
// 	if (menuId == BabyEvent::ID_ABOUT) {
// 		wxMessageBox(BABY_MGR_ABOUT, wxT("About"), wxOK | wxCENTRE, this);
// 	} else if (menuId == BabyEvent::ID_EXIT) {
// 		Close();
// 	}
}

void client_frame::OnClose(wxCloseEvent& WXUNUSED(e)) {
	Destroy();
}

}
