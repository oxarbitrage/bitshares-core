#include "../include/panels/getcommitteemember.hpp"
#include "../include/panels/wallet.hpp"

GetCommitteeMember::GetCommitteeMember(GWallet* gwallet)
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GetCommitteeMember::OnOk));
   Connect(XRCID("owner_account"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(GetCommitteeMember::OnSearchAccount), NULL, this);
}

void GetCommitteeMember::OnSearchAccount(wxCommandEvent& event)
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

void GetCommitteeMember::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto account = owner_account->GetValue().ToStdString();
   committee_member_object result;
   try
   {
      result = p_GWallet->bitshares.wallet_api_ptr->get_committee_member(account);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Account is not a committee member"));
      owner_account->SetFocus();
      return;
   }

   wxAuiPaneInfo info;
   info.Top();
   info.Name("Committee member response");
   info.Caption("Committee member response");
   info.PinButton();
   info.Position(2);
   info.MaximizeButton();
   info.MinimizeButton();

   GetCommitteeMemberResponse *response_panel = new GetCommitteeMemberResponse(p_GWallet);

   const auto root = response_panel->response_tree->AddRoot("Committee member object");

   const auto id = response_panel->response_tree->AppendItem(root, "ID");
   response_panel->response_tree->AppendItem(id, string(object_id_type(result.id)));

   const auto committee_member_account = response_panel->response_tree->AppendItem(root, "Committee member account");
   response_panel->response_tree->AppendItem(committee_member_account, string(object_id_type(result.committee_member_account)));

   const auto vote_id = response_panel->response_tree->AppendItem(root, "Vote ID");
   response_panel->response_tree->AppendItem(vote_id, fc::json::to_string(result.vote_id));

   const auto total_votes = response_panel->response_tree->AppendItem(root, "Total votes");
   response_panel->response_tree->AppendItem(total_votes, wxNumberFormatter::ToString((long)(result.total_votes/pow(10, 5))));

   const auto url = response_panel->response_tree->AppendItem(root, "Url");
   response_panel->response_tree->AppendItem(url, fc::json::to_string(result.url));

   response_panel->response_tree->ExpandAll();

   p_GWallet->m_mgr.AddPane(response_panel, info);
   p_GWallet->m_mgr.Update();
}

GetCommitteeMemberResponse::GetCommitteeMemberResponse(GWallet* gwallet)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);
}

