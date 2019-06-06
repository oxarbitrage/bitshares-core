#include "../include/panels/getaccounthistory.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

GetAccountHistory::GetAccountHistory(GWallet* gwallet)
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GetAccountHistory::OnOk));
   Connect(XRCID("name"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(GetAccountHistory::OnSearchAccount), NULL, this);

   name->SetValue(p_GWallet->strings.selected_account);
}

void GetAccountHistory::OnSearchAccount(wxCommandEvent& event)
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
      name->SetValue(dialog.GetStringSelection());
}

void GetAccountHistory::OnOk(wxCommandEvent& WXUNUSED(event))
{
   wxWindowDisabler disableAll;
   wxBusyInfo wait(_("Please wait, getting account history ..."));
   wxTheApp->Yield();

   const auto name_v = name->GetValue().ToStdString();
   const auto limit_v = limit->GetString(limit->GetSelection());

   vector<graphene::wallet::operation_detail> result;
   wxAny response;
   try
   {
      result = p_GWallet->bitshares.wallet_api_ptr->get_account_history(name_v, wxAtoi(limit_v));
      response = result;
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Account not found"));
      name->SetFocus();
      return;
   }

   new GetAccountHistoryResponse(p_GWallet, response);

   if(cli->IsChecked())
   {
      auto command = "get_account_history " + name_v + " " + limit_v;
      p_GWallet->panels.p_cli->command->SetValue(command);
      wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, XRCID("run"));
      p_GWallet->panels.p_cli->OnCliCommand(event);
   }
}

GetAccountHistoryResponse::GetAccountHistoryResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   response_grid->CreateGrid(10, 5);

   response_grid->SetColLabelValue(0, "ID");
   response_grid->SetColLabelValue(1, "Date");
   response_grid->SetColLabelValue(2, "Block");
   response_grid->SetColLabelValue(3, "Text");
   response_grid->SetColLabelValue(4, "Memo");

   response_grid->DeleteRows(0, 25);

   auto results = any_response.As<vector<graphene::wallet::operation_detail>>();

   auto z = 0;

   response_grid->BeginBatch();
   response_grid->ClearSelection();

   response_grid->AppendRows(results.size());
   for( auto& ah : results ) {

      const auto b = gwallet->bitshares.database_api->get_block_header(ah.op.block_num);
      FC_ASSERT(b);

      response_grid->SetCellValue(z, 0, fc::json::to_string(ah.op.id).substr(1, fc::json::to_string(ah.op.id).length() - 2));
      response_grid->SetCellValue(z, 1, b->timestamp.to_iso_string());
      response_grid->SetCellValue(z, 2, fc::to_string(ah.op.block_num));
      response_grid->SetCellValue(z, 3, ah.description);
      response_grid->SetCellValue(z, 4, ah.memo);

      ++z;
   }
   response_grid->AutoSize();
   response_grid->ForceRefresh();
   response_grid->EndBatch();

   gwallet->panels.p_commands->notebook->AddPage(this, "Account history response");
}
