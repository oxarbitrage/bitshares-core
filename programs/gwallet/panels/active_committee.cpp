#include "../include/panels/active_committee.hpp"
#include "../include/panels/wallet.hpp"

ActiveCommittee::ActiveCommittee(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;

   wxWindowDisabler disableAll;
   wxBusyInfo wait(_("Please wait, updating active committee members info ..."));
   wxTheApp->Yield();

   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   response_grid->CreateGrid(11, 3);

   response_grid->SetColLabelValue(0, "ID");
   response_grid->SetColLabelValue(1, "Account");
   response_grid->SetColLabelValue(2, "Votes");

   response_grid->BeginBatch();
   response_grid->ClearSelection();

   const auto info = p_GWallet->bitshares.wallet_api_ptr->info();
   auto z = 0;
   for( auto& active_committee : info["active_committee_members"].get_array() ) {

      const auto committee = p_GWallet->bitshares.wallet_api_ptr->get_committee_member(active_committee.as_string());
      response_grid->SetCellValue(z, 0, active_committee.as_string());

      auto committee_account = p_GWallet->bitshares.wallet_api_ptr->get_account(string(object_id_type(committee.committee_member_account))).name;
      response_grid->SetCellValue(z, 1, committee_account);

      response_grid->SetCellValue(z, 2, wxNumberFormatter::ToString(long(committee.total_votes)));

      ++z;
   }

   response_grid->AutoSize();
   response_grid->ForceRefresh();
   response_grid->EndBatch();
}

