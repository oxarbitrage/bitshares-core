#include "../include/panels/wallet.hpp"
#include "../include/panels/transfer.hpp"
#include "../include/panels/sellasset.hpp"
#include "../include/panels/borrowasset.hpp"
#include "../include/panels/cancelorder.hpp"
#include "../include/panels/setproxy.hpp"
#include "../include/panels/getcommitteemember.hpp"
#include "../include/panels/getaccounthistory.hpp"
#include "../include/panels/getorderbook.hpp"

#include "../include/panels/commands.hpp"

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
   Connect(XRCID("get_account_history"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnGetAccountHistory), NULL, this);
   Connect(XRCID("get_order_book"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnGetOrderBook), NULL, this);

   if(p_GWallet->bitshares.wallet_api_ptr->is_locked())
      DisableOperations();
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
   get_account_history->Enable(true);
   get_order_book->Enable(true);
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
   get_account_history->Enable(false);
   get_order_book->Enable(false);
}

void Wallet::OnTransfer(wxCommandEvent& event)
{
   OpenCommandsPane();
   Transfer *transfer = new Transfer(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(transfer, "Transfer");
}

void Wallet::OnSellAsset(wxCommandEvent& event)
{
   OpenCommandsPane();
   SellAsset *sell_asset = new SellAsset(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(sell_asset, "Sell Asset");
}

void Wallet::OnBorrowAsset(wxCommandEvent& event)
{
   OpenCommandsPane();
   BorrowAsset *borrow_asset = new BorrowAsset(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(borrow_asset, "Borrow Asset");
}

void Wallet::OnCancelOrder(wxCommandEvent& event)
{
   OpenCommandsPane();
   CancelOrder *cancel_order = new CancelOrder(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(cancel_order, "Cancel order");
}

void Wallet::OnSetProxy(wxCommandEvent& event)
{
   OpenCommandsPane();
   SetProxy *set_proxy = new SetProxy(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(set_proxy, "Set proxy");
}

void Wallet::OnSuggestBrainKey(wxCommandEvent& event)
{
   auto result = p_GWallet->bitshares.wallet_api_ptr->suggest_brain_key();
   wxMessageBox(result.brain_priv_key, _("Suggested Brain key"), wxNO_DEFAULT|wxOK|wxICON_INFORMATION, this);
}

void Wallet::OnGetCommitteeMember(wxCommandEvent& event)
{
   OpenCommandsPane();
   GetCommitteeMember *committee_member = new GetCommitteeMember(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(committee_member, "Committee member");
}

void Wallet::OnGetAccountHistory(wxCommandEvent& event)
{
   OpenCommandsPane();
   GetAccountHistory *account_history = new GetAccountHistory(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(account_history, "Account history");
}

void Wallet::OnGetOrderBook(wxCommandEvent& event)
{
   OpenCommandsPane();
   GetOrderBook *order_book = new GetOrderBook(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(order_book, "Order book");
}

void Wallet::OpenCommandsPane()
{
   if(!p_GWallet->m_mgr.GetPane("Commands").IsShown()) {
      Commands *commands = new Commands(p_GWallet);
      p_GWallet->panels.p_commands = commands;
      p_GWallet->CreateCommandsPane(commands);
      p_GWallet->m_mgr.Update();

      p_GWallet->menubar->Check(XRCID("m_view_commands"), true);
   }
}
