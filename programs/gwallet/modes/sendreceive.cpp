#include "../include/modes/sendreceive.hpp"

SendReceive::SendReceive(GWallet* gwallet) : wxFrame()
{
   p_GWallet = gwallet;
}

void SendReceive::CreateControls()
{
   p_GWallet->sizerTransferMode = new wxBoxSizer(wxVERTICAL);
   p_GWallet->mainSizer->Add(p_GWallet->sizerTransferMode, 0, wxGROW|wxALL);

   wxBoxSizer* sizer4 = new wxBoxSizer(wxVERTICAL);
   p_GWallet->sizerTransferMode->Add(sizer4, 0, wxGROW|wxALL);

   wxStaticText* itemStaticText1 = new wxStaticText( p_GWallet->panel, wxID_STATIC, _("Send digital currency to a bitshares account"), wxDefaultPosition, wxDefaultSize, 0 );
   sizer4->Add(itemStaticText1, 0, wxALIGN_LEFT|wxALL, 5);

   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
   sizer4->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText4 = new wxStaticText( p_GWallet->panel, wxID_STATIC, _("From: "), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer3->Add(itemStaticText4, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   from = new wxComboBox( p_GWallet->panel, ID_TRANSFER_ACCOUNTS, wxEmptyString, wxDefaultPosition, wxDefaultSize, p_GWallet->strings_accounts, wxCB_DROPDOWN );
   itemBoxSizer3->Add(from, 10, wxGROW|wxALL, 5);

   wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
   sizer4->Add(itemBoxSizer7, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText8 = new wxStaticText( p_GWallet->panel, wxID_STATIC, _("To:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer7->Add(itemStaticText8, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   to = new wxTextCtrl( p_GWallet->panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer7->Add(to, 10, wxGROW|wxALL, 5);

   wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
   sizer4->Add(itemBoxSizer11, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText12 = new wxStaticText( p_GWallet->panel, wxID_STATIC, _("Asset:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer11->Add(itemStaticText12, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   asset = new wxComboBox( p_GWallet->panel, ID_TRANSFER_ASSETS, wxEmptyString, wxDefaultPosition, wxDefaultSize, p_GWallet->strings_assets, wxCB_DROPDOWN );
   itemBoxSizer11->Add(asset, 10, wxGROW|wxALL, 5);

   wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
   sizer4->Add(itemBoxSizer15, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText16 = new wxStaticText( p_GWallet->panel, wxID_STATIC, _("Amount:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer15->Add(itemStaticText16, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   amount = new wxTextCtrl( p_GWallet->panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
   itemBoxSizer15->Add(amount, 10, wxGROW|wxALL, 5);

   wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
   sizer4->Add(itemBoxSizer19, 0, wxGROW|wxALL, 5);

   wxCheckBox* itemCheckBox20 = new wxCheckBox( p_GWallet->panel, wxID_ANY, _("Broadcast transaction"), wxDefaultPosition, wxDefaultSize, 0 );
   itemCheckBox20->SetValue(true);
   itemBoxSizer19->Add(itemCheckBox20, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
   sizer4->Add(itemBoxSizer21, 0, wxGROW|wxALL, 5);

   wxButton* itemButton22 = new wxButton( p_GWallet->panel, ID_TRANSFER_OK, _("Do Transfer"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer21->Add(itemButton22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   from->SetSelection(0);
   asset->SetSelection(0);

   p_GWallet->mainSizer->Hide(p_GWallet->sizerTransferMode, true);
   p_GWallet->mainSizer->Layout();
}

void SendReceive::CreateEvents()
{
   p_GWallet->panel->Connect(ID_TRANSFER_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SendReceive::OnTransferOk), NULL, this);
}

void SendReceive::OnTransferOk(wxCommandEvent & event)
{
   wdump((p_GWallet->wallet.wallet_api->info()));

   auto from_v = p_GWallet->strings_accounts[from->GetCurrentSelection()];
   auto to_v = to->GetValue();
   auto asset_v = p_GWallet->strings_assets[asset->GetCurrentSelection()];
   auto amount_v = amount->GetValue();

   try
   {
      auto result = p_GWallet->wallet.wallet_api->transfer(from_v.ToStdString(), to_v.ToStdString(),
              amount_v.ToStdString(), asset_v.ToStdString(), "", false);
      wdump((result));
   }
   catch( const fc::exception& e )
   {
      p_GWallet->OnError(e.to_detail_string());
   }
}


