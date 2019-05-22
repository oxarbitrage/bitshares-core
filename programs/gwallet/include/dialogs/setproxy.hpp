#include <wx/wx.h>
#include <wx/srchctrl.h>

#include "../gwallet.hpp"

class SetProxyDialog: public wxDialog
{
public:
   SetProxyDialog(wxWindow* parent);

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
   //void OnChangeAccount(wxCommandEvent& event);
   void DoVotingAccount();

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("SetProxyDialog"), wxT("wxDialog"));
      account_to_modify = XRCCTRL(*this,"account_to_modify",wxComboBox);
      voting_account = XRCCTRL(*this,"voting_account",wxSearchCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }


};