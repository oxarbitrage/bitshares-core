#include <wx/wx.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class SendReceive : public wxFrame
{
public:
   SendReceive(GWallet* gwallet);
   void CreateControls();
   void CreateEvents();

   void OnTransferOk(wxCommandEvent &event);

   GWallet* p_GWallet;

   wxComboBox* from;
   wxTextCtrl* to;
   wxComboBox* asset;
   wxTextCtrl* amount;

private:
};

const int ID_TRANSFER_OK = 4221;