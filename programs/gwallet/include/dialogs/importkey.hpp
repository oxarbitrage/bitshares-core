#include <wx/wx.h>

class ImportKeyDialog: public wxDialog
{
public:

    ImportKeyDialog ( wxWindow * parent, wxWindowID id, const wxString & title,
                      const wxPoint & pos = wxDefaultPosition,
                      const wxSize & size = wxDefaultSize,
                      long style = wxDEFAULT_DIALOG_STYLE );

    wxTextCtrl *account;
    wxTextCtrl *key;
private:

    void OnOk( wxCommandEvent & event );
    void OnCancel( wxCommandEvent & event );
};

const int ID_IMPORTKEY_OK = 107;
const int ID_IMPORTKEY_CANCEL = 108;
