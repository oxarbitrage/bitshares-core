#include <wx/wx.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class History : public wxFrame
{
public:

    History(GWallet* gwallet);
    void CreateControls();
    void CreateEvents();
    void DoHistory(std::string account);

    GWallet* p_GWallet;
    wxGrid* grid;

private:
};
