#include <wx/wx.h>

#include "../gwallet.hpp"

class TransferDialog : public wxDialog {
protected:
   GWallet* p_GWallet;

   wxComboBox* from;
   wxTextCtrl* to;
   wxTextCtrl* amount;
   wxComboBox* asset;
   wxTextCtrl* memo;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("TransferDialog"), wxT("wxDialog"));
      from = XRCCTRL(*this,"from",wxComboBox);
      to = XRCCTRL(*this,"to",wxTextCtrl);
      amount = XRCCTRL(*this,"amount",wxTextCtrl);
      asset = XRCCTRL(*this,"asset",wxComboBox);
      memo = XRCCTRL(*this,"memo",wxTextCtrl);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
   void OnOk(wxCommandEvent& event);
public:
   TransferDialog(wxWindow *parent=NULL);
};
