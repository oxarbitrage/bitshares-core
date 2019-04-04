#ifndef GWALLET_H
#define GWALLET_H
#include "include/gwallet.hpp"
#endif
#include "include/dialogs/importkey.hpp"

#include "include/wizards/welcome.hpp"

#include "include/modes/home.hpp"
#include "include/modes/cli.hpp"
#include "include/modes/sendreceive.hpp"
#include "include/modes/history.hpp"
#include "include/modes/wallet.hpp"

#include <wx/stattext.h>
#include <wx/statline.h>
#include <wx/busyinfo.h>
#include <wx/listctrl.h>
#include <wx/combobox.h>
#include <wx/notebook.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/hyperlink.h>
#include <wx/filedlg.h>

GWallet::GWallet(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1200, 900))
{
   // full size
   //wxTopLevelWindow::Maximize(true);

   wxTopLevelWindow::SetMinSize(wxSize(600, 450));

   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   directory = f.GetPath();

   wxIcon application_icon(directory + wxT("/icons/btslogo.png"), wxBITMAP_TYPE_PNG);
   SetIcon(application_icon);

   CreateMenu();
   CreateEvents();
   CreateTool();
   CreateMain();
   CreateInfo();

   CreateStatusBar(2);

   config = new wxConfig(wxT("GWallet"));

   wxString path;
   wxString server;
   bool allset;
   if (!config->Read("WalletPath", &path) || !config->Read("Server", &server)) {
      is_noconfig = true;
   } else {
      is_noconfig = false;
      if(config->Read("AllSet", &allset))
         is_account_linked = true;
   }
   DoState();

   // todo: delete all config, enable to start with an empty config, could be command line option
   //config->DeleteAll();

   Centre();
}

void GWallet::OnNew(wxCommandEvent & WXUNUSED(event))
{
   LoadWelcomeWidget();
}

void GWallet::OnOpen(wxCommandEvent & WXUNUSED(event))
{
   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   wxString defaultDir(f.GetPath());

   wxString wildcard = wxT("JSON files (*.json)|*.json");

   wxFileDialog dialog(this, _("Open a saved wallet"), defaultDir, wxT("wallet.json"), wildcard);
   if (dialog.ShowModal() == wxID_OK)
   {
      wxString path = dialog.GetPath();
      config->Write("WalletPath", path);
      config->Flush();

      if(is_connected) {
         wxCommandEvent event_disconnect(wxEVT_COMMAND_MENU_SELECTED, ID_DISCONNECT);
         ProcessWindowEvent(event_disconnect);
         wxSleep(1);
      }

      wxCommandEvent event_connect(wxEVT_COMMAND_MENU_SELECTED, ID_CONNECT);
      ProcessWindowEvent(event_connect);
   }
}

void GWallet::OnSave(wxCommandEvent & WXUNUSED(event))
{
   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   wxString defaultDir(f.GetPath());

   wxString wildcard = wxT("JSON files (*.json)|*.json");

   wxFileDialog dialog(this, _("Open a saved wallet"), defaultDir, wxT("wallet.json"), wildcard, wxFC_SAVE);
   if (dialog.ShowModal() == wxID_OK)
   {
      wxString path = dialog.GetPath();
      bitshares.wallet_api_ptr->save_wallet_file(path.ToStdString());
      config->Write("WalletPath", path);
      config->Flush();
   }
}

void GWallet::OnNetwork(wxCommandEvent & WXUNUSED(event))
{
   wxTextEntryDialog dialog(this, _("Enter server"), _("Websocket endpoint"));
   if ( dialog.ShowModal() == wxID_OK )
   {
      wxString ws_server = dialog.GetValue();
      config->Write("Server", ws_server);
      config->Flush();

      if(is_connected) {
         wxCommandEvent event_disconnect(wxEVT_COMMAND_MENU_SELECTED, ID_DISCONNECT);
         ProcessWindowEvent(event_disconnect);
         wxSleep(1);
      }
      wxCommandEvent event_connect(wxEVT_COMMAND_MENU_SELECTED, ID_CONNECT);
      ProcessWindowEvent(event_connect);
   }
}

void GWallet::OnQuit(wxCommandEvent& WXUNUSED(event))
{
   Close(true);
}

void GWallet::OnConnect(wxCommandEvent& WXUNUSED(event))
{
   wxWindowDisabler disableAll;
   wxBusyInfo wait(_("Please wait, connecting ..."));
   wxTheApp->Yield();

   wxString path;
   wxString server;
   if (config->Read("WalletPath", &path) && config->Read("Server", &server)) {

      try {
         bitshares.Connect(server.ToStdString(), path.ToStdString());
      }
      catch(const fc::exception &e) {
         OnError(_("Problem at connecting, please try again ..."));
      }

      int widths[] = {GetClientSize().x - 25 - GetTextExtent(server).x, -1};
      SetStatusWidths(2, widths);
      SetStatusText(_("Connected"), 0);
      SetStatusText(server, 1);

      if (bitshares.wallet_api_ptr->is_new()) {
         is_new = true;
         is_locked = false;
         is_unlocked = false;
         is_account_linked = false;
      } else {
         if (bitshares.wallet_api_ptr->is_locked()) {
            is_new = false;
            is_locked = true;
            is_unlocked = false;
            is_account_linked = true;
         } else {
            is_new = false;
            is_locked = false;
            is_unlocked = true;
            is_account_linked = true;
         }

         DoAccounts();
         DoAssets(first_account_name);

         selected_account = first_account_name;
         selected_asset = strings_assets[0];

         if(!modes_created) {
            DoModes();
            modes_created = true;
         }

      }

      is_connected = true;
      DoState();
   }
}

void GWallet::OnDisconnect(wxCommandEvent& WXUNUSED(event))
{
   try {
      bitshares.Disconnect();
   }
   catch(const fc::exception& e)
   {
      OnError(_("Some problem when disconnecting, try again ..."));
   }
   SetStatusText(_("Disconnected"));

   is_connected = false;

   DoState();
}

void GWallet::OnSetPassword(wxCommandEvent& WXUNUSED(event))
{
   wxPasswordEntryDialog dialog(this, _("Enter password"));
   if (dialog.ShowModal() == wxID_OK)
   {
      wxString value = dialog.GetValue();

      bitshares.wallet_api_ptr->set_password(value.ToStdString());
      SetStatusText(_("Connected | Locked"));

      is_new = false;
      is_locked = true;
      DoState();

      if(!modes_created) {
         DoModes();
         modes_created = true;
      }
   }
}
void GWallet::OnAbout(wxCommandEvent& WXUNUSED(event))
{
   wxString msg;
   msg.Printf(_("Secure and simple bitshares wallet build with %s"), wxVERSION_STRING);
   wxMessageBox(msg, _("About G-Wallet"), wxOK | wxICON_INFORMATION, this);
}

void GWallet::OnLock(wxCommandEvent & WXUNUSED(event))
{
   try
   {
      bitshares.wallet_api_ptr->lock();
   }
   catch(const fc::exception& e)
   {
      OnError(_("Some problem when locking, try again ..."));
   }
   SetStatusText(wxT("Connected | Locked"));
   wallet_m->Enable(ID_UNLOCK, true);
   wallet_m->Enable(ID_LOCK, false);
   itemToolBar->EnableTool(ID_ICON_LOCK, false);
   itemToolBar->EnableTool(ID_ICON_UNLOCK, true);

   p_wallet->DisableOperations();
}

void GWallet::OnUnlock(wxCommandEvent& WXUNUSED(event))
{
   wxPasswordEntryDialog dialog(this, _("Enter password"));
   if (dialog.ShowModal() == wxID_OK)
   {
      wxString value = dialog.GetValue();

      try {
         bitshares.wallet_api_ptr->unlock(value.ToStdString());
      }
      catch(const fc::exception& e)
      {
         OnError(_("Password is incorrect, please try again."));
      }
      is_locked = false;
      is_unlocked = true;
      DoState();
      p_wallet->EnableOperations();
   }
}
void GWallet::OnImportKey(wxCommandEvent& WXUNUSED(event))
{
   ImportKeyDialog importKeyDialog( this, -1, _("Import key"), wxDefaultPosition, wxDefaultSize );
}

void GWallet::OnChangeAccount(wxCommandEvent& WXUNUSED(event))
{
   wxWindowDisabler disableAll;
   wxBusyInfo wait(_("Please wait, switching accounts ..."));
   wxTheApp->Yield();

   auto selected = combo_accounts->GetCurrentSelection();
   auto account_name = strings_accounts[selected];

   DoAssets(account_name.ToStdString());
   p_history->DoHistory(account_name.ToStdString());

   selected_account = strings_accounts[selected];
   selected_asset = strings_assets[0];
}

void GWallet::OnChangeAsset(wxCommandEvent& WXUNUSED(event))
{
   auto selected = combo_assets->GetCurrentSelection();

   auto asset_name = strings_assets[selected];
   auto balance = strings_balances[selected].ToStdString();
   auto precision = strings_precisions[selected].ToStdString();

   auto dividend = pow(10, std::stoi(precision));

   stringstream pretty_balance;
   pretty_balance << fixed << std::setprecision(std::stoi(precision)) << std::stod(balance)/dividend;

   balanceMsg->SetLabel(pretty_balance.str() + " " + asset_name);

   p_sendreceive->send_asset->SetSelection(selected);

   infoSizer->Layout();
   sizerTransferMode->Layout();
}

void GWallet::DoAssets(std::string account)
{
   combo_assets->Clear();
   strings_assets.Clear();

   int n = 0;
   std::string first_asset;
   std::string first_balance;
   std::string first_precision;

   auto my_balances = bitshares.wallet_api_ptr->list_account_balances(account);
   for( auto& mb : my_balances ) {

      std::string asset_id = fc::to_string(mb.asset_id.space_id)
            + "." + fc::to_string(mb.asset_id.type_id)
            + "." + fc::to_string(mb.asset_id.instance.value);

      auto asset = bitshares.wallet_api_ptr->get_asset(asset_id).symbol;

      if(n == 0) {
         first_asset = asset;
         first_balance = fc::to_string(mb.amount.value);
         first_precision = bitshares.wallet_api_ptr->get_asset(asset_id).precision;

         // insert balance
         auto dividend = pow(10, bitshares.wallet_api_ptr->get_asset(asset_id).precision);
         auto precision = bitshares.wallet_api_ptr->get_asset(asset_id).precision;
         stringstream pretty_balance;

         pretty_balance << fixed << std::setprecision(precision)  << mb.amount.value/dividend;
         balanceMsg->SetLabel(pretty_balance.str() + " " + first_asset);
         infoSizer->Layout();
      }
      strings_assets.Add(asset);
      strings_balances.Add(fc::to_string(mb.amount.value));
      strings_precisions.Add(fc::to_string(bitshares.wallet_api_ptr->get_asset(asset_id).precision));
      n++;
   }
   combo_assets->Set(strings_assets);
   combo_assets->SetSelection(0);
   combo_assets->Enable(true);
}

void GWallet::DoAccounts()
{
   auto my_accounts = bitshares.wallet_api_ptr->list_my_accounts();
   int n = 0;

   for( auto& ma : my_accounts ) {
      auto name = ma.name;
      if(n == 0) first_account_name = name;
      strings_accounts.Add(name);
      n++;
   }
   combo_accounts->Set(strings_accounts);
   combo_accounts->SetSelection(0);
   combo_accounts->Enable(true);
}

void GWallet::DoModes()
{
   Home *home = new Home(this);
   home->CreateControls();
   home->CreateEvents();

   Cli *cli = new Cli(this);
   cli->CreateControls();
   cli->CreateEvents();

   SendReceive *sendreceive = new SendReceive(this);
   sendreceive->CreateControls();
   sendreceive->CreateEvents();
   p_sendreceive = sendreceive;

   History *history = new History(this);
   history->CreateControls();
   history->CreateEvents();
   p_history = history;

   Wallet *wallet = new Wallet(this);
   wallet->CreateControls();
   wallet->CreateEvents();
   p_wallet = wallet;

}

void GWallet::LoadWelcomeWidget()
{
   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   wxString directory(f.GetPath());

   wxBitmap wizard_icon(directory + wxT("/icons/wizard.png"), wxBITMAP_TYPE_PNG);

   wizard = new wxWizard(panel, ID_WIZARD, _("Welcome to Bitshares G-Wallet"),
         wizard_icon, wxDefaultPosition, wxDEFAULT_DIALOG_STYLE);

   page1 = new Welcome1(wizard, this);
   page2 = new Welcome2(wizard, this);
   page3 = new Welcome3(wizard, this);
   page4 = new Welcome4(wizard, this);

   (*page1).Chain(page2).Chain(page3).Chain(page4);

   wizard->RunWizard(page1);
}

void GWallet::OnHomeMode(wxCommandEvent& WXUNUSED(event))
{
   //Home::StartTimer();
   //m_timer.Start(30000);
   mainSizer->Show(sizerHomeMode, true, true);
   mainSizer->Hide(sizerCommandMode, true);
   mainSizer->Hide(sizerTransferMode, true);
   mainSizer->Hide(sizerHistoryMode, true);
   mainSizer->Hide(sizerWalletMode, true);
   mainSizer->Layout();
}

void GWallet::OnCommandMode(wxCommandEvent& WXUNUSED(event))
{
   //Home::StopTimer();
   //m_timer.Stop();
   mainSizer->Show(sizerCommandMode, true, true);
   mainSizer->Hide(sizerHomeMode, true);
   mainSizer->Hide(sizerHistoryMode, true);
   mainSizer->Hide(sizerTransferMode, true);
   mainSizer->Hide(sizerWalletMode, true);
   mainSizer->Layout();
}

void GWallet::OnTransferMode(wxCommandEvent& WXUNUSED(event))
{
   //m_timer.Stop();
   mainSizer->Show(sizerTransferMode, true, true);
   mainSizer->Hide(sizerHomeMode, true);
   mainSizer->Hide(sizerCommandMode, true);
   mainSizer->Hide(sizerHistoryMode, true);
   mainSizer->Hide(sizerWalletMode, true);
   mainSizer->Layout();
}

void GWallet::OnHistoryMode(wxCommandEvent& WXUNUSED(event))
{
   //m_timer.Stop();

   wxWindowDisabler disableAll;
   wxBusyInfo wait(_("Please wait, pulling the last history of your account ..."));
   wxTheApp->Yield();
   p_history->DoHistory(first_account_name);

   mainSizer->Show(sizerHistoryMode, true, true);
   mainSizer->Hide(sizerHomeMode, true);
   mainSizer->Hide(sizerCommandMode, true);
   mainSizer->Hide(sizerTransferMode, true);
   mainSizer->Hide(sizerWalletMode, true);
   mainSizer->Layout();
}

void GWallet::OnWalletMode(wxCommandEvent& WXUNUSED(event))
{
   //m_timer.Stop();
   mainSizer->Show(sizerWalletMode, true, true);
   mainSizer->Hide(sizerHomeMode, true);
   mainSizer->Hide(sizerCommandMode, true);
   mainSizer->Hide(sizerTransferMode, true);
   mainSizer->Hide(sizerHistoryMode, true);
   mainSizer->Layout();
}

void GWallet::CreateEvents()
{
   Connect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnNew));
   Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnOpen));
   Connect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnSave));
   Connect(wxID_NETWORK, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnNetwork));
   Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnQuit));
   Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnAbout));

   Connect(ID_CONNECT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnConnect));
   Connect(ID_SETPASSWORD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnSetPassword));
   Connect(ID_LOCK, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnLock));
   Connect(ID_UNLOCK, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnUnlock));
   Connect(ID_IMPORTKEY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnImportKey));
   Connect(ID_DISCONNECT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnDisconnect));

   Connect(ID_ICON_CONNECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnConnect));
   Connect(ID_ICON_DISCONNECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnDisconnect));
   Connect(ID_ICON_LOCK, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnLock));
   Connect(ID_ICON_UNLOCK, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnUnlock));

   Connect(ID_ICON_HOME, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnHomeMode));
   Connect(ID_ICON_COMMAND, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnCommandMode));
   Connect(ID_ICON_SENDRECEIVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnTransferMode));
   Connect(ID_ICON_HISTORY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnHistoryMode));
   Connect(ID_ICON_WALLET, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnWalletMode));

   Connect(ID_COMBO_ACCOUNTS, wxEVT_COMBOBOX, wxCommandEventHandler(GWallet::OnChangeAccount));
   Connect(ID_COMBO_ASSETS, wxEVT_COMBOBOX, wxCommandEventHandler(GWallet::OnChangeAsset));
}

void GWallet::OnError(wxString msg)
{
   wxMessageDialog dialog( NULL, msg, _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);
   if (dialog.ShowModal() == wxID_OK)
      return;
}
