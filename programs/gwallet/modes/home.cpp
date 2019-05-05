#include "../include/modes/home.hpp"

#include <wx/statline.h>
#include <wx/numformatter.h>
#include <wx/busyinfo.h>

Home::Home(GWallet* gwallet) : wxFrame()
{
   p_GWallet = gwallet;
}

void Home::CreateControls()
{
   DoTimers();
   DoInitialData();

   p_GWallet->sizers.home = new wxBoxSizer(wxVERTICAL);
   p_GWallet->sizers.main->Add(p_GWallet->sizers.home, 0, wxGROW|wxALL);

   wxGridSizer* itemGridSizer2 = new wxGridSizer(0, 2, 0, 0);

   wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(p_GWallet->panel, wxID_ANY, _("Blockchain Information"));
   wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
   itemGridSizer2->Add(itemStaticBoxSizer3, 6, wxGROW|wxALIGN_TOP|wxALL, 5);

   wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText5 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
         _("Head block number:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer4->Add(itemStaticText5, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   blockchain.head_block_num = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
         blockchain.head_block_number_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer4->Add(blockchain.head_block_num, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer7, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText8 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
         _("Block age:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer7->Add(itemStaticText8, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   blockchain.head_block_age = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
         blockchain.head_block_age_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer7->Add(blockchain.head_block_age, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer1, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText2 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
         _("Next maintenance time:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer1->Add(itemStaticText2, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   blockchain.next_maintenance_time = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
         blockchain.next_maintenance_time_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer1->Add(blockchain.next_maintenance_time, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer5, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText7 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
         _("Participation:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer5->Add(itemStaticText7, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   blockchain.participation = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
         blockchain.participation_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer5->Add(blockchain.participation, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer11, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText12 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
         _("Active witnesses:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer11->Add(itemStaticText12, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxHyperlinkCtrl* itemHyperlinkCtrl13 = new wxHyperlinkCtrl( itemStaticBoxSizer3->GetStaticBox(), ID_VIEW_WITNESSES,
         _("View"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_ALIGN_LEFT );
   itemBoxSizer11->Add(itemHyperlinkCtrl13, 6, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer14, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText15 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
         _("Active committee:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer14->Add(itemStaticText15, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxHyperlinkCtrl* itemHyperlinkCtrl16 = new wxHyperlinkCtrl( itemStaticBoxSizer3->GetStaticBox(), ID_VIEW_COMMITTEE,
         _("View"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_ALIGN_LEFT );
   itemBoxSizer14->Add(itemHyperlinkCtrl16, 6, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer9, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText26 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
         _("Chain ID:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer9->Add(itemStaticText26, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText29 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
         blockchain.chain_id_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer9->Add(itemStaticText29, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticBox* itemStaticBoxSizer1Static = new wxStaticBox(p_GWallet->panel, wxID_ANY, _("Account Information"));
   wxStaticBoxSizer* itemStaticBoxSizer1 = new wxStaticBoxSizer(itemStaticBoxSizer1Static, wxVERTICAL);
   itemGridSizer2->Add(itemStaticBoxSizer1, 6, wxGROW|wxALIGN_TOP|wxALL, 5);

   wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer2, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText4 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
         _("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer2->Add(itemStaticText4, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   account.name = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
         account.name_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer2->Add(account.name, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer12, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText13 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
         _("ID:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer12->Add(itemStaticText13, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   account.id = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
         account.id_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer12->Add(account.id, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer8, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText20 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
         _("Referrer"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer8->Add(itemStaticText20, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   account.referrer = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
         account.referrer_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer8->Add(account.referrer, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer30, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText32 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
         _("Registrar"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer30->Add(itemStaticText32, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   account.registrar = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
         account.registrar_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer30->Add(account.registrar, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer36 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer36, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText38 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
         _("Voting as"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer36->Add(itemStaticText38, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   account.voting_as = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
         account.voting_as_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer36->Add(account.voting_as, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer43 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer43, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText45 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
         _("Total ops"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer43->Add(itemStaticText45, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   account.total_ops = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
         account.total_ops_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer43->Add(account.total_ops, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticBox* itemStaticBoxSizer2Static = new wxStaticBox(p_GWallet->panel, wxID_ANY, _("Software Information"));
   wxStaticBoxSizer* itemStaticBoxSizer2 = new wxStaticBoxSizer(itemStaticBoxSizer2Static, wxVERTICAL);
   itemGridSizer2->Add(itemStaticBoxSizer2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText16 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         _("Client version:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer3->Add(itemStaticText16, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText17 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         software.client_version_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer3->Add(itemStaticText17, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer6, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText18 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         _("Graphene revision:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer6->Add(itemStaticText18, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText19 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         software.graphene_revision_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer6->Add(itemStaticText19, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer20, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText21 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         _("Revision age:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer20->Add(itemStaticText21, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText22 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         software.graphene_revision_age_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer20->Add(itemStaticText22, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer23, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText24 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         _("FC revision:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer23->Add(itemStaticText24, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText25 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         software.fc_revision_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer23->Add(itemStaticText25, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer26, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText27 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         _("FC revision age:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer26->Add(itemStaticText27, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText28 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         software.fc_revision_age_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer26->Add(itemStaticText28, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer29 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer29, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText30 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         _("Compile date:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer29->Add(itemStaticText30, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText31 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         software.compile_date_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer29->Add(itemStaticText31, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer32 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer32, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText33 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         _("Boost version:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer32->Add(itemStaticText33, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText34 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         software.boost_version_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer32->Add(itemStaticText34, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer35, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText36 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         _("OpenSSL version:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer35->Add(itemStaticText36, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText37 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         software.openssl_version_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer35->Add(itemStaticText37, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer38, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText39 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         _("OS build:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer38->Add(itemStaticText39, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText40 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
         software.build_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer38->Add(itemStaticText40, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticBox* itemStaticBoxSizer41Static = new wxStaticBox(p_GWallet->panel, wxID_ANY,
         _("Market Prices"));
   wxStaticBoxSizer* itemStaticBoxSizer41 = new wxStaticBoxSizer(itemStaticBoxSizer41Static, wxVERTICAL);
   itemGridSizer2->Add(itemStaticBoxSizer41, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer42 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer41->Add(itemBoxSizer42, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText43 = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC,
         wxT("USD/BTS:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer42->Add(itemStaticText43, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   market.usd_bts = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC, market.usd_bts_value,
         wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer42->Add(market.usd_bts, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer41->Add(itemBoxSizer45, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText46 = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC,
         wxT("CNY/BTS:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer45->Add(itemStaticText46, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   market.cny_bts = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC, market.cny_bts_value,
         wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer45->Add(market.cny_bts, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer48 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer41->Add(itemBoxSizer48, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText49 = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC,
         wxT("EUR/BTS:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer48->Add(itemStaticText49, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   market.eur_bts = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC, market.eur_bts_value,
         wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer48->Add(market.eur_bts, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer41->Add(itemBoxSizer51, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText52 = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC,
         wxT("GOLD/BTS:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer51->Add(itemStaticText52, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   market.gold_bts = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC, market.gold_bts_value,
         wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer51->Add(market.gold_bts, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer54 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer41->Add(itemBoxSizer54, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText55 = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC,
         wxT("SILVER/BTS:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer54->Add(itemStaticText55, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   market.silver_bts = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC, market.silver_bts_value,
         wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer54->Add(market.silver_bts, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer41->Add(itemBoxSizer57, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText58 = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC,
         wxT("BTC/BTS:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer57->Add(itemStaticText58, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   market.btc_bts = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC, market.btc_bts_value,
         wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer57->Add(market.btc_bts, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   p_GWallet->sizers.home->Add(itemGridSizer2, 0, wxALL, 5);

   p_GWallet->sizers.main->Layout();
}

void Home::CreateEvents()
{
   Connect(ID_TIMER_SLOW, wxEVT_TIMER, wxTimerEventHandler(Home::OnTimerSlow), NULL, this);
   Connect(ID_TIMER_FAST, wxEVT_TIMER, wxTimerEventHandler(Home::OnTimerFast), NULL, this);

   p_GWallet->panel->Connect(ID_VIEW_WITNESSES, wxEVT_HYPERLINK, wxHyperlinkEventHandler(Home::OnWitness), NULL, this);
   p_GWallet->panel->Connect(ID_VIEW_COMMITTEE, wxEVT_HYPERLINK, wxHyperlinkEventHandler(Home::OnCommittee), NULL, this);
}

void Home::DoInitialData()
{
   const auto info = p_GWallet->bitshares.wallet_api_ptr->info();

   blockchain.head_block_number_value = fc::json::to_string(info["head_block_num"]);

   const auto head_block_age_string = fc::json::to_string(info["head_block_age"]);
   blockchain.head_block_age_value = head_block_age_string.substr(1, head_block_age_string.length()-2);

   auto next_maintenance_time_string = fc::json::to_string(info["next_maintenance_time"]);
   blockchain.next_maintenance_time_value = next_maintenance_time_string.substr(1,
         next_maintenance_time_string.length()-2);

   const auto participation_string = fc::json::to_string(info["participation"]);
   blockchain.participation_value = participation_string.substr(1, participation_string.length()-2);

   const auto chain_id_string = fc::json::to_string(info["chain_id"]);
   blockchain.chain_id_value = chain_id_string.substr(chain_id_string.length()-11, 10);

   const auto about = p_GWallet->bitshares.wallet_api_ptr->about();

   const auto client_version_string = fc::json::to_string(about["client_version"]);
   software.client_version_value = client_version_string.substr(1, client_version_string.length()-2);

   const auto graphene_revision_string = fc::json::to_string(about["graphene_revision"]);
   software.graphene_revision_value = graphene_revision_string.substr(1, graphene_revision_string.length()-2);

   const auto graphene_revision_age_string = fc::json::to_string(about["graphene_revision_age"]);
   software.graphene_revision_age_value = graphene_revision_age_string.substr(1,
         graphene_revision_age_string.length()-2);

   const auto fc_revision_string = fc::json::to_string(about["fc_revision"]);
   software.fc_revision_value = fc_revision_string.substr(1, fc_revision_string.length()-2);

   const auto fc_revision_age_string = fc::json::to_string(about["fc_revision_age"]);
   software.fc_revision_age_value = fc_revision_age_string.substr(1, fc_revision_age_string.length()-2);

   const auto compile_date_string = fc::json::to_string(about["compile_date"]);
   software.compile_date_value = compile_date_string.substr(1, compile_date_string.length()-2);

   const auto boost_version_string = fc::json::to_string(about["boost_version"]);
   software.boost_version_value = boost_version_string.substr(1, boost_version_string.length()-2);

   const auto openssl_version_string = fc::json::to_string(about["openssl_version"]);
   software.openssl_version_value = openssl_version_string.substr(1, openssl_version_string.length()-2);

   const auto build_string = fc::json::to_string(about["graphene_revision"]);
   software.build_value = build_string.substr(1, build_string.length()-2);

   if(!p_GWallet->state.is_new) {
      const auto account_object = p_GWallet->bitshares.wallet_api_ptr->get_account(
            p_GWallet->strings.selected_account.ToStdString());
      account.name_value = account_object.name;

      const auto account_id_string = fc::json::to_string(account_object.id);
      account.id_value = account_id_string.substr(1, account_id_string.length() - 2);

      const auto referrer_name = p_GWallet->bitshares.wallet_api_ptr->get_object(
            account_object.referrer).get_array()[0]["name"];
      const auto referrer_string = fc::json::to_string(referrer_name);
      account.referrer_value = referrer_string.substr(1, referrer_string.length() - 2);

      const auto registrar_name = p_GWallet->bitshares.wallet_api_ptr->get_object(
            account_object.registrar).get_array()[0]["name"];
      const auto registrar_string = fc::json::to_string(registrar_name);
      account.registrar_value = registrar_string.substr(1, registrar_string.length() - 2);

      auto voting_as_name = p_GWallet->bitshares.wallet_api_ptr->get_object(
            account_object.options.voting_account).get_array()[0]["name"];
      const auto voting_as_string = fc::json::to_string(voting_as_name);
      account.voting_as_value = voting_as_string.substr(1, voting_as_string.length() - 2);

      const auto stats = p_GWallet->bitshares.wallet_api_ptr->get_object(account_object.statistics).get_array()[0];

      account.total_ops_value = fc::json::to_string(stats["total_ops"]);
   }
   else {
      account.name_value = "MY ACCOUNT";
      account.id_value = "MY ID";
      account.referrer_value = "MY REFERRER";
      account.registrar_value = "MY REGISTRAR";
      account.voting_as_value = "SELF";
      account.total_ops_value = "0";
   }

   market.usd_bts_value = p_GWallet->bitshares.database_api->get_ticker("USD", "BTS").latest;
   market.cny_bts_value = p_GWallet->bitshares.database_api->get_ticker("CNY", "BTS").latest;
   market.eur_bts_value = p_GWallet->bitshares.database_api->get_ticker("EUR", "BTS").latest;
   market.gold_bts_value = p_GWallet->bitshares.database_api->get_ticker("GOLD", "BTS").latest;
   market.silver_bts_value = p_GWallet->bitshares.database_api->get_ticker("SILVER", "BTS").latest;
   market.btc_bts_value = p_GWallet->bitshares.database_api->get_ticker("BTC", "BTS").latest;
}

void Home::DoTimers()
{
   /* Todo: Timers freeze whole app giving a bad UX, uncomment when solution is found(probably wxthreads) */
   /*
   m_timer_fast.SetOwner(this, ID_TIMER_FAST);
   m_timer_fast.Start(3000);

   m_timer_slow.SetOwner(this, ID_TIMER_SLOW);
   m_timer_slow.Start(60000);
   */
}

void Home::OnTimerSlow(wxTimerEvent& event)
{
   wdump(("timer slow!"));
   const auto info = p_GWallet->bitshares.wallet_api_ptr->info();

   const auto next_maintenance_time_string = fc::json::to_string(info["next_maintenance_time"]);
   const auto participation_string = fc::json::to_string(info["participation"]);

   blockchain.next_maintenance_time->SetLabel(
         next_maintenance_time_string.substr(1,next_maintenance_time_string.length()-2));
   blockchain.participation->SetLabel(participation_string.substr(1,participation_string.length()-2));

   const auto usd_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("USD", "BTS");
   const auto cny_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("CNY", "BTS");
   const auto eur_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("EUR", "BTS");
   const auto gold_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("GOLD", "BTS");
   const auto silver_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("SILVER", "BTS");
   const auto btc_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("BTC", "BTS");

   market.usd_bts->SetLabel(usd_bts_ticker.latest);
   market.cny_bts->SetLabel(cny_bts_ticker.latest);
   market.eur_bts->SetLabel(eur_bts_ticker.latest);
   market.gold_bts->SetLabel(gold_bts_ticker.latest);
   market.silver_bts->SetLabel(silver_bts_ticker.latest);
   market.btc_bts->SetLabel(btc_bts_ticker.latest);
}

void Home::OnTimerFast(wxTimerEvent& event)
{
   wdump(("timer fast!"));
   const auto info = p_GWallet->bitshares.wallet_api_ptr->info();

   const auto head_block_num_string = fc::json::to_string(info["head_block_num"]);
   const auto head_block_age_string = fc::json::to_string(info["head_block_age"]);

   blockchain.head_block_num->SetLabel(head_block_num_string);
   blockchain.head_block_age->SetLabel(head_block_age_string.substr(1,head_block_age_string.length()-2));
}

void Home::DoAccount()
{
   const auto info = p_GWallet->bitshares.wallet_api_ptr->info();

   const auto head_block_num_string = fc::json::to_string(info["head_block_num"]);
   const auto head_block_age_string = fc::json::to_string(info["head_block_age"]);

   blockchain.head_block_num->SetLabel(head_block_num_string);
   blockchain.head_block_age->SetLabel(head_block_age_string.substr(1,head_block_age_string.length()-2));

   const auto account_object = p_GWallet->bitshares.wallet_api_ptr->get_account(p_GWallet->strings.selected_account.ToStdString());
   const auto account_id_string = fc::json::to_string(account_object.id);
   account.id_value = account_id_string.substr(1, account_id_string.length() - 2);

   const auto referrer_name = p_GWallet->bitshares.wallet_api_ptr->get_object(account_object.referrer).get_array()[0]["name"];
   const auto referrer_string = fc::json::to_string(referrer_name);
   account.referrer_value = referrer_string.substr(1, referrer_string.length() - 2);

   const auto registrar_name = p_GWallet->bitshares.wallet_api_ptr->get_object(account_object.registrar).get_array()[0]["name"];
   const auto registrar_string = fc::json::to_string(registrar_name);
   account.registrar_value = registrar_string.substr(1, registrar_string.length() - 2);

   auto voting_as_name = p_GWallet->bitshares.wallet_api_ptr->get_object(account_object.options.voting_account).get_array()[0]["name"];
   const auto voting_as_string = fc::json::to_string(voting_as_name);
   account.voting_as_value = voting_as_string.substr(1, voting_as_string.length() - 2);

   const auto stats = p_GWallet->bitshares.wallet_api_ptr->get_object(account_object.statistics).get_array()[0];

   account.total_ops_value = fc::json::to_string(stats["total_ops"]);

   account.name->SetLabel(account_object.name);
   account.id->SetLabel(account.id_value);
   account.referrer->SetLabel(account.referrer_value);
   account.registrar->SetLabel(account.registrar_value);
   account.voting_as->SetLabel(account.voting_as_value);
   account.total_ops->SetLabel(account.total_ops_value);
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
