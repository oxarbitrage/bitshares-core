#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
#include <wx/numformatter.h>
#include <wx/xrc/xmlres.h>

//#include "../gwallet.hpp"
class GWallet;

class GetCommitteeMember: public wxPanel
{
public:
   GetCommitteeMember(GWallet* gwallet);

protected:
   wxSearchCtrl* owner_account;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetCommitteeMember"), wxT("wxPanel"));
      owner_account = XRCCTRL(*this,"owner_account",wxSearchCtrl);
   }
};

class GetCommitteeMemberResponse: public wxPanel
{
public:
   GetCommitteeMemberResponse(GWallet* gwallet);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetCommitteeMemberResponse"), wxT("wxPanel"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};