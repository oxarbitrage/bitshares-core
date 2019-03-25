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

    void OnTimerSlow(wxTimerEvent& event);
    void OnTimerFast(wxTimerEvent& event);

    GWallet* p_GWallet;

    wxStaticText* head_block_num, *head_block_age, *next_maintenance_time, *participation;
    wxStaticText* account_name, *account_id;
    wxStaticText* usd_bts, *cny_bts, *eur_bts, *gold_bts, *silver_bts, *btc_bts;
private:
    wxTimer m_timer_fast;
    wxTimer m_timer_slow;

};

const int ID_TIMER_SLOW = 991;
const int ID_TIMER_FAST = 992;
