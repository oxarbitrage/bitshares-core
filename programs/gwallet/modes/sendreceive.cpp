#include "../include/modes/sendreceive.hpp"

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

   send_to = new wxSearchCtrl( itemStaticBoxSizer3->GetStaticBox(), ID_SEND_FROM,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY) );
   itemStaticBoxSizer3->Add(send_to, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText6 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           _("Amount:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer3->Add(itemStaticText6, 0, wxALIGN_LEFT|wxALL, 5);

   wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer7, 0, wxGROW|wxALL, 0);

   send_amount = new wxTextCtrl( itemStaticBoxSizer3->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY|wxFILTER_DIGITS) );
   itemBoxSizer7->Add(send_amount, 8, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   send_asset = new wxComboBox( itemStaticBoxSizer3->GetStaticBox(), wxID_ANY, p_GWallet->selected_asset,
           wxDefaultPosition, wxDefaultSize, p_GWallet->strings_assets, wxCB_DROPDOWN, wxTextValidator(wxFILTER_EMPTY) );
   itemBoxSizer7->Add(send_asset, 4, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer4, 0, wxALIGN_RIGHT|wxALL, 0);

   wxButton* itemButton5 = new wxButton( itemStaticBoxSizer3->GetStaticBox(), ID_SEND_GENERATE_URL,
           _("Generate Url"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer4->Add(itemButton5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxButton* itemButton6 = new wxButton( itemStaticBoxSizer3->GetStaticBox(), ID_TRANSFER_OK,
           _("Send"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer4->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText5 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           _("Generated URL for transfer:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer3->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALL, 5);

   send_url = new wxTextCtrl( itemStaticBoxSizer3->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   send_url->Enable(false);
   itemStaticBoxSizer3->Add(send_url, 0, wxGROW|wxALL, 5);

   wxStaticBitmap* itemStaticBitmap7 = new wxStaticBitmap( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           qr_image, wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer3->Add(itemStaticBitmap7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(p_GWallet->panel, wxID_ANY, _("Receive"));
   wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxVERTICAL);
   itemBoxSizer2->Add(itemStaticBoxSizer11, 6, wxALIGN_TOP|wxALL, 5);

   wxStaticText* itemStaticText9 = new wxStaticText( itemStaticBoxSizer11->GetStaticBox(), wxID_STATIC,
           _("From:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer11->Add(itemStaticText9, 0, wxALIGN_LEFT|wxALL, 5);

   receive_from = new wxSearchCtrl( itemStaticBoxSizer11->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY) );
   itemStaticBoxSizer11->Add(receive_from, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText11 = new wxStaticText( itemStaticBoxSizer11->GetStaticBox(), wxID_STATIC,
           _("Amount and asset to receive:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer11->Add(itemStaticText11, 0, wxALIGN_LEFT|wxALL, 5);

   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer11->Add(itemBoxSizer3, 0, wxGROW|wxALL, 0);

   receive_amount = new wxTextCtrl( itemStaticBoxSizer11->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY|wxFILTER_DIGITS) );
   itemBoxSizer3->Add(receive_amount, 8, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   receive_asset = new wxSearchCtrl( itemStaticBoxSizer11->GetStaticBox(), ID_RECEIVE_ASSET,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY) );
   itemBoxSizer3->Add(receive_asset, 4, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxButton* itemButton1 = new wxButton( itemStaticBoxSizer11->GetStaticBox(), ID_RECEIVE_GENERATE_URL,
           _("Generate URL"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer11->Add(itemButton1, 0, wxALIGN_RIGHT|wxALL, 5);

   wxStaticText* itemStaticText7 = new wxStaticText( itemStaticBoxSizer11->GetStaticBox(), wxID_STATIC,
           _("To receive asset simply send your account name and amount to the sender, send URL or QR:"),
           wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer11->Add(itemStaticText7, 0, wxGROW|wxALL, 5);

   receive_url = new wxTextCtrl( itemStaticBoxSizer11->GetStaticBox(), wxID_ANY,
           wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   receive_url->Enable(false);
   itemStaticBoxSizer11->Add(receive_url, 0, wxGROW|wxALL, 5);

   wxStaticBitmap* itemStaticBitmap12 = new wxStaticBitmap( itemStaticBoxSizer11->GetStaticBox(), wxID_STATIC,
           qr_image, wxDefaultPosition, wxDefaultSize, 0 );
   itemStaticBoxSizer11->Add(itemStaticBitmap12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   p_GWallet->mainSizer->Hide(p_GWallet->sizerTransferMode, true);
   p_GWallet->mainSizer->Layout();
}

void SendReceive::CreateEvents()
{
   p_GWallet->panel->Connect(ID_SEND_FROM, wxEVT_SEARCHCTRL_SEARCH_BTN,
           wxCommandEventHandler(SendReceive::OnSearchAccount), NULL, this);

   p_GWallet->panel->Connect(ID_RECEIVE_ASSET, wxEVT_SEARCHCTRL_SEARCH_BTN,
           wxCommandEventHandler(SendReceive::OnSearchAsset), NULL, this);

   p_GWallet->panel->Connect(ID_TRANSFER_OK, wxEVT_COMMAND_BUTTON_CLICKED,
           wxCommandEventHandler(SendReceive::OnTransferOk), NULL, this);

   p_GWallet->panel->Connect(ID_SEND_GENERATE_URL, wxEVT_COMMAND_BUTTON_CLICKED,
           wxCommandEventHandler(SendReceive::OnSendUrl), NULL, this);

   p_GWallet->panel->Connect(ID_RECEIVE_GENERATE_URL, wxEVT_COMMAND_BUTTON_CLICKED,
           wxCommandEventHandler(SendReceive::OnReceiveUrl), NULL, this);
}

void SendReceive::OnSearchAccount(wxCommandEvent & event)
{
   auto keyword = event.GetString();
   wdump((keyword.ToStdString()));
}

void SendReceive::OnSearchAsset(wxCommandEvent & event)
{
   auto keyword = event.GetString();
   wdump((keyword.ToStdString()));
}

void SendReceive::OnTransferOk(wxCommandEvent & event)
{
   auto to_v = send_to->GetValue().ToStdString();
   auto amount_v = send_amount->GetValue().ToStdString();
   auto asset_v = p_GWallet->strings_assets[send_asset->GetCurrentSelection()].ToStdString();
   auto from_v = p_GWallet->selected_account;

   try
   {
      auto result = p_GWallet->bitshares.wallet_api_ptr->transfer(from_v.ToStdString(), to_v,
              amount_v, asset_v, "", false);

      if (wxYES == wxMessageBox(fc::json::to_pretty_string(result.operations[0]), _("Confirm transfer?"),
              wxNO_DEFAULT|wxYES_NO|wxICON_QUESTION, this))
      {
         p_GWallet->bitshares.wallet_api_ptr->transfer(from_v.ToStdString(), to_v, amount_v, asset_v, "", true);
      }
   }
   catch( const fc::exception& e )
   {
      p_GWallet->OnError(e.to_detail_string());
   }
}

void SendReceive::OnSendUrl(wxCommandEvent &event)
{
   auto to_v = send_to->GetValue().ToStdString();
   auto amount_v = send_amount->GetValue().ToStdString();
   auto asset_v = p_GWallet->strings_assets[send_asset->GetCurrentSelection()].ToStdString();
   auto from_v = p_GWallet->selected_account;

   auto url = "bitshares://operation/transfer?to="+to_v+"&from="+from_v+"&asset="+asset_v+"&amount="+amount_v;

   send_url->WriteText(url);
   send_url->Enable(true);
}

void SendReceive::OnReceiveUrl(wxCommandEvent &event)
{
   auto from_v = receive_from->GetValue().ToStdString();
   auto amount_v = receive_amount->GetValue().ToStdString();
   auto asset_v = receive_asset->GetValue().ToStdString();
   auto to_v = p_GWallet->selected_account;

   auto url = "bitshares://operation/transfer?to="+to_v+"&from="+from_v+"&asset="+asset_v+"&amount="+amount_v;

   receive_url->WriteText(url);
   receive_url->Enable(true);
}
