#include <wx/wx.h>
#include <wx/srchctrl.h>

#include "../gwallet.hpp"

class BorrowAssetDialog: public wxDialog
{
public:
   BorrowAssetDialog(wxWindow *parent);

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

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("BorrowAsset"), wxT("wxDialog"));
      seller = XRCCTRL(*this,"seller",wxComboBox);
      borrow_amount = XRCCTRL(*this,"borrow_amount",wxTextCtrl);
      borrow_asset = XRCCTRL(*this,"borrow_asset",wxSearchCtrl);
      collateral_amount = XRCCTRL(*this,"collateral_amount",wxTextCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};
