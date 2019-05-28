#include "../include/panels/setproxy.hpp"
#include "../include/panels/wallet.hpp"

SetProxy::SetProxy(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   DoVotingAccount();

   account_to_modify->Append(p_GWallet->strings.accounts);
   account_to_modify->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   voting_account->SetLabelText(current_voting_account);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SetProxy::OnOk));
   Connect(XRCID("voting_account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(SetProxy::OnSearchAccount), NULL, this);
}

void SetProxy::OnSearchAccount(wxCommandEvent& event)
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

      voting_account->SetValue(dialog.GetStringSelection());
}

void SetProxy::DoVotingAccount()
{
   const auto selected_account = p_GWallet->strings.selected_account.ToStdString();

   try
   {
      const auto account = p_GWallet->bitshares.wallet_api_ptr->get_account(selected_account);
      const auto account_id_string = std::string(object_id_type(account.options.voting_account));
      current_voting_account = p_GWallet->bitshares.wallet_api_ptr->get_account(account_id_string).name;
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, e.to_detail_string());
   }
}

void SetProxy::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto account_value = p_GWallet->strings.accounts[account_to_modify->GetCurrentSelection()].ToStdString();
   const auto voting_account_value = voting_account->GetValue().ToStdString();

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_account(account_value);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Account is invalid"));
      voting_account->SetFocus();
      return;
   }

   try
   {
      auto result = p_GWallet->bitshares.wallet_api_ptr->set_voting_proxy(account_value, voting_account_value, false);
      if (wxYES == wxMessageBox(fc::json::to_pretty_string(result.operations[0]), _("Confirm update of voting account?"),
            wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
         p_GWallet->bitshares.wallet_api_ptr->set_voting_proxy(account_value, voting_account_value, true);
         Close(true);
      }
   }
   catch (const fc::exception &e) {
      p_GWallet->OnError(this, e.to_detail_string());
   }
}



