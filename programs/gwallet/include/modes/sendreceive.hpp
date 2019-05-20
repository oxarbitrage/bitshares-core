#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/combo.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class SendReceive : public wxPanel
{
public:
   SendReceive(GWallet* gwallet);

protected:
   wxSearchCtrl* send_to;
   wxTextCtrl* send_amount;
   wxComboBox* send_asset;
   wxButton* generate_url_send;
   wxButton* send;
   wxTextCtrl* generated_url_send;
   wxSearchCtrl* receive_from;
   wxTextCtrl* receive_amount;
   wxSearchCtrl* receive_asset;
   wxButton* generate_url_receive;
   wxTextCtrl* generated_url_receive;


private:
   void OnTransferOk(wxCommandEvent &event);
   void OnSearchAccount(wxCommandEvent &event);
   void OnSearchAsset(wxCommandEvent &event);

   void OnSendUrl(wxCommandEvent &event);
   void OnReceiveUrl(wxCommandEvent &event);

   bool ValidateSend();
   bool ValidateReceive();

   GWallet* p_GWallet;

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("SendReceive"), wxT("wxPanel"));
      send_to = XRCCTRL(*this,"send_to",wxSearchCtrl);
      send_amount = XRCCTRL(*this,"send_amount",wxTextCtrl);
      send_asset = XRCCTRL(*this,"send_asset",wxComboBox);
      generate_url_send = XRCCTRL(*this,"generate_url_send",wxButton);
      send = XRCCTRL(*this,"send",wxButton);
      generated_url_send = XRCCTRL(*this,"generated_url_send",wxTextCtrl);
      receive_from = XRCCTRL(*this,"receive_from",wxSearchCtrl);
      receive_amount = XRCCTRL(*this,"receive_amount",wxTextCtrl);
      receive_asset = XRCCTRL(*this,"receive_asset",wxSearchCtrl);
      generate_url_receive = XRCCTRL(*this,"generate_url_receive",wxButton);
      generated_url_receive = XRCCTRL(*this,"generated_url_receive",wxTextCtrl);
   }
};

