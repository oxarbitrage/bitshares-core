#include "../include/modes/home.hpp"

#include <wx/statline.h>
#include <wx/numformatter.h>
#include <wx/busyinfo.h>

Home::Home(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;

   InitWidgetsFromXRC((wxWindow *)p_GWallet);
   DoInitialData();

   Connect(XRCID("ID_VIEW_WITNESSES"), wxEVT_HYPERLINK, wxHyperlinkEventHandler(Home::OnWitness), NULL, this);
   Connect(XRCID("ID_VIEW_COMMITTEE"), wxEVT_HYPERLINK, wxHyperlinkEventHandler(Home::OnCommittee), NULL, this);
}

void Home::DoInitialData()
{
   const auto info = p_GWallet->bitshares.wallet_api_ptr->info();

   head_block_num->SetLabel(wxNumberFormatter::ToString((long)info["head_block_num"].as_uint64()));
   head_block_age->SetLabel(info["head_block_age"].as_string());
   next_maintenance_time->SetLabel(info["next_maintenance_time"].as_string());
   partcipation->SetLabel(info["participation"].as_string());
   chain_id->SetLabel(info["chain_id"].as_string().substr(info["chain_id"].as_string().length()-11, 10));
   core_asset->SetLabel(GRAPHENE_SYMBOL);
   blockchain_name->SetLabel("Bitshares 3.0");

   const auto about = p_GWallet->bitshares.wallet_api_ptr->about();

   client_version->SetLabel(about["client_version"].as_string());
   graphene_revision->SetLabel(about["graphene_revision"].as_string());
   revision_age->SetLabel(about["graphene_revision_age"].as_string());
   fc_revision->SetLabel(about["fc_revision"].as_string());
   fc_revision_age->SetLabel(about["fc_revision_age"].as_string());
   compile_date->SetLabel(about["compile_date"].as_string());
   boost_revision->SetLabel(about["boost_version"].as_string());
   openssl_version->SetLabel(about["openssl_version"].as_string());
   os_build->SetLabel(about["build"].as_string());

   if(!p_GWallet->state.is_new)
      DoAccount(false);

   usd_bts->SetLabel(p_GWallet->bitshares.database_api->get_ticker("USD", "BTS").latest.substr(0, 8));
   cny_bts->SetLabel(p_GWallet->bitshares.database_api->get_ticker("CNY", "BTS").latest.substr(0, 8));
   eur_bts->SetLabel(p_GWallet->bitshares.database_api->get_ticker("EUR", "BTS").latest.substr(0, 8));
   gold_bts->SetLabel(p_GWallet->bitshares.database_api->get_ticker("GOLD", "BTS").latest.substr(0, 8));
   silver_bts->SetLabel(p_GWallet->bitshares.database_api->get_ticker("SILVER", "BTS").latest.substr(0, 8));
   btc_bts->SetLabel(p_GWallet->bitshares.database_api->get_ticker("BTC", "BTS").latest.substr(0, 8));
   ruble_bts->SetLabel(p_GWallet->bitshares.database_api->get_ticker("RUBLE", "BTS").latest.substr(0, 8));
   jpy_bts->SetLabel(p_GWallet->bitshares.database_api->get_ticker("JPY", "BTS").latest.substr(0, 8));
   cad_bts->SetLabel(p_GWallet->bitshares.database_api->get_ticker("CAD", "BTS").latest.substr(0, 8));
}

void Home::DoAccount(bool update_head_block)
{
   if(update_head_block) {
      const auto info = p_GWallet->bitshares.wallet_api_ptr->info();

      head_block_num->SetLabel(wxNumberFormatter::ToString((long)info["head_block_num"].as_uint64()));
      head_block_age->SetLabel(info["head_block_age"].as_string());
   }
   const auto account_object = p_GWallet->bitshares.wallet_api_ptr->get_account(p_GWallet->strings.selected_account.ToStdString());

   name->SetLabel(account_object.name);

   id->SetLabel(string(object_id_type(account_object.id)));

   const auto referrer_name = p_GWallet->bitshares.wallet_api_ptr->get_object(account_object.referrer).get_array()[0]["name"];
   referrer->SetLabel(referrer_name.as_string());

   const auto registrar_name = p_GWallet->bitshares.wallet_api_ptr->get_object(account_object.registrar).get_array()[0]["name"];
   registrar->SetLabel(registrar_name.as_string());

   auto voting_as_name = p_GWallet->bitshares.wallet_api_ptr->get_object(account_object.options.voting_account).get_array()[0]["name"];
   voting_as->SetLabel(voting_as_name.as_string());

   const auto stats = p_GWallet->bitshares.wallet_api_ptr->get_object(account_object.statistics).get_array()[0];

   total_ops->SetLabel(wxNumberFormatter::ToString((long)stats["total_ops"].as_uint64()));

   if(account_object.is_lifetime_member())
      membership_type->SetLabel(_("LIFETIME MEMBER"));
   else
      membership_type->SetLabel(_("FREE MEMBER"));

   last_vote_date->SetLabel(stats["last_vote_time"].as_string());
   auto pretty_balance = p_GWallet->DoPrettyBalance(5, std::stod(stats["lifetime_fees_paid"].as_string()));
   lifetime_fees_paid->SetLabel(pretty_balance + " " + GRAPHENE_SYMBOL);
}

void Home::OnWitness(wxHyperlinkEvent& event)
{
   wxDialog dlg;
   if (!wxXmlResource::Get()->LoadDialog(&dlg, NULL, "ID_ACTIVEWITNESS"))
      return;

   wxGrid* grid = XRCCTRL(dlg, "ID_GRID", wxGrid);
   if (grid) {
      grid->CreateGrid(21, 4);

      grid->SetColLabelValue(0, "ID");
      grid->SetColLabelValue(1, "Account");
      grid->SetColLabelValue(2, "Votes");
      grid->SetColLabelValue(3, "Last block");

      grid->BeginBatch();
      grid->ClearSelection();

      wxWindowDisabler disableAll;
      wxBusyInfo wait(_("Please wait, updating active witnesses info ..."));
      wxTheApp->Yield();

      const auto info = p_GWallet->bitshares.wallet_api_ptr->info();
      auto z = 0;
      for( auto& active_witness : info["active_witnesses"].get_array() ) {

         const auto witness = p_GWallet->bitshares.wallet_api_ptr->get_witness(active_witness.as_string());
         grid->SetCellValue(z, 0, active_witness.as_string());
         grid->SetCellValue(z, 1, string(object_id_type(witness.witness_account)));
         grid->SetCellValue(z, 2, wxNumberFormatter::ToString(long(witness.total_votes)));
         grid->SetCellValue(z, 3, wxNumberFormatter::ToString(long(witness.last_confirmed_block_num)));

         ++z;
      }

      //wait.~wxBusyInfo();
      grid->AutoSize();
      grid->ForceRefresh();
      grid->EndBatch();
   }

   dlg.ShowModal();

}

void Home::OnCommittee(wxHyperlinkEvent& event)
{
   wxDialog dlg;
   if (!wxXmlResource::Get()->LoadDialog(&dlg, NULL, "ID_ACTIVECOMMITTE"))
      return;

   wxGrid* grid = XRCCTRL(dlg, "ID_GRID", wxGrid);
   if (grid) {
      grid->CreateGrid(11, 3);

      grid->SetColLabelValue(0, "ID");
      grid->SetColLabelValue(1, "Account");
      grid->SetColLabelValue(2, "Votes");

      grid->BeginBatch();
      grid->ClearSelection();

      wxWindowDisabler disableAll;
      wxBusyInfo wait(_("Please wait, updating active committee members info ..."));
      wxTheApp->Yield();

      const auto info = p_GWallet->bitshares.wallet_api_ptr->info();
      auto z = 0;
      for( auto& active_committee : info["active_committee_members"].get_array() ) {

         const auto committee = p_GWallet->bitshares.wallet_api_ptr->get_committee_member(active_committee.as_string());
         grid->SetCellValue(z, 0, active_committee.as_string());
         grid->SetCellValue(z, 1, string(object_id_type(committee.committee_member_account)));
         grid->SetCellValue(z, 2, wxNumberFormatter::ToString(long(committee.total_votes)));

         ++z;
      }

      grid->AutoSize();
      grid->ForceRefresh();
      grid->EndBatch();
   }
   dlg.ShowModal();
}
