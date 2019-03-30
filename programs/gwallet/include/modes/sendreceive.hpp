#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/combo.h>

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
   void OnSearchAccount(wxCommandEvent &event);
   void OnSearchAsset(wxCommandEvent &event);

   void OnSendUrl(wxCommandEvent &event);
   void OnReceiveUrl(wxCommandEvent &event);

   bool ValidateSend();
   bool ValidateReceive();

   GWallet* p_GWallet;

   wxSearchCtrl* send_to;
   wxTextCtrl* send_amount;
   wxComboBox* send_asset;
   wxSearchCtrl* receive_from;
   wxTextCtrl* receive_amount;
   wxSearchCtrl* receive_asset;
   wxTextCtrl* send_url;
   wxTextCtrl* receive_url;

private:
};

const int ID_TRANSFER_OK = 4221;
const int ID_SEND_FROM = 4222;
const int ID_SEND_GENERATE_URL = 4223;
const int ID_RECEIVE_GENERATE_URL = 4224;
const int ID_RECEIVE_ASSET = 4225;

