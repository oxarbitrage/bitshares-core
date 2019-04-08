#include "../include/modes/history.hpp"

History::History(GWallet* gwallet) : wxFrame()
{
   p_GWallet = gwallet;
}

void History::CreateControls()
{
   p_GWallet->sizers.history = new wxBoxSizer(wxVERTICAL);
   p_GWallet->sizers.main->Add(p_GWallet->sizers.history, 0, wxGROW|wxALL);

   grid = new wxGrid(p_GWallet->panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
   grid->CreateGrid(10, 5);

   grid->SetColLabelValue(0, "ID");
   grid->SetColLabelValue(1, "Date");
   grid->SetColLabelValue(2, "Block");
   grid->SetColLabelValue(3, "Text");
   grid->SetColLabelValue(4, "Memo");
   //grid->SetColLabelValue(5, "Raw");

   grid->Enable(false);
   p_GWallet->sizers.history->Add(grid, 0, wxALL, 5);

   p_GWallet->sizers.main->Hide(p_GWallet->sizers.history, true);
   p_GWallet->sizers.main->Layout();
}

void History::CreateEvents()
{

}

void History::DoHistory(std::string account)
{
   grid->DeleteRows(0, 25);

   auto my_account_history = p_GWallet->bitshares.wallet_api_ptr->get_account_history(account, 25);

   auto z = 0;

   grid->Enable(true);

   grid->BeginBatch();
   grid->ClearSelection();

   grid->AppendRows(my_account_history.size());
   for( auto& ah : my_account_history ) {

      auto b = p_GWallet->bitshares.database_api->get_block_header(ah.op.block_num);
      FC_ASSERT(b);

      grid->SetCellValue(z, 0, fc::json::to_string(ah.op.id).substr(1, fc::json::to_string(ah.op.id).length() - 2));
      grid->SetCellValue(z, 1, b->timestamp.to_iso_string());
      grid->SetCellValue(z, 2, fc::to_string(ah.op.block_num));
      grid->SetCellValue(z, 3, ah.description);
      grid->SetCellValue(z, 4, ah.memo);
      //grid->SetCellValue(z, 5, fc::json::to_string(ah.op));

      ++z;
   }
   grid->AutoSize();
   grid->ForceRefresh();
   grid->EndBatch();
}

