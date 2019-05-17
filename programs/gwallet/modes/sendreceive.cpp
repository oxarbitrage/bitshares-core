#include "../include/modes/sendreceive.hpp"

SendReceive::SendReceive(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   Connect(XRCID("send_from"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(SendReceive::OnSearchAccount), NULL, this);
   Connect(XRCID("receive_asset"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(SendReceive::OnSearchAsset), NULL, this);
   Connect(XRCID("send"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SendReceive::OnTransferOk), NULL, this);
   Connect(XRCID("generate_url_send"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SendReceive::OnSendUrl), NULL, this);
   Connect(XRCID("generate_url_receive"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SendReceive::OnReceiveUrl), NULL, this);

   send_asset->Set(p_GWallet->strings.assets);
   send_asset->SetSelection(p_GWallet->strings.assets.Index(p_GWallet->strings.selected_asset));
}

void SendReceive::OnSearchAccount(wxCommandEvent & event)
{
   const auto keyword = event.GetString();
   wdump((keyword.ToStdString()));
}

void SendReceive::OnSearchAsset(wxCommandEvent & event)
{
   const auto keyword = event.GetString();
   wdump((keyword.ToStdString()));
}

void SendReceive::OnTransferOk(wxCommandEvent & event)
{
   if (ValidateSend()) {
      const auto to_v = send_to->GetValue().ToStdString();
      const auto amount_v = send_amount->GetValue().ToStdString();
      const auto asset_v = p_GWallet->strings.assets[send_asset->GetCurrentSelection()].ToStdString();
      const auto from_v = p_GWallet->strings.selected_account;

      try {
         const auto result = p_GWallet->bitshares.wallet_api_ptr->transfer(from_v.ToStdString(), to_v,
               amount_v, asset_v, "", false);

         if (wxYES == wxMessageBox(fc::json::to_pretty_string(result.operations[0]), _("Confirm transfer?"),
               wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
            p_GWallet->bitshares.wallet_api_ptr->transfer(from_v.ToStdString(), to_v, amount_v, asset_v, "", true);
         }
      }
      catch(const fc::exception &e) {
         p_GWallet->OnError(e.to_detail_string());
      }
   }
}
void SendReceive::OnSendUrl(wxCommandEvent &event)
{
   if(ValidateSend()) {
      const auto to_v = send_to->GetValue().ToStdString();
      const auto amount_v = send_amount->GetValue().ToStdString();
      const auto asset_v = p_GWallet->strings.assets[send_asset->GetCurrentSelection()].ToStdString();
      const auto from_v = p_GWallet->strings.selected_account;

      const auto url = "bitshares://operation/transfer?to=" + to_v + "&from=" + from_v + "&asset=" + asset_v + "&amount=" +
            amount_v;

      generated_url_send->WriteText(url);
      generated_url_send->Enable(true);
   }
}

void SendReceive::OnReceiveUrl(wxCommandEvent &event)
{
   if(ValidateReceive()) {
      const auto from_v = receive_from->GetValue().ToStdString();
      const auto amount_v = receive_amount->GetValue().ToStdString();
      const auto asset_v = receive_asset->GetValue().ToStdString();
      const auto to_v = p_GWallet->strings.selected_account;

      const auto url = "bitshares://operation/transfer?to=" + to_v + "&from=" + from_v + "&asset=" + asset_v + "&amount=" +
            amount_v;

      generated_url_receive->WriteText(url);
      generated_url_receive->Enable(true);
   }
}

bool SendReceive::ValidateSend()
{
   if(send_to->GetValue() == "") {
      wxMessageDialog dialog( NULL, _("Receipient is empty"), _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if (dialog.ShowModal() == wxID_OK)
      {
         send_to->SetFocus();
         return false;
      }
   }
   else
   {
      try
      {
         p_GWallet->bitshares.wallet_api_ptr->get_account(send_to->GetValue().ToStdString());
      }
      catch(const fc::exception& e)
      {
         p_GWallet->OnError("Account is invalid");
         send_to->SetFocus();
         return false;
      }
   }
   if(send_amount->GetValue() == "") {
      wxMessageDialog dialog( NULL, _("Amount is empty"), _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if (dialog.ShowModal() == wxID_OK)
      {
         send_amount->SetFocus();
         return false;
      }
   }
   return true;
}

bool SendReceive::ValidateReceive()
{
   if(receive_from->GetValue() == "") {
      wxMessageDialog dialog( NULL, _("Receipient is empty"), _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if (dialog.ShowModal() == wxID_OK)
      {
         receive_from->SetFocus();
         return false;
      }
   }
   else
   {
      try
      {
         p_GWallet->bitshares.wallet_api_ptr->get_account(receive_from->GetValue().ToStdString());
      }
      catch(const fc::exception& e)
      {
         p_GWallet->OnError("Account is invalid");
         receive_from->SetFocus();
         return false;
      }
   }
   if(receive_amount->GetValue() == "") {
      wxMessageDialog dialog( NULL, _("Amount is empty"), _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if (dialog.ShowModal() == wxID_OK)
      {
         receive_amount->SetFocus();
         return false;
      }
   }
   if(receive_asset->GetValue() == "") {
      wxMessageDialog dialog( NULL, _("Asset is empty"), _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if (dialog.ShowModal() == wxID_OK)
      {
         receive_asset->SetFocus();
         return false;
      }
   }
   else
   {
      try
      {
         p_GWallet->bitshares.wallet_api_ptr->get_asset(receive_asset->GetValue().ToStdString());
      }
      catch(const fc::exception& e)
      {
         p_GWallet->OnError("Asset is invalid");
         receive_asset->SetFocus();
         return false;
      }
   }
   return true;
}
