#include "../include/panels/sellasset.hpp"
#include "../include/panels/wallet.hpp"

SellAsset::SellAsset(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   seller->Append(p_GWallet->strings.accounts);
   seller->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   sell_asset->Append(p_GWallet->strings.assets);
   sell_asset->SetSelection(p_GWallet->strings.assets.Index(p_GWallet->strings.selected_asset));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SellAsset::OnOk));
   Connect(XRCID("receive_asset"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(SellAsset::OnSearchAsset), NULL, this);
}

void SellAsset::OnSearchAsset(wxCommandEvent& event)
{
   const auto keyword = event.GetString().ToStdString();
   wxArrayString choices;
   auto findings = p_GWallet->bitshares.database_api->list_assets(keyword, 100);
   for(auto f : findings)
   {
      choices.Add(f.symbol);
   }

   wxSingleChoiceDialog dialog(this, _("Assets found"), _("Please select an asset"), choices);
   if (dialog.ShowModal() == wxID_OK)
      receive_asset->SetValue(dialog.GetStringSelection());
}

void SellAsset::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto seller_value = p_GWallet->strings.accounts[seller->GetCurrentSelection()].ToStdString();
   const auto sell_amount_value = sell_amount->GetValue().ToStdString();
   const auto sell_asset_value = p_GWallet->strings.assets[sell_asset->GetCurrentSelection()].ToStdString();
   const auto receive_amount_value = receive_amount->GetValue().ToStdString();
   const auto receive_asset_value = receive_asset->GetValue().ToStdString();
   const auto fill_or_kill_value = fill_or_kill->GetValue() ? true : false;

   const auto date_value = date->GetValue().ToUTC().GetValue().ToLong()/1000;
   const auto time_second = time->GetValue().ToUTC().GetSecond();
   const auto time_minute = time->GetValue().ToUTC().GetMinute();
   const auto time_hour = time->GetValue().ToUTC().GetHour();
   const auto time_value = (time_hour*3600) + (time_minute*60) + time_second;
   auto now = wxDateTime::Now().ToUTC().GetValue().ToLong()/1000;
   uint32_t expiration_value = date_value + time_value - now;

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_asset(receive_asset_value);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Asset is invalid"));
      receive_asset->SetFocus();
      return;
   }

   try {
      auto result = p_GWallet->bitshares.wallet_api_ptr->sell_asset(seller_value, sell_amount_value,
            sell_asset_value, receive_amount_value, receive_asset_value, expiration_value, fill_or_kill_value, false);
      if (wxYES == wxMessageBox(fc::json::to_pretty_string(result.operations[0]), _("Confirm Sell Asset?"),
            wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
         p_GWallet->bitshares.wallet_api_ptr->sell_asset(seller_value, sell_amount_value,
               sell_asset_value, receive_amount_value, receive_asset_value, expiration_value, fill_or_kill_value, true);
         Close(true);
      }
   }
   catch (const fc::exception &e) {
      p_GWallet->OnError(this, e.to_detail_string());
   }
}
