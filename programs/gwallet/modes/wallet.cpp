#include "../include/modes/wallet.hpp"
#include "../include/dialogs/transfer.hpp"
#include "../include/dialogs/sellasset.hpp"
#include "../include/dialogs/borrowasset.hpp"
#include "../include/dialogs/cancelorder.hpp"
#include "../include/dialogs/setproxy.hpp"
#include "../include/dialogs/getcommitteemember.hpp"

Wallet::Wallet(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   Connect(XRCID("transfer"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnTransfer), NULL, this);
   Connect(XRCID("sell_asset"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnSellAsset), NULL, this);
   Connect(XRCID("borrow_asset"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnBorrowAsset), NULL, this);
   Connect(XRCID("cancel_order"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnCancelOrder), NULL, this);
   Connect(XRCID("update_proxy"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnSetProxy), NULL, this);
   Connect(XRCID("suggest_brain_key"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnSuggestBrainKey), NULL, this);
   Connect(XRCID("get_committee_member"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnGetCommitteeMember), NULL, this);
}

void Wallet::EnableOperations()
{
   transfer->Enable(true);
   sell_asset->Enable(true);
   borrow_asset->Enable(true);
   cancel_order->Enable(true);
   update_proxy->Enable(true);
   suggest_brain_key->Enable(true);
   get_committee_member->Enable(true);
}

void Wallet::DisableOperations()
{
   transfer->Enable(false);
   sell_asset->Enable(false);
   borrow_asset->Enable(false);
   cancel_order->Enable(false);
   update_proxy->Enable(false);
   suggest_brain_key->Enable(false);
   get_committee_member->Enable(false);
}

void Wallet::OnTransfer(wxCommandEvent& event)
{
   TransferDialog TransferDialog(this);
}

void Wallet::OnSellAsset(wxCommandEvent& event)
{
   SellAssetDialog SellAssetDialog(this);
}

void Wallet::OnBorrowAsset(wxCommandEvent& event)
{
   BorrowAssetDialog BorrowAssetDialog(this);
}

void Wallet::OnCancelOrder(wxCommandEvent& event)
{
   CancelOrderDialog CancelOrderDialog(this);
}

void Wallet::OnSetProxy(wxCommandEvent& event)
{
   SetProxyDialog SetProxyDialog(this);
}

void Wallet::OnSuggestBrainKey(wxCommandEvent& event)
{
   auto result = p_GWallet->bitshares.wallet_api_ptr->suggest_brain_key();
   wxMessageBox(result.brain_priv_key, _("Suggested Brain key"), wxNO_DEFAULT|wxOK|wxICON_INFORMATION, this);
}

void Wallet::OnGetCommitteeMember(wxCommandEvent& event)
{
   GetCommitteeMemberDialog GetCommitteeMemberDialog(this);
}
