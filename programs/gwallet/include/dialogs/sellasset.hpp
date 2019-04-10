#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/timectrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>

#include "../gwallet.hpp"

class SellAssetDialog: public wxDialog
{
public:
   SellAssetDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition,
         const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

private:
   GWallet* p_GWallet;

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

   void CreateEvents();
   void OnOk(wxCommandEvent& event);
};

//const int ID_DIALOG_TRANSFER_OK = 11100;
//const int ID_DIALOG_TRANSFER_CANCEL = 11101;

//const int ID_DIALOG_TRANSFER_BROADCAST = 11102;
//const int ID_DIALOG_TRANSFER_CLI = 11103;