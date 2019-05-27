#include "../include/panels/wallet.hpp"
#include "../include/panels/transfer.hpp"
#include "../include/panels/sellasset.hpp"
#include "../include/panels/borrowasset.hpp"
#include "../include/panels/cancelorder.hpp"
#include "../include/panels/setproxy.hpp"
#include "../include/panels/getcommitteemember.hpp"

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
   Transfer *transfer = new Transfer(p_GWallet);

   if(p_GWallet->m_mgr.GetPane("Information").IsValid()) {
      wxWindow* wnd = p_GWallet->m_mgr.GetPane("Information").window;
      p_GWallet->m_mgr.DetachPane(wnd);
      wnd->Destroy();
   }

   wxAuiPaneInfo info;
   info.Top();
   info.PinButton();
   info.Caption(_("Transfer"));
   info.PinButton();
   info.Position(2);
   //info.Row(1);
   //info.Dock();
   //info.LeftDockable();
   //info.MinSize(600, 600);
   //info.BestSize(600, 600);
   //info.dock_proportion = 1;
   info.MaximizeButton();
   info.MinimizeButton();
   info.Name("Transfer");

   //wxWindow* wnd = p_GWallet->m_mgr.GetPane("Home").window;
   //p_GWallet->m_mgr.DetachPane(wnd);
   //wnd->Destroy();

   p_GWallet->m_mgr.AddPane(transfer, info);
   p_GWallet->m_mgr.Update();

}

void Wallet::OnSellAsset(wxCommandEvent& event)
{
   SellAsset *sellasset = new SellAsset(p_GWallet);

   wxAuiPaneInfo info;
   info.Top();
   info.Name("Sell Asset");
   info.Caption("Sell Asset");
   info.PinButton();
   info.Position(2);
   info.MaximizeButton();
   info.MinimizeButton();

   p_GWallet->m_mgr.AddPane(sellasset, info);
   p_GWallet->m_mgr.Update();
}

void Wallet::OnBorrowAsset(wxCommandEvent& event)
{
   BorrowAsset *borrow_asset = new BorrowAsset(p_GWallet);

   wxAuiPaneInfo info;
   info.Top();
   info.Name("Borrow Asset");
   info.Caption("Borrow Asset");
   info.PinButton();
   info.Position(2);
   info.MaximizeButton();
   info.MinimizeButton();

   p_GWallet->m_mgr.AddPane(borrow_asset, info);
   p_GWallet->m_mgr.Update();
}

void Wallet::OnCancelOrder(wxCommandEvent& event)
{
   CancelOrder *cancel_order = new CancelOrder(p_GWallet);

   wxAuiPaneInfo info;
   info.Top();
   info.Name("Cancel Order");
   info.Caption("Cancel Order");
   info.PinButton();
   info.Position(2);
   info.MaximizeButton();
   info.MinimizeButton();

   p_GWallet->m_mgr.AddPane(cancel_order, info);
   p_GWallet->m_mgr.Update();
}

void Wallet::OnSetProxy(wxCommandEvent& event)
{
   SetProxy *set_proxy = new SetProxy(p_GWallet);

   wxAuiPaneInfo info;
   info.Top();
   info.Name("Set proxy");
   info.Caption("Set proxy");
   info.PinButton();
   info.Position(2);
   info.MaximizeButton();
   info.MinimizeButton();

   p_GWallet->m_mgr.AddPane(set_proxy, info);
   p_GWallet->m_mgr.Update();
}

void Wallet::OnSuggestBrainKey(wxCommandEvent& event)
{
   auto result = p_GWallet->bitshares.wallet_api_ptr->suggest_brain_key();
   wxMessageBox(result.brain_priv_key, _("Suggested Brain key"), wxNO_DEFAULT|wxOK|wxICON_INFORMATION, this);
}

void Wallet::OnGetCommitteeMember(wxCommandEvent& event)
{
   GetCommitteeMember *cm = new GetCommitteeMember(p_GWallet);

   wxAuiPaneInfo info;
   info.Top();
   info.Name("Committee member");
   info.Caption("Committee member");
   info.PinButton();
   info.Position(2);
   info.MaximizeButton();
   info.MinimizeButton();

   p_GWallet->m_mgr.AddPane(cm, info);
   p_GWallet->m_mgr.Update();
}
