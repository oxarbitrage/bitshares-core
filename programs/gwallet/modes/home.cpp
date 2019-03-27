#include "../include/modes/home.hpp"

#include <wx/hyperlink.h>

Home::Home(GWallet* gwallet) : wxFrame()
{
   p_GWallet = gwallet;
}

void Home::CreateControls()
{
   DoTimers();
   DoInitialData();

   p_GWallet->sizerHomeMode = new wxBoxSizer(wxVERTICAL);
   p_GWallet->mainSizer->Add(p_GWallet->sizerHomeMode, 0, wxGROW|wxALL);

   wxGridSizer* itemGridSizer2 = new wxGridSizer(0, 2, 0, 0);

   wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(p_GWallet->panel, wxID_ANY, _("Blockchain Information"));
   wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
   itemGridSizer2->Add(itemStaticBoxSizer3, 6, wxGROW|wxALIGN_TOP|wxALL, 5);

   wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText5 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           _("Head block number:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer4->Add(itemStaticText5, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   head_block_num = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           head_block_number_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer4->Add(head_block_num, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer7, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText8 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           _("Block age:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer7->Add(itemStaticText8, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   head_block_age = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           head_block_age_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer7->Add(head_block_age, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer1, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText2 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           _("Next maintenance time:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer1->Add(itemStaticText2, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   next_maintenance_time = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           next_maintenance_time_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer1->Add(next_maintenance_time, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer3->Add(itemBoxSizer5, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText7 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           _("Participation:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer5->Add(itemStaticText7, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   participation = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC,
           participation_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer5->Add(participation, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

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

   wxStaticBox* itemStaticBoxSizer1Static = new wxStaticBox(p_GWallet->panel, wxID_ANY, _("Account Information"));
   wxStaticBoxSizer* itemStaticBoxSizer1 = new wxStaticBoxSizer(itemStaticBoxSizer1Static, wxVERTICAL);
   itemGridSizer2->Add(itemStaticBoxSizer1, 6, wxGROW|wxALIGN_TOP|wxALL, 5);

   wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer2, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText4 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
           _("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer2->Add(itemStaticText4, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   account_name = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
           account_name_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer2->Add(account_name, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer12, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText13 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
           _("ID:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer12->Add(itemStaticText13, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   account_id = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC,
           account_id_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer12->Add(account_id, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticBox* itemStaticBoxSizer2Static = new wxStaticBox(p_GWallet->panel, wxID_ANY, _("Software Information"));
   wxStaticBoxSizer* itemStaticBoxSizer2 = new wxStaticBoxSizer(itemStaticBoxSizer2Static, wxVERTICAL);
   itemGridSizer2->Add(itemStaticBoxSizer2, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText16 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           _("Client version:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer3->Add(itemStaticText16, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText17 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           client_version_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer3->Add(itemStaticText17, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer6, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText18 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           _("Graphene revision:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer6->Add(itemStaticText18, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText19 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           graphene_revision_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer6->Add(itemStaticText19, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer20, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText21 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           _("Revision age:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer20->Add(itemStaticText21, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText22 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           graphene_revision_age_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer20->Add(itemStaticText22, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer23, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText24 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           _("FC revision:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer23->Add(itemStaticText24, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText25 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           fc_revision_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer23->Add(itemStaticText25, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer26, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText27 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           _("FC revision age:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer26->Add(itemStaticText27, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText28 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           fc_revision_age_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer26->Add(itemStaticText28, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer29 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer29, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText30 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           _("Compile date:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer29->Add(itemStaticText30, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText31 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           compile_date_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer29->Add(itemStaticText31, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer32 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer32, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText33 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           _("Boost version:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer32->Add(itemStaticText33, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText34 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           boost_version_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer32->Add(itemStaticText34, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer35, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText36 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           _("OpenSSL version:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer35->Add(itemStaticText36, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText37 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           openssl_version_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer35->Add(itemStaticText37, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer2->Add(itemBoxSizer38, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText39 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           _("OS build:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer38->Add(itemStaticText39, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticText* itemStaticText40 = new wxStaticText( itemStaticBoxSizer2->GetStaticBox(), wxID_STATIC,
           build_value, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer38->Add(itemStaticText40, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxStaticBox* itemStaticBoxSizer41Static = new wxStaticBox(p_GWallet->panel, wxID_ANY,
           _("Market Prices"));
   wxStaticBoxSizer* itemStaticBoxSizer41 = new wxStaticBoxSizer(itemStaticBoxSizer41Static, wxVERTICAL);
   itemGridSizer2->Add(itemStaticBoxSizer41, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer42 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer41->Add(itemBoxSizer42, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText43 = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC,
           _("USD/BTS:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer42->Add(itemStaticText43, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   usd_bts = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC, usd_bts_ticker,
           wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer42->Add(usd_bts, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer41->Add(itemBoxSizer45, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText46 = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC,
           _("CNY/BTS:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer45->Add(itemStaticText46, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   cny_bts = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC, cny_bts_ticker,
           wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer45->Add(cny_bts, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer48 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer41->Add(itemBoxSizer48, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText49 = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC,
           _("EUR/BTS:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer48->Add(itemStaticText49, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   eur_bts = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC, eur_bts_ticker,
           wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer48->Add(eur_bts, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer41->Add(itemBoxSizer51, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText52 = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC,
           _("GOLD/BTS:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer51->Add(itemStaticText52, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   gold_bts = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC, gold_bts_ticker,
           wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer51->Add(gold_bts, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer54 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer41->Add(itemBoxSizer54, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText55 = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC,
           _("SILVER/BTS:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer54->Add(itemStaticText55, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   silver_bts = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC, silver_bts_ticker,
           wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer54->Add(silver_bts, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer41->Add(itemBoxSizer57, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText58 = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC,
           _("BTC/BTS:"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer57->Add(itemStaticText58, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   btc_bts = new wxStaticText( itemStaticBoxSizer41->GetStaticBox(), wxID_STATIC, btc_bts_ticker,
           wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer57->Add(btc_bts, 6, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   p_GWallet->sizerHomeMode->Add(itemGridSizer2, 0, wxALL, 5);

   p_GWallet->mainSizer->Layout();
}

void Home::CreateEvents()
{
   Connect(ID_TIMER_SLOW, wxEVT_TIMER, wxTimerEventHandler(Home::OnTimerSlow), NULL, this);
   Connect(ID_TIMER_FAST, wxEVT_TIMER, wxTimerEventHandler(Home::OnTimerFast), NULL, this);
}

void Home::DoInitialData()
{
   auto info = p_GWallet->bitshares.wallet_api_ptr->info();

   head_block_number_value = fc::json::to_string(info["head_block_num"]);

   auto head_block_age_string = fc::json::to_string(info["head_block_age"]);
   head_block_age_value = head_block_age_string.substr(1, head_block_age_string.length()-2);

   auto next_maintenance_time_string = fc::json::to_string(info["next_maintenance_time"]);
   next_maintenance_time_value = next_maintenance_time_string.substr(1, next_maintenance_time_string.length()-2);

   auto participation_string = fc::json::to_string(info["participation"]);
   participation_value = participation_string.substr(1, participation_string.length()-2);

   auto about = p_GWallet->bitshares.wallet_api_ptr->about();

   auto client_version_string = fc::json::to_string(about["client_version"]);
   client_version_value = client_version_string.substr(1, client_version_string.length()-2);

   auto graphene_revision_string = fc::json::to_string(about["graphene_revision"]);
   graphene_revision_value = graphene_revision_string.substr(1, graphene_revision_string.length()-2);

   auto graphene_revision_age_string = fc::json::to_string(about["graphene_revision_age"]);
   graphene_revision_age_value = graphene_revision_age_string.substr(1, graphene_revision_age_string.length()-2);

   auto fc_revision_string = fc::json::to_string(about["fc_revision"]);
   fc_revision_value = fc_revision_string.substr(1, fc_revision_string.length()-2);

   auto fc_revision_age_string = fc::json::to_string(about["fc_revision_age"]);
   fc_revision_age_value = fc_revision_age_string.substr(1, fc_revision_age_string.length()-2);

   auto compile_date_string = fc::json::to_string(about["compile_date"]);
   compile_date_value = compile_date_string.substr(1, compile_date_string.length()-2);

   auto boost_version_string = fc::json::to_string(about["boost_version"]);
   boost_version_value = boost_version_string.substr(1, boost_version_string.length()-2);

   auto openssl_version_string = fc::json::to_string(about["openssl_version"]);
   openssl_version_value = openssl_version_string.substr(1, openssl_version_string.length()-2);

   auto build_string = fc::json::to_string(about["graphene_revision"]);
   build_value = build_string.substr(1, build_string.length()-2);

   auto account = p_GWallet->bitshares.wallet_api_ptr->get_account(p_GWallet->first_account_name);

   account_name_value = account.name;

   auto account_id_string = fc::json::to_string(account.id);
   account_id_value = account_id_string.substr(1, account_id_string.length()-2);

   usd_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("USD", "BTS").latest;
   cny_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("CNY", "BTS").latest;
   eur_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("EUR", "BTS").latest;
   gold_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("GOLD", "BTS").latest;
   silver_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("SILVER", "BTS").latest;
   btc_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("BTC", "BTS").latest;
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

void Home::OnTimerSlow(wxTimerEvent & event)
{
   wdump(("timer slow!"));
   auto info = p_GWallet->bitshares.wallet_api_ptr->info();

   auto next_maintenance_time_string = fc::json::to_string(info["next_maintenance_time"]);
   auto participation_string = fc::json::to_string(info["participation"]);

   next_maintenance_time->SetLabel(next_maintenance_time_string.substr(1,next_maintenance_time_string.length()-2));
   participation->SetLabel(participation_string.substr(1,participation_string.length()-2));

   auto usd_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("USD", "BTS");
   auto cny_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("CNY", "BTS");
   auto eur_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("EUR", "BTS");
   auto gold_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("GOLD", "BTS");
   auto silver_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("SILVER", "BTS");
   auto btc_bts_ticker = p_GWallet->bitshares.database_api->get_ticker("BTC", "BTS");

   usd_bts->SetLabel(usd_bts_ticker.latest);
   cny_bts->SetLabel(cny_bts_ticker.latest);
   eur_bts->SetLabel(eur_bts_ticker.latest);
   gold_bts->SetLabel(gold_bts_ticker.latest);
   silver_bts->SetLabel(silver_bts_ticker.latest);
   btc_bts->SetLabel(btc_bts_ticker.latest);
}

void Home::OnTimerFast(wxTimerEvent & event)
{
   wdump(("timer fast!"));
   auto info = p_GWallet->bitshares.wallet_api_ptr->info();

   auto head_block_num_string = fc::json::to_string(info["head_block_num"]);
   auto head_block_age_string = fc::json::to_string(info["head_block_age"]);

   head_block_num->SetLabel(head_block_num_string);
   head_block_age->SetLabel(head_block_age_string.substr(1,head_block_age_string.length()-2));
}
