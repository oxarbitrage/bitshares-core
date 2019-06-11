#include "../include/panels/wallet.hpp"
#include "../include/panels/transfer.hpp"
#include "../include/panels/sellasset.hpp"
#include "../include/panels/borrowasset.hpp"
#include "../include/panels/cancelorder.hpp"
#include "../include/panels/setproxy.hpp"
#include "../include/panels/getcommitteemember.hpp"
#include "../include/panels/getaccounthistory.hpp"
#include "../include/panels/getorderbook.hpp"
#include "../include/panels/about.hpp"
#include "../include/panels/info.hpp"
#include "../include/panels/active_committee.hpp"
#include "../include/panels/active_witnesses.hpp"

#include "../include/panels/commands.hpp"

Wallet::Wallet(GWallet* gwallet) : wxPanel()
{
   p_GWallet = gwallet;
   InitWidgetsFromXRC((wxWindow *)p_GWallet);

   const auto root = wallet_tree->AddRoot("Operations");

   wallet_tree->AppendItem(root, "Transfer");
   wallet_tree->AppendItem(root, "Sell asset");
   wallet_tree->AppendItem(root, "Borrow asset");
   wallet_tree->AppendItem(root, "Cancel order");
   wallet_tree->AppendItem(root, "Set proxy");
   wallet_tree->AppendItem(root, "Suggest brain key");
   wallet_tree->AppendItem(root, "Get committee member");
   wallet_tree->AppendItem(root, "Get account history");
   wallet_tree->AppendItem(root, "Get order book");
   wallet_tree->AppendItem(root, "About");
   wallet_tree->AppendItem(root, "Info");
   wallet_tree->AppendItem(root, "Active witnesses");
   wallet_tree->AppendItem(root, "Active committee");

   wallet_tree->ExpandAll();

   if(p_GWallet->bitshares.wallet_api_ptr->is_locked())
      DisableOperations();
   else
      EnableOperations();

   Connect(wallet_tree->GetId(), wxEVT_TREE_ITEM_ACTIVATED, wxTreeEventHandler(Wallet::OnCommand), NULL, this);
}

void Wallet::EnableOperations()
{
   wallet_tree->Enable(true);
   locked_msg->SetForegroundColour(wxNullColour);
   locked_msg->SetLabel(_("Please double click a command from below"));
}

void Wallet::DisableOperations()
{
   wallet_tree->Enable(false);
}

void Wallet::OnCommand(wxTreeEvent& event)
{
   auto selected = wallet_tree->GetItemText(event.GetItem()).ToStdString();

   OpenCommandsPane();

   if(selected == "Transfer")
      DoTransfer();
   else if(selected == "Sell asset")
      DoSellAsset();
   else if(selected == "Sell asset")
      DoSellAsset();
   else if(selected == "Borrow asset")
      DoBorrowAsset();
   else if(selected == "Cancel order")
      DoCancelOrder();
   else if(selected == "Set proxy")
      DoSetProxy();
   else if(selected == "Suggest brain key")
      DoSuggestBrainKey();
   else if(selected == "Get committee member")
      DoGetCommitteeMember();
   else if(selected == "Get account history")
      DoGetAccountHistory();
   else if(selected == "Get order book")
      DoGetOrderBook();
   else if(selected == "About")
      DoAbout();
   else if(selected == "Info")
      DoInfo();
   else if(selected == "Active witnesses")
      DoActiveWitnesses();
   else if(selected == "Active committee")
      DoActiveCommittee();

}

void Wallet::DoTransfer()
{
   Transfer *transfer = new Transfer(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(transfer, _("Transfer"));
}

void Wallet::DoSellAsset()
{
   SellAsset *sell_asset = new SellAsset(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(sell_asset, _("Sell Asset"));
}

void Wallet::DoBorrowAsset()
{
   BorrowAsset *borrow_asset = new BorrowAsset(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(borrow_asset, _("Borrow Asset"));
}

void Wallet::DoCancelOrder()
{
   CancelOrder *cancel_order = new CancelOrder(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(cancel_order, _("Cancel order"));
}

void Wallet::DoSetProxy()
{
   SetProxy *set_proxy = new SetProxy(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(set_proxy, _("Set proxy"));
}

void Wallet::DoSuggestBrainKey()
{
   auto result = p_GWallet->bitshares.wallet_api_ptr->suggest_brain_key();
   wxMessageBox(result.brain_priv_key, _("Suggested Brain key"), wxNO_DEFAULT|wxOK|wxICON_INFORMATION, this);
}

void Wallet::DoGetCommitteeMember()
{
   GetCommitteeMember *committee_member = new GetCommitteeMember(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(committee_member, _("Committee member"));
}

void Wallet::DoGetAccountHistory()
{
   GetAccountHistory *account_history = new GetAccountHistory(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(account_history, _("Account history"));
}

void Wallet::DoGetOrderBook()
{
   GetOrderBook *order_book = new GetOrderBook(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(order_book, _("Order book"));
}

void Wallet::DoAbout()
{
   About *about = new About(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(about, _("About"));
}

void Wallet::DoInfo()
{
   Info *info = new Info(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(info, _("Blockchain information"));
}

void Wallet::DoActiveWitnesses()
{
   ActiveWitnesses *active_witnesses = new ActiveWitnesses(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(active_witnesses, _("Active witnsses"));
}

void Wallet::DoActiveCommittee()
{
   ActiveCommittee *active_committee = new ActiveCommittee(p_GWallet);
   p_GWallet->panels.p_commands->notebook->AddPage(active_committee, _("Active committee"));
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
