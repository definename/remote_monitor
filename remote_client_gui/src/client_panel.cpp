#include "client_panel.h"

namespace gui
{

wxBEGIN_EVENT_TABLE(client_panel, wxPanel)
	//EVT_BUTTON(wxID_ANY, BabyFramePanel::OnControl)
wxEND_EVENT_TABLE()

client_panel::client_panel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, parent->GetSize()) {

// 	SetBackgroundColour(BABY_PANEL_COLOUR);
// 
// 	wxImageList* imageList16 = new wxImageList(16, 16, true, 1);
// 	imageList16->Add(wxICON(COLUMN_SELECTED));
// 	list_->SetImageList(imageList16, wxIMAGE_LIST_SMALL);
// 
// 	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
// 	const int border_size = 2;
// 
// 	// List.
// 	topSizer->Add(list_, 1, wxEXPAND | wxALL, border_size);
// 	
// 	// Buttons.
// 	wxBoxSizer* controlSizer = new wxBoxSizer(wxVERTICAL);
// 	controlSizer->Add(new wxButton(this, BabyEvent::ID_ADD, BABY_ACTION_ADD_TEXT), 1, wxSTRETCH_NOT);
// 	controlSizer->Add(new wxButton(this, BabyEvent::ID_EDIT, BABY_ACTION_EDIT_TEXT), 1, wxSTRETCH_NOT);
// 	controlSizer->Add(new wxButton(this, BabyEvent::ID_DELETE, BABY_ACTION_DELETE_TEXT), 1, wxSTRETCH_NOT);
// 
// 	topSizer->Add(controlSizer, 0, wxSTRETCH_NOT | wxUP | wxDOWN | wxRIGHT, border_size);
// 
// 	SetSizer(topSizer);
// 	list_->Update();
}

// void BabyFramePanel::OnControl(wxCommandEvent& e) {
// 	const BabyEvent buttonId = static_cast<BabyEvent>(e.GetId());
// 	if (buttonId == BabyEvent::ID_ADD) {
// 		BabyAddUpdateDialog* addDialog = new BabyAddUpdateDialog(this, wxT("Add"));
// 		if (addDialog->ShowModal() == wxID_OK) {
// 			list_->AddItem(addDialog->GetBabyData());
// 		}
// 		addDialog->Destroy();
// 	} else if (buttonId == BabyEvent::ID_EDIT) {
// 		long index;
// 		if (list_->GetSelectedIndex(index)) {
// 			BabyDataPtr dataOld;
// 			if (list_->GetItem(index, dataOld)) {
// 				BabyAddUpdateDialog* updateDialog = new BabyAddUpdateDialog(this, wxT("Update"), dataOld);
// 				if (updateDialog->ShowModal() == wxID_OK) {
// 					list_->UpdateItem(index, updateDialog->GetBabyData());
// 				}
// 				updateDialog->Destroy();
// 			} else {
// 				wxString msg;
// 				msg.Printf("Failed to retrieve item with given index:%d", index);
// 				wxMessageBox(msg.ToStdWstring(), wxT("Update item"), wxICON_WARNING, this);
// 			}
// 		} else {
// 			wxMessageBox(wxT("Please select item to update"), wxT("Update item"), wxICON_INFORMATION, this);
// 		}
// 	} else if (buttonId == BabyEvent::ID_DELETE) {
// 		long index;
// 		if (list_->GetSelectedIndex(index)) {
// 			list_->RemoveItem(index);
// 		} else {
// 			wxMessageBox(wxT("Please select item to remove"), wxT("Remove item"), wxICON_INFORMATION, this);
// 		}
// 	}
// }

}
