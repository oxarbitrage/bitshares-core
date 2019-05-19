#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/timectrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>

#include "../gwallet.hpp"

class SellAssetDialog: public wxDialog
{
public:
   SellAssetDialog(wxWindow *parent=NULL);

protected:
   wxComboBox* seller;
   wxTextCtrl* sell_amount;
   wxComboBox* sell_asset;
   wxTextCtrl* receive_amount;
   wxSearchCtrl* receive_asset;
   wxDatePickerCtrl* date;
   wxTimePickerCtrl* time;
   wxCheckBox* fill_or_kill;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("SellAssetDialog"), wxT("wxDialog"));
      seller = XRCCTRL(*this,"seller",wxComboBox);
      sell_amount = XRCCTRL(*this,"sell_amount",wxTextCtrl);
      sell_asset = XRCCTRL(*this,"sell_asset",wxComboBox);
      receive_amount = XRCCTRL(*this,"receive_amount",wxTextCtrl);
      receive_asset = XRCCTRL(*this,"receive_asset",wxSearchCtrl);
      date = XRCCTRL(*this,"date",wxDatePickerCtrl);
      time = XRCCTRL(*this,"time",wxTimePickerCtrl);
      fill_or_kill = XRCCTRL(*this,"fill_or_kill",wxCheckBox);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

