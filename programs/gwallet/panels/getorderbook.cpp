#include "../include/panels/getorderbook.hpp"
#include "../include/panels/wallet.hpp"
#include "../include/panels/cli.hpp"
#include "../include/panels/commands.hpp"

GetOrderBook::GetOrderBook(GWallet* gwallet)
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GetOrderBook::OnOk));
   Connect(XRCID("base"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(GetOrderBook::OnSearchAsset), NULL, this);
   Connect(XRCID("quote"), wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEventHandler(GetOrderBook::OnSearchAsset), NULL, this);
}

void GetOrderBook::OnSearchAsset(wxCommandEvent& event)
{
   const auto keyword = event.GetString().ToStdString();
   wxArrayString choices;
   auto findings = p_GWallet->bitshares.database_api->list_assets(keyword, 100);
   for(auto f : findings)
   {
      choices.Add(f.symbol);
   }

   wxSingleChoiceDialog dialog(this, _("Assets found"), _("Please select an asset"), choices);
   if (dialog.ShowModal() == wxID_OK) {

      wxSearchCtrl* s = wxDynamicCast(event.GetEventObject(), wxSearchCtrl);
      s->SetValue(dialog.GetStringSelection());
   }
}

void GetOrderBook::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto base_v = base->GetValue().ToStdString();
   const auto quote_v = quote->GetValue().ToStdString();
   const auto limit_v = limit->GetString(limit->GetSelection());

   order_book result;
   wxAny response;
   try
   {
      result = p_GWallet->bitshares.wallet_api_ptr->get_order_book(base_v, quote_v, wxAtoi(limit_v));
      response = result;
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(this, _("Invalid market"));
      //name->SetFocus();
      return;
   }

   new GetOrderBookResponse(p_GWallet, response, "asks");
   new GetOrderBookResponse(p_GWallet, response, "bids");

   if(cli->IsChecked())
   {
      auto command = "get_order_book " + base_v + " " + quote_v + " " + limit_v;
      p_GWallet->panels.p_cli->command->SetValue(command);
      wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, XRCID("run"));
      p_GWallet->panels.p_cli->OnCliCommand(event);
   }
}

GetOrderBookResponse::GetOrderBookResponse(GWallet* gwallet, wxAny any_response, string response_type)
{
   InitWidgetsFromXRC((wxWindow *)gwallet);

   response_grid->CreateGrid(10, 3);
   response_grid->SetColLabelValue(0, "Price");
   response_grid->SetColLabelValue(1, "Quote");
   response_grid->SetColLabelValue(2, "Base");
   response_grid->DeleteRows(0, 25);

   auto results = any_response.As<order_book>();
   auto z = 0;

   response_grid->BeginBatch();
   response_grid->ClearSelection();

   if(response_type == "asks") {
      response_grid->AppendRows(results.asks.size());
      for (auto &ask : results.asks) {

         response_grid->SetCellValue(z, 0, ask.price);
         response_grid->SetCellValue(z, 1, ask.quote);
         response_grid->SetCellValue(z, 2, ask.base);
         ++z;
      }
   }
   else
   {
      response_grid->AppendRows(results.bids.size());
      for (auto &bid : results.bids) {

         response_grid->SetCellValue(z, 0, bid.price);
         response_grid->SetCellValue(z, 1, bid.quote);
         response_grid->SetCellValue(z, 2, bid.base);
         ++z;
      }
   }
   response_grid->AutoSize();
   response_grid->ForceRefresh();
   response_grid->EndBatch();

   gwallet->panels.p_commands->notebook->AddPage(this, "Order book "+ response_type +" response");

}
