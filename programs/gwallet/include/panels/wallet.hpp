#pragma once

#include <wx/wx.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class Wallet : public wxPanel
{
public:
   Wallet(GWallet* gwallet);
   void EnableOperations();
   void DisableOperations();

   GWallet* p_GWallet;

   Wallet(wxWindow *parent=NULL) {
      InitWidgetsFromXRC((wxWindow *)parent);
   }

protected:
   wxButton* transfer;
   wxButton* sell_asset;
   wxButton* borrow_asset;
   wxButton* cancel_order;
   wxButton* update_proxy;
   wxButton* suggest_brain_key;
   wxButton* get_committee_member;
   wxButton* get_account_history;

private:
   void InitWidgetsFromXRC(wxWindow *parent) {
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Wallet"), wxT("wxPanel"));
      transfer = XRCCTRL(*this,"transfer",wxButton);
      sell_asset = XRCCTRL(*this,"sell_asset",wxButton);
      borrow_asset = XRCCTRL(*this,"borrow_asset",wxButton);
      cancel_order = XRCCTRL(*this,"cancel_order",wxButton);
      update_proxy = XRCCTRL(*this,"update_proxy",wxButton);
      suggest_brain_key = XRCCTRL(*this,"suggest_brain_key",wxButton);
      get_committee_member = XRCCTRL(*this,"get_committee_member",wxButton);
      get_account_history = XRCCTRL(*this,"get_account_history",wxButton);
   }

   void OnTransfer(wxCommandEvent& event);
   void OnSellAsset(wxCommandEvent& event);
   void OnBorrowAsset(wxCommandEvent& event);
   void OnCancelOrder(wxCommandEvent& event);
   void OnSetProxy(wxCommandEvent& event);
   void OnSuggestBrainKey(wxCommandEvent& event);
   void OnGetCommitteeMember(wxCommandEvent& event);
   void OnGetAccountHistory(wxCommandEvent& event);

   void CloseInformationPane();
};
