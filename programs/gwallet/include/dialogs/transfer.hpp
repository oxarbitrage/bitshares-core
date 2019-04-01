#include <wx/wx.h>

#include "../gwallet.hpp"

class TransferDialog: public wxDialog
{
public:
   TransferDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition,
         const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

private:
   GWallet* p_GWallet;

   wxComboBox* from;
   wxTextCtrl* to;
   wxTextCtrl* amount;
   wxComboBox* asset;
   wxTextCtrl* memo;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

   void CreateEvents();
   void OnOk(wxCommandEvent& event);
};

const int ID_DIALOG_TRANSFER_OK = 11100;
const int ID_DIALOG_TRANSFER_CANCEL = 11101;

const int ID_DIALOG_TRANSFER_BROADCAST = 11102;
const int ID_DIALOG_TRANSFER_CLI = 11103;
