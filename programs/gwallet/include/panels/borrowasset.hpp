#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/statline.h>
#include <wx/combo.h>
#include <wx/xrc/xmlres.h>

//#include "../gwallet.hpp"
class GWallet;

class BorrowAsset: public wxPanel
{
public:
   BorrowAsset(GWallet* gwallet);

protected:
   wxComboBox* seller;
   wxTextCtrl* borrow_amount;
   wxSearchCtrl* borrow_asset;
   wxTextCtrl* collateral_amount;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchAsset(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("BorrowAsset"), wxT("wxPanel"));
      seller = XRCCTRL(*this,"seller",wxComboBox);
      borrow_amount = XRCCTRL(*this,"borrow_amount",wxTextCtrl);
      borrow_asset = XRCCTRL(*this,"borrow_asset",wxSearchCtrl);
      collateral_amount = XRCCTRL(*this,"collateral_amount",wxTextCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};
