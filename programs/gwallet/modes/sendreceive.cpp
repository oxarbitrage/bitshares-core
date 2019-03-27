#include "../include/modes/sendreceive.hpp"

#include <wx/srchctrl.h>
#include <wx/combo.h>

SendReceive::SendReceive(GWallet* gwallet) : wxFrame()
{
   p_GWallet = gwallet;
}

void SendReceive::CreateControls()
{
   wxBitmap qr_image(p_GWallet->directory + wxT("/icons/qr.png"), wxBITMAP_TYPE_PNG);

   p_GWallet->sizerTransferMode = new wxBoxSizer(wxVERTICAL);
   p_GWallet->mainSizer->Add(p_GWallet->sizerTransferMode, 0, wxGROW|wxALL);

   wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
   p_GWallet->sizerTransferMode->Add(itemBoxSizer1, 0, wxGROW|wxALL);

   wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer1->Add(itemBoxSizer2, 0, wxGROW|wxALL, 5);

   wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(p_GWallet->panel, wxID_ANY, _("Send"));
   wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
   itemBoxSizer2->Add(itemStaticBoxSizer3, 6, wxALIGN_TOP|wxALL, 5);

   wxStaticText* itemStaticText4 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           _("Receipient:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer3->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALL, 5);

   wxSearchCtrl* itemSearchCtrl5 = new wxSearchCtrl( itemStaticBoxSizer3->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer3->Add(itemSearchCtrl5, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText6 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           _("Amount:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer3->Add(itemStaticText6, 0, wxALIGN_LEFT|wxALL, 5);

   wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer7, 0, wxGROW|wxALL, 0);

   wxTextCtrl* itemTextCtrl8 = new wxTextCtrl( itemStaticBoxSizer3->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer7->Add(itemTextCtrl8, 8, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxComboCtrl* itemComboCtrl9 = new wxComboCtrl( itemStaticBoxSizer3->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer7->Add(itemComboCtrl9, 4, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStdDialogButtonSizer* itemStdDialogButtonSizer1 = new wxStdDialogButtonSizer;

   itemStaticBoxSizer3->Add(itemStdDialogButtonSizer1, 0, wxALIGN_RIGHT|wxALL, 5);
   wxButton* itemButton2 = new wxButton( itemStaticBoxSizer3->GetStaticBox(), wxID_OK,
           _("Send"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStdDialogButtonSizer1->AddButton(itemButton2);

   wxButton* itemButton3 = new wxButton( itemStaticBoxSizer3->GetStaticBox(), wxID_CANCEL,
           _("Generate URL"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStdDialogButtonSizer1->AddButton(itemButton3);

   itemStdDialogButtonSizer1->Realize();

   wxStaticText* itemStaticText5 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           _("Generated URL for transfer:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer3->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALL, 5);

   wxTextCtrl* itemTextCtrl6 = new wxTextCtrl( itemStaticBoxSizer3->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer3->Add(itemTextCtrl6, 0, wxGROW|wxALL, 5);

   wxStaticBitmap* itemStaticBitmap7 = new wxStaticBitmap( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           qr_image, wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer3->Add(itemStaticBitmap7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(p_GWallet->panel, wxID_ANY, _("Receive"));
   wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxVERTICAL);
   itemBoxSizer2->Add(itemStaticBoxSizer11, 6, wxALIGN_TOP|wxALL, 5);

   wxStaticText* itemStaticText9 = new wxStaticText( itemStaticBoxSizer11->GetStaticBox(), wxID_STATIC,
           _("From:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer11->Add(itemStaticText9, 0, wxALIGN_LEFT|wxALL, 5);

   wxSearchCtrl* itemSearchCtrl10 = new wxSearchCtrl( itemStaticBoxSizer11->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer11->Add(itemSearchCtrl10, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText11 = new wxStaticText( itemStaticBoxSizer11->GetStaticBox(), wxID_STATIC,
           _("Amount and asset to receive:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer11->Add(itemStaticText11, 0, wxALIGN_LEFT|wxALL, 5);

   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer11->Add(itemBoxSizer3, 0, wxGROW|wxALL, 0);

   wxTextCtrl* itemTextCtrl4 = new wxTextCtrl( itemStaticBoxSizer11->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer3->Add(itemTextCtrl4, 8, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxComboCtrl* itemComboCtrl5 = new wxComboCtrl( itemStaticBoxSizer11->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer3->Add(itemComboCtrl5, 4, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxButton* itemButton1 = new wxButton( itemStaticBoxSizer11->GetStaticBox(), wxID_ANY,
           _("Generate URL"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer11->Add(itemButton1, 0, wxALIGN_RIGHT|wxALL, 5);

   wxStaticText* itemStaticText7 = new wxStaticText( itemStaticBoxSizer11->GetStaticBox(), wxID_STATIC,
           _("To receive asset simply send your account name and amount to the sender, send URL or QR:"),
           wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer11->Add(itemStaticText7, 0, wxGROW|wxALL, 5);

   wxTextCtrl* itemTextCtrl9 = new wxTextCtrl( itemStaticBoxSizer11->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemTextCtrl9->Enable(false);
   itemStaticBoxSizer11->Add(itemTextCtrl9, 0, wxGROW|wxALL, 5);

   wxStaticBitmap* itemStaticBitmap12 = new wxStaticBitmap( itemStaticBoxSizer11->GetStaticBox(), wxID_STATIC,
           qr_image, wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer11->Add(itemStaticBitmap12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   //from->SetSelection(0);
   //asset->SetSelection(0);

   p_GWallet->mainSizer->Hide(p_GWallet->sizerTransferMode, true);
   p_GWallet->mainSizer->Layout();
}

void SendReceive::CreateEvents()
{
   p_GWallet->panel->Connect(ID_TRANSFER_OK, wxEVT_COMMAND_BUTTON_CLICKED,
           wxCommandEventHandler(SendReceive::OnTransferOk), NULL, this);
}

void SendReceive::OnTransferOk(wxCommandEvent & event)
{
   wdump((p_GWallet->bitshares.wallet_api_ptr->info()));

   auto from_v = p_GWallet->strings_accounts[from->GetCurrentSelection()];
   auto to_v = to->GetValue();
   auto asset_v = p_GWallet->strings_assets[asset->GetCurrentSelection()];
   auto amount_v = amount->GetValue();

   try
   {
      auto result = p_GWallet->bitshares.wallet_api_ptr->transfer(from_v.ToStdString(), to_v.ToStdString(),
              amount_v.ToStdString(), asset_v.ToStdString(), "", false);
      wdump((result));
   }
   catch( const fc::exception& e )
   {
      p_GWallet->OnError(e.to_detail_string());
   }
}


