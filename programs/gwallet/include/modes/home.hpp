#include <wx/wx.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class Home : public wxFrame
{
public:
   Home(GWallet* gwallet);
   void CreateControls();
   void CreateEvents();

private:
   void OnTimerSlow(wxTimerEvent& event);
   void OnTimerFast(wxTimerEvent& event);

   void DoTimers();
   void DoInitialData();

   GWallet* p_GWallet;

   wxStaticText* head_block_num, *head_block_age, *next_maintenance_time, *participation;
   wxStaticText* account_name, *account_id;
   wxStaticText* usd_bts, *cny_bts, *eur_bts, *gold_bts, *silver_bts, *btc_bts;

   std::string head_block_number_value, head_block_age_value, next_maintenance_time_value, participation_value;
   std::string client_version_value, graphene_revision_value, graphene_revision_age_value;
   std::string fc_revision_value, fc_revision_age_value, compile_date_value, boost_version_value;
   std::string openssl_version_value, build_value;
   std::string account_name_value, account_id_value;
   std::string usd_bts_ticker, cny_bts_ticker, eur_bts_ticker, gold_bts_ticker, silver_bts_ticker, btc_bts_ticker;

   wxTimer m_timer_fast;
   wxTimer m_timer_slow;
};

const int ID_TIMER_SLOW = 991;
const int ID_TIMER_FAST = 992;
