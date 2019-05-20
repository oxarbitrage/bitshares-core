#include "../include/dialogs/setproxy.hpp"
#include "../include/modes/wallet.hpp"

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/combo.h>

SetProxyDialog::SetProxyDialog(wxWindow* parent)
{
   InitWidgetsFromXRC((wxWindow *)parent);

   Wallet* p_Wallet = dynamic_cast<Wallet*>(GetParent());
   p_GWallet = p_Wallet->p_GWallet;

   DoVotingAccount();

   account_to_modify->Append(p_GWallet->strings.accounts);
   account_to_modify->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   voting_account->SetLabelText(current_voting_account);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SetProxyDialog::OnOk));

   ShowModal();
}

void SetProxyDialog::DoVotingAccount()
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

   }
}

void SetProxyDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto account_value = p_GWallet->strings.accounts[account_to_modify->GetCurrentSelection()].ToStdString();
   const auto voting_account_value = voting_account->GetValue().ToStdString();

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_account(account_value);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(_("Account is invalid"));
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
      p_GWallet->OnError(e.to_detail_string());
   }
}



