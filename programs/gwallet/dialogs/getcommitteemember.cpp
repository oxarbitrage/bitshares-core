#include "../include/dialogs/getcommitteemember.hpp"
#include "../include/modes/wallet.hpp"

#include <wx/wx.h>
#include <wx/numformatter.h>

GetCommitteeMemberDialog::GetCommitteeMemberDialog(wxWindow* parent)
{
   InitWidgetsFromXRC((wxWindow *)parent);

   Wallet* p_Wallet = dynamic_cast<Wallet*>(GetParent());
   p_GWallet = p_Wallet->p_GWallet;

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GetCommitteeMemberDialog::OnOk));
   Connect(XRCID("owner_account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(GetCommitteeMemberDialog::OnSearchAccount), NULL, this);

   ShowModal();
}

void GetCommitteeMemberDialog::OnSearchAccount(wxCommandEvent& event)
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
      owner_account->SetValue(dialog.GetStringSelection());
}

void GetCommitteeMemberDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto account = owner_account->GetValue().ToStdString();
   committee_member_object result;
   try
   {
      result = p_GWallet->bitshares.wallet_api_ptr->get_committee_member(account);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(_("Account is not a committee member"));
      owner_account->SetFocus();
      return;
   }

   response_panel->Show(true);
   const auto root = response_tree->AddRoot("Committee member object");

   const auto id = response_tree->AppendItem(root, "ID");
   response_tree->AppendItem(id, string(object_id_type(result.id)));

   const auto committee_member_account = response_tree->AppendItem(root, "Committee member account");
   response_tree->AppendItem(committee_member_account, string(object_id_type(result.committee_member_account)));

   const auto vote_id = response_tree->AppendItem(root, "Vote ID");
   response_tree->AppendItem(vote_id, fc::json::to_string(result.vote_id));

   const auto total_votes = response_tree->AppendItem(root, "Total votes");
   response_tree->AppendItem(total_votes, wxNumberFormatter::ToString((long)(result.total_votes/pow(10, 5))));

   const auto url = response_tree->AppendItem(root, "Url");
   response_tree->AppendItem(url, fc::json::to_string(result.url));

   response_tree->ExpandAll();

   Fit();
}