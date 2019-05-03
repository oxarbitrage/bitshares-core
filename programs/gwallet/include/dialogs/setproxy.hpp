#include <wx/wx.h>
#include <wx/srchctrl.h>

#include "../gwallet.hpp"

class SetProxyDialog: public wxDialog
{
public:
   SetProxyDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

private:
   GWallet* p_GWallet;

   wxComboBox* account_to_modify;
   wxSearchCtrl* voting_account;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

   wxString current_voting_account;

   void CreateEvents();
   void OnOk(wxCommandEvent& event);
   //void OnChangeAccount(wxCommandEvent& event);
   void DoVotingAccount();

};