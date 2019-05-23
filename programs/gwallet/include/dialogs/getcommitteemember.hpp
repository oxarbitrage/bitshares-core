#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>

#include "../gwallet.hpp"

class GetCommitteeMemberDialog: public wxDialog
{
public:
   GetCommitteeMemberDialog(wxWindow *parent);

protected:
   wxSearchCtrl* owner_account;
   wxPanel* response_panel;
   wxTreeCtrl* response_tree;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetCommitteeMemberDialog"), wxT("wxDialog"));
      owner_account = XRCCTRL(*this,"owner_account",wxSearchCtrl);
      response_panel = XRCCTRL(*this,"response_panel",wxPanel);
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};