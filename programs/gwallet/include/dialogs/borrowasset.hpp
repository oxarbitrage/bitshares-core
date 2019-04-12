#include <wx/wx.h>
#include <wx/srchctrl.h>

#include "../gwallet.hpp"

class BorrowAssetDialog: public wxDialog
{
public:
   BorrowAssetDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition,
         const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

private:
   GWallet* p_GWallet;

   wxComboBox* seller;
   wxTextCtrl* borrow_amount;
   wxSearchCtrl* borrow_asset;
   wxTextCtrl* collateral_amount;

   wxCheckBox* broadcast;
   wxCheckBox* cli;

   void CreateEvents();
   void OnOk(wxCommandEvent& event);
};
