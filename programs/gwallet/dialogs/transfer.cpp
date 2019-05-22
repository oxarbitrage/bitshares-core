#include "../include/dialogs/transfer.hpp"
#include "../include/modes/wallet.hpp"

TransferDialog::TransferDialog(wxWindow *parent) {
   InitWidgetsFromXRC((wxWindow *)parent);

   Wallet* p_Wallet = dynamic_cast<Wallet*>(GetParent());
   p_GWallet = p_Wallet->p_GWallet;

   wxTextValidator* numeric_validator = new wxTextValidator(wxFILTER_EMPTY|wxFILTER_NUMERIC);
   wxTextValidator* empty_validator = new wxTextValidator(wxFILTER_EMPTY);

   from->Append(p_GWallet->strings.accounts);
   from->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   to->SetValidator(*empty_validator);

   amount->SetValidator(*numeric_validator);

   asset->Append(p_GWallet->strings.assets);
   asset->SetSelection(p_GWallet->strings.assets.Index(p_GWallet->strings.selected_asset));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(TransferDialog::OnOk));
   Connect(XRCID("to"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(TransferDialog::OnSearchAccount), NULL, this);

   ShowModal();
}

void TransferDialog::OnSearchAccount(wxCommandEvent& event)
{
   const auto keyword = event.GetString().ToStdString();

   wxArrayString choices;
   auto findings = p_GWallet->bitshares.database_api->lookup_accounts(keyword, 100);
   for(auto f : findings)
   {
      choices.Add(f.first);
   }

   wxSingleChoiceDialog dialog(this, _("Accounts found"), _("Please select an account"), choices);
   if (dialog.ShowModal() == wxID_OK)

   to->SetValue(dialog.GetStringSelection());
}

void TransferDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto from_v = p_GWallet->strings.accounts[from->GetCurrentSelection()].ToStdString();
   const auto to_v = to->GetValue().ToStdString();
   const auto amount_v = amount->GetValue().ToStdString();
   const auto asset_v = p_GWallet->strings.assets[asset->GetCurrentSelection()].ToStdString();
   const auto memo_v = memo->GetValue().ToStdString();

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_account(to_v);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(_("Account is invalid"));
      to->SetFocus();
      return;
   }

   try {
      auto result = p_GWallet->bitshares.wallet_api_ptr->transfer(from_v, to_v, amount_v, asset_v, memo_v, false);
      if (wxYES == wxMessageBox(fc::json::to_pretty_string(result.operations[0]), _("Confirm transfer?"),
            wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
         p_GWallet->bitshares.wallet_api_ptr->transfer(from_v, to_v, amount_v, asset_v, memo_v, true);
         Close(true);
      }
   }
   catch (const fc::exception &e) {
      p_GWallet->OnError(e.to_detail_string());
   }
}

