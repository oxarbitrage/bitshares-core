#include "../include/modes/wallet.hpp"
#include "../include/dialogs/transfer.hpp"
#include "../include/dialogs/sellasset.hpp"
#include "../include/dialogs/borrowasset.hpp"
#include "../include/dialogs/cancelorder.hpp"
#include "../include/dialogs/setproxy.hpp"

Wallet::Wallet(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   Connect(XRCID("transfer"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnTransfer), NULL, this);
   Connect(XRCID("sell_asset"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnSellAsset), NULL, this);
   Connect(XRCID("borrow_asset"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnBorrowAsset), NULL, this);
   Connect(XRCID("cancel_order"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnCancelOrder), NULL, this);
   Connect(XRCID("update_proxy"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnSetProxy), NULL, this);
}

void Wallet::EnableOperations()
{
   transfer->Enable(true);
   sell_asset->Enable(true);
   borrow_asset->Enable(true);
   cancel_order->Enable(true);
   update_proxy->Enable(true);
}

void Wallet::DisableOperations()
{
   transfer->Enable(false);
   sell_asset->Enable(false);
   borrow_asset->Enable(false);
   cancel_order->Enable(false);
   update_proxy->Enable(false);
}

void Wallet::OnTransfer(wxCommandEvent& event)
{
   TransferDialog TransferDialog( this );
}

void Wallet::OnSellAsset(wxCommandEvent& event)
{
   SellAssetDialog SellAssetDialog( this );
}

void Wallet::OnBorrowAsset(wxCommandEvent& event)
{
   BorrowAssetDialog BorrowAssetDialog( this );
}

void Wallet::OnCancelOrder(wxCommandEvent& event)
{
   CancelOrderDialog CancelOrderDialog( this );
}

void Wallet::OnSetProxy(wxCommandEvent& event)
{
   SetProxyDialog SetProxyDialog( this, -1, _("Update your voting account"), wxDefaultPosition, wxSize(-1, 300));
}
