#include <wx/wx.h>
#include <wx/srchctrl.h>

#include "../gwallet.hpp"

class SetProxy: public wxPanel
{
public:
   SetProxy(GWallet* gwallet);

protected:
   wxComboBox* account_to_modify;
   wxSearchCtrl* voting_account;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   wxString current_voting_account;

   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent& event);
   void DoVotingAccount();

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("SetProxy"), wxT("wxPanel"));
      account_to_modify = XRCCTRL(*this,"account_to_modify",wxComboBox);
      voting_account = XRCCTRL(*this,"voting_account",wxSearchCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};