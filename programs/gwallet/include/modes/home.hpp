#include <wx/wx.h>
#include <wx/hyperlink.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

struct Blockchain
{
   wxStaticText* head_block_num;
   wxStaticText* head_block_age;
   wxStaticText* next_maintenance_time;
   wxStaticText* participation;

   std::string head_block_number_value;
   std::string head_block_age_value;
   std::string next_maintenance_time_value;
   std::string participation_value;
   std::string chain_id_value;
};
struct Account
{
   wxStaticText* name;
   wxStaticText* id;
   wxStaticText* referrer;
   wxStaticText* registrar;
   wxStaticText* voting_as;
   wxStaticText* total_ops;

   std::string name_value;
   std::string id_value;
   std::string referrer_value;
   std::string registrar_value;
   std::string voting_as_value;
   std::string total_ops_value;
};

struct Software
{
   std::string client_version_value;
   std::string graphene_revision_value;
   std::string graphene_revision_age_value;
   std::string fc_revision_value;
   std::string fc_revision_age_value;
   std::string compile_date_value;
   std::string boost_version_value;
   std::string openssl_version_value;
   std::string build_value;
};

struct Market
{
   wxStaticText* usd_bts;
   wxStaticText* cny_bts;
   wxStaticText* eur_bts;
   wxStaticText* gold_bts;
   wxStaticText* silver_bts;
   wxStaticText* btc_bts;

   std::string usd_bts_value;
   std::string cny_bts_value;
   std::string eur_bts_value;
   std::string gold_bts_value;
   std::string silver_bts_value;
   std::string btc_bts_value;
};

class Home : public wxFrame
{
public:
   Home(GWallet* gwallet);
   void CreateControls();
   void CreateEvents();
   void DoInitialData();
   void DoAccount();

   GWallet* p_GWallet;

private:
   void OnTimerSlow(wxTimerEvent& event);
   void OnTimerFast(wxTimerEvent& event);

   void OnWitness(wxHyperlinkEvent& event);
   void OnCommittee(wxHyperlinkEvent& event);

   void DoTimers();


   Blockchain blockchain;
   Account account;
   Software software;
   Market market;

   wxTimer m_timer_fast;
   wxTimer m_timer_slow;
};

const int ID_TIMER_SLOW = 991;
const int ID_TIMER_FAST = 992;

const int ID_VIEW_WITNESSES = 993;
const int ID_VIEW_COMMITTEE = 994;
