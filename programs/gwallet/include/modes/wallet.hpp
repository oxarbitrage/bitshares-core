#include <wx/wx.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class Wallet : public wxFrame
{
public:

    Wallet(GWallet* gwallet);
    void CreateControls();
    void CreateEvents();

    void OnTransfer(wxCommandEvent& event);
    void OnSellAsset(wxCommandEvent& event);
    void OnBorrowAsset(wxCommandEvent& event);
    void OnCancelOrder(wxCommandEvent& event);
    void OnSetProxy(wxCommandEvent& event);

    GWallet* p_GWallet;

private:
};

const int ID_WALLET_TRANSFER = 1;
const int ID_WALLET_SELLASSET = 2;
const int ID_WALLET_BORROWASSET = 3;
const int ID_WALLET_CANCELORDER = 4;
const int ID_WALLET_SETPROXY = 5;
