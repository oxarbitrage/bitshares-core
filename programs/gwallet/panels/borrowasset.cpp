#include "../include/panels/borrowasset.hpp"
#include "../include/panels/wallet.hpp"

BorrowAsset::BorrowAsset(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   seller->Append(p_GWallet->strings.accounts);
   seller->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(BorrowAsset::OnOk));
   Connect(XRCID("borrow_asset"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(BorrowAsset::OnSearchAsset), NULL, this);
}

void BorrowAsset::OnSearchAsset(wxCommandEvent& event)
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
      borrow_asset->SetValue(dialog.GetStringSelection());
}

void BorrowAsset::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto seller_value = p_GWallet->strings.accounts[seller->GetCurrentSelection()].ToStdString();
   const auto borrow_amount_value = borrow_amount->GetValue().ToStdString();
   const auto borrow_asset_value = borrow_asset->GetValue().ToStdString();
   const auto collateral_amount_value = collateral_amount->GetValue().ToStdString();

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_asset(borrow_asset_value);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Asset is invalid"));
      borrow_asset->SetFocus();
      return;
   }

   try {
      auto result = p_GWallet->bitshares.wallet_api_ptr->borrow_asset(seller_value, borrow_amount_value,
            borrow_asset_value, collateral_amount_value, false);
      if (wxYES == wxMessageBox(fc::json::to_pretty_string(result.operations[0]), _("Confirm Borrow Asset?"),
            wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
         p_GWallet->bitshares.wallet_api_ptr->borrow_asset(seller_value, borrow_amount_value,
               borrow_asset_value, collateral_amount_value, true);
         Close(true);
      }
   }
   catch (const fc::exception &e) {
      p_GWallet->OnError(this, e.to_detail_string());
   }
}
