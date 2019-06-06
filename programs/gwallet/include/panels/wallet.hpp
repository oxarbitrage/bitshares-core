#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>

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
   wxTreeCtrl* wallet_tree;
   wxStaticText* locked_msg;

private:
   void InitWidgetsFromXRC(wxWindow *parent) {
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Wallet"), wxT("wxPanel"));
      wallet_tree = XRCCTRL(*this,"wallet_tree",wxTreeCtrl);
      locked_msg = XRCCTRL(*this,"locked_msg",wxStaticText);
   }

   void OnCommand(wxTreeEvent& event);

   void DoTransfer();
   void DoSellAsset();
   void DoBorrowAsset();
   void DoCancelOrder();
   void DoSetProxy();
   void DoSuggestBrainKey();
   void DoGetCommitteeMember();
   void DoGetAccountHistory();
   void DoGetOrderBook();
   void DoAbout();
   void DoInfo();
   void DoActiveWitnesses();
   void DoActiveCommittee();

   void OpenCommandsPane();
};
