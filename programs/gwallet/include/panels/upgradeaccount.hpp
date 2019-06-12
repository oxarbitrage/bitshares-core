#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/treectrl.h>
//#include <wx/numformatter.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class UpgradeAccount: public wxPanel
{
public:
   UpgradeAccount(GWallet* gwallet);

protected:
   wxComboBox* name;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("UpgradeAccount"), wxT("wxPanel"));
      name = XRCCTRL(*this,"name",wxComboBox);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class UpgradeAccountResponse: public wxPanel
{
public:
   UpgradeAccountResponse(GWallet* gwallet, wxAny any_response);
   wxTreeCtrl* response_tree;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("UpgradeAccountResponse"), wxT("wxPanel"));
      response_tree = XRCCTRL(*this,"response_tree",wxTreeCtrl);
   }
};