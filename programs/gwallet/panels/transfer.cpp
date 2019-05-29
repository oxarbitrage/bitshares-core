#include "../include/panels/transfer.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"

Transfer::Transfer(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   wxTextValidator* numeric_validator = new wxTextValidator(wxFILTER_EMPTY|wxFILTER_NUMERIC);
   wxTextValidator* empty_validator = new wxTextValidator(wxFILTER_EMPTY);

   from->Append(p_GWallet->strings.accounts);
   from->SetSelection(p_GWallet->strings.accounts.Index(p_GWallet->strings.selected_account));

   to->SetValidator(*empty_validator);

   amount->SetValidator(*numeric_validator);

   asset->Append(p_GWallet->strings.assets);
   asset->SetSelection(p_GWallet->strings.assets.Index(p_GWallet->strings.selected_asset));

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Transfer::OnOk));
   Connect(XRCID("to"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(Transfer::OnSearchAccount), NULL, this);
}

void Transfer::OnSearchAccount(wxCommandEvent& event)
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

void Transfer::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto from_v = p_GWallet->strings.accounts[from->GetCurrentSelection()].ToStdString();
   const auto to_v = to->GetValue().ToStdString();
   const auto amount_v = amount->GetValue().ToStdString();
   const auto asset_v = p_GWallet->strings.assets[asset->GetCurrentSelection()].ToStdString();
   const auto memo_v = memo->GetValue().ToStdString();
   string broadcast_v = "false";
   if(broadcast->IsChecked())
      broadcast_v = "true";

   wxWindow* wnd = p_GWallet->m_mgr.GetPane("Transfer").window;

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_account(to_v);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(wnd, _("Account is invalid"));
      to->SetFocus();
      return;
   }

   signed_transaction result_obj;
   wxAny response;

   if(cli->IsChecked())
   {
      auto command = "transfer " + from_v + " " + to_v + " " + amount_v + " " + asset_v + " \"" + memo_v + "\" " + broadcast_v;
      p_GWallet->panels.p_cli->command->SetValue(command);
      wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, XRCID("run"));
      p_GWallet->panels.p_cli->OnCliCommand(event);
   }

   else
   {
      try {
         auto result_obj = p_GWallet->bitshares.wallet_api_ptr->transfer(from_v, to_v, amount_v, asset_v, memo_v, false);

         if(broadcast->IsChecked()) {
            if (wxYES == wxMessageBox(fc::json::to_pretty_string(result_obj.operations[0]), _("Confirm transfer?"),
                  wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
               result_obj = p_GWallet->bitshares.wallet_api_ptr->transfer(from_v, to_v, amount_v, asset_v, memo_v, true);
            }
         }
         response = result_obj;
         new TransferResponse(p_GWallet, response);
      }
      catch (const fc::exception &e) {
         p_GWallet->OnError(wnd, e.to_detail_string());
      }
   }
}

TransferResponse::TransferResponse(GWallet* gwallet, wxAny any_response)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   wxAuiPaneInfo info;
   info.Top();
   info.Name("Transfer response");
   info.Caption("Transfer response");
   info.PinButton();
   info.Position(3);
   info.MaximizeButton();
   info.MinimizeButton();

   signed_transaction result = any_response.As<signed_transaction>();

   const auto root = response_tree->AddRoot("Signed Transaction");

   const auto ref_block_num = response_tree->AppendItem(root, "Reference block number");
   response_tree->AppendItem(ref_block_num, to_string(result.ref_block_num));

   const auto ref_block_prefix = response_tree->AppendItem(root, "Reference block prefix");
   response_tree->AppendItem(ref_block_prefix, to_string(result.ref_block_prefix));

   const auto expiration = response_tree->AppendItem(root, "Expiration");
   response_tree->AppendItem(expiration, result.expiration.to_iso_string());

   const auto operations = response_tree->AppendItem(root, "Operations");
   response_tree->AppendItem(operations, fc::json::to_string(result.operations));

   const auto extensions = response_tree->AppendItem(root, "Extensions");
   response_tree->AppendItem(extensions, fc::json::to_string(result.extensions));

   const auto signatures = response_tree->AppendItem(root, "Signatures");
   response_tree->AppendItem(signatures, fc::json::to_string(result.signatures));

   response_tree->ExpandAll();

   gwallet->m_mgr.AddPane(this, info);
   gwallet->m_mgr.Update();
}
