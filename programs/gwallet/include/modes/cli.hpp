#include <wx/wx.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class Cli : public wxFrame
{
public:

    Cli(GWallet* gwallet);
    void CreateControls();
    void CreateEvents();

    void OnCliCommand(wxCommandEvent& event);
    void OnCliClear(wxCommandEvent& event);

    GWallet* p_GWallet;
    wxTextCtrl* sendCliText;
    wxButton* clearCliButton;
    wxTextCtrl* itemTextCLI;

private:
};

const int ID_CLI = 117;
const int ID_CLITEXT = 118;
const int ID_CLISEND = 119;
const int ID_CLICLEAR = 120;