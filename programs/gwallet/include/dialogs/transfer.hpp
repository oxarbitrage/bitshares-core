#include <wx/wx.h>

class TransferDialog: public wxDialog
{
public:
   TransferDialog(wxWindow * parent, wxWindowID id, const wxString & title, const wxPoint & pos = wxDefaultPosition,
         const wxSize & size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

    //wxTextCtrl *account;
    //wxTextCtrl *key;
private:
   void OnOk( wxCommandEvent & event );
   void OnCancel( wxCommandEvent & event );
};

const int ID_DIALOG_TRANSFER_OK = 11100;
const int ID_DIALOG_TRANSFER_CANCEL = 11101;

const int ID_DIALOG_TRANSFER_BROADCAST = 11102;
const int ID_DIALOG_TRANSFER_CLI = 11103;
