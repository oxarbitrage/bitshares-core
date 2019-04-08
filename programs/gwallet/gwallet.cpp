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

   const wxIcon application_icon(directory + wxT("/icons/btslogo.png"), wxBITMAP_TYPE_PNG);
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
      state.is_noconfig = true;
   } else {
      state.is_noconfig = false;
      if(config->Read("AllSet", &allset))
         state.is_account_linked = true;
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

      if(state.is_connected) {
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
   const wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   const wxString defaultDir(f.GetPath());

   const wxString wildcard = wxT("JSON files (*.json)|*.json");

   wxFileDialog dialog(this, _("Open a saved wallet"), defaultDir, wxT("wallet.json"), wildcard, wxFC_SAVE);
   if (dialog.ShowModal() == wxID_OK)
   {
      const wxString path = dialog.GetPath();
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
      const wxString ws_server = dialog.GetValue();
      config->Write("Server", ws_server);
      config->Flush();

      if(state.is_connected) {
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

      if (bitshares.wallet_api_ptr->is_new()) {
         state.is_new = true;
         state.is_locked = false;
         state.is_unlocked = false;
         state.is_account_linked = false;
      } else {
         if (bitshares.wallet_api_ptr->is_locked()) {
            state.is_new = false;
            state.is_locked = true;
            state.is_unlocked = false;
            state.is_account_linked = true;
         } else {
            state.is_new = false;
            state.is_locked = false;
            state.is_unlocked = true;
            state.is_account_linked = true;
         }

         DoAccounts();
         DoAssets(strings.first_account_name.ToStdString());

         strings.selected_account = strings.first_account_name;
         strings.selected_asset = strings.assets[0];

         if(!state.modes_created) {
            DoModes();
            state.modes_created = true;
         }
      }
      state.is_connected = true;
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

   state.is_connected = false;
   DoState();
}

void GWallet::OnSetPassword(wxCommandEvent& WXUNUSED(event))
{
   wxPasswordEntryDialog dialog(this, _("Enter password"));
   if (dialog.ShowModal() == wxID_OK)
   {
      const wxString password = dialog.GetValue();

      bitshares.wallet_api_ptr->set_password(password.ToStdString());
      SetStatusText(_("Connected | Locked"));

      state.is_new = false;
      state.is_locked = true;
      DoState();

      if(!state.modes_created) {
         DoModes();
         state.modes_created = true;
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
   state.is_locked = true;
   state.is_unlocked = false;
   DoState();

   modes.p_wallet->DisableOperations();
}

void GWallet::OnUnlock(wxCommandEvent& WXUNUSED(event))
{
   wxPasswordEntryDialog dialog(this, _("Enter password"));
   if (dialog.ShowModal() == wxID_OK)
   {
      const wxString password = dialog.GetValue();

      try {
         bitshares.wallet_api_ptr->unlock(password.ToStdString());
      }
      catch(const fc::exception& e)
      {
         OnError(_("Password is incorrect, please try again."));
      }
      state.is_locked = false;
      state.is_unlocked = true;
      DoState();
      modes.p_wallet->EnableOperations();
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

   const auto selected = strings.combo_accounts->GetCurrentSelection();
   const auto account_name = strings.accounts[selected];

   DoAssets(account_name.ToStdString());
   modes.p_history->DoHistory(account_name.ToStdString());

   strings.selected_account = strings.accounts[selected];
   strings.selected_asset = strings.assets[0];

   modes.p_home->DoAccount();
}

void GWallet::OnChangeAsset(wxCommandEvent& WXUNUSED(event))
{
   const auto selected = strings.combo_assets->GetCurrentSelection();

   const auto asset_name = strings.assets[selected];
   const auto balance = strings.balances[selected].ToStdString();
   const auto precision = strings.precisions[selected].ToStdString();

   const auto dividend = pow(10, std::stoi(precision));

   stringstream pretty_balance;
   pretty_balance << fixed << std::setprecision(std::stoi(precision)) << std::stod(balance)/dividend;

   strings.balance->SetLabel(pretty_balance.str() + " " + asset_name);

   modes.p_sendreceive->send_asset->SetSelection(selected);
   strings.selected_asset = asset_name;

   sizers.info->Layout();
   sizers.transfer->Layout();
}

void GWallet::DoAssets(std::string account)
{
   strings.combo_assets->Clear();
   strings.assets.Clear();

   int n = 0;
   std::string first_asset;
   std::string first_balance;
   std::string first_precision;

   const auto my_balances = bitshares.wallet_api_ptr->list_account_balances(account);
   for( auto& mb : my_balances ) {

      std::string asset_id = fc::to_string(mb.asset_id.space_id)
            + "." + fc::to_string(mb.asset_id.type_id)
            + "." + fc::to_string(mb.asset_id.instance.value);

      const auto asset = bitshares.wallet_api_ptr->get_asset(asset_id).symbol;

      if(n == 0) {
         first_asset = asset;
         first_balance = fc::to_string(mb.amount.value);
         first_precision = bitshares.wallet_api_ptr->get_asset(asset_id).precision;

         // insert balance
         const auto dividend = pow(10, bitshares.wallet_api_ptr->get_asset(asset_id).precision);
         const auto precision = bitshares.wallet_api_ptr->get_asset(asset_id).precision;
         stringstream pretty_balance;

         pretty_balance << fixed << std::setprecision(precision)  << mb.amount.value/dividend;
         strings.balance->SetLabel(pretty_balance.str() + " " + first_asset);
         sizers.info->Layout();
      }
      strings.assets.Add(asset);
      strings.balances.Add(fc::to_string(mb.amount.value));
      strings.precisions.Add(fc::to_string(bitshares.wallet_api_ptr->get_asset(asset_id).precision));
      n++;
   }
   strings.combo_assets->Set(strings.assets);
   strings.combo_assets->SetSelection(0);
   strings.combo_assets->Enable(true);
}

void GWallet::DoAccounts()
{
   const auto my_accounts = bitshares.wallet_api_ptr->list_my_accounts();
   int n = 0;

   for( auto& ma : my_accounts ) {
      auto name = ma.name;
      if(n == 0) strings.first_account_name = name;
      strings.accounts.Add(name);
      n++;
   }
   strings.combo_accounts->Set(strings.accounts);
   strings.combo_accounts->SetSelection(0);
   strings.combo_accounts->Enable(true);
}

void GWallet::DoModes()
{
   Home *home = new Home(this);
   home->CreateControls();
   home->CreateEvents();
   modes.p_home = home;

   Cli *cli = new Cli(this);
   cli->CreateControls();
   cli->CreateEvents();
   modes.p_cli = cli;

   SendReceive *sendreceive = new SendReceive(this);
   sendreceive->CreateControls();
   sendreceive->CreateEvents();
   modes.p_sendreceive = sendreceive;

   History *history = new History(this);
   history->CreateControls();
   history->CreateEvents();
   modes.p_history = history;

   Wallet *wallet = new Wallet(this);
   wallet->CreateControls();
   wallet->CreateEvents();
   modes.p_wallet = wallet;
}

void GWallet::LoadWelcomeWidget()
{
   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   wxString directory(f.GetPath());

   const wxBitmap wizard_icon(directory + wxT("/icons/wizard.png"), wxBITMAP_TYPE_PNG);

   welcome.wizard = new wxWizard(panel, ID_WIZARD, _("Welcome to Bitshares G-Wallet"),
         wizard_icon, wxDefaultPosition, wxDEFAULT_DIALOG_STYLE);

   welcome.page1 = new Welcome1(welcome.wizard, this);
   welcome.page2 = new Welcome2(welcome.wizard, this);
   welcome.page3 = new Welcome3(welcome.wizard, this);
   welcome.page4 = new Welcome4(welcome.wizard, this);

   (*welcome.page1).Chain(welcome.page2).Chain(welcome.page3).Chain(welcome.page4);

   welcome.wizard->RunWizard(welcome.page1);
}

void GWallet::OnHomeMode(wxCommandEvent& WXUNUSED(event))
{
   //Home::StartTimer();
   //m_timer.Start(30000);
   //p_home->DoInitialData();

   sizers.main->Show(sizers.home, true, true);
   sizers.main->Hide(sizers.cli, true);
   sizers.main->Hide(sizers.transfer, true);
   sizers.main->Hide(sizers.history, true);
   sizers.main->Hide(sizers.wallet, true);
   sizers.main->Layout();
}

void GWallet::OnCommandMode(wxCommandEvent& WXUNUSED(event))
{
   //Home::StopTimer();
   //m_timer.Stop();
   sizers.main->Hide(sizers.home, true);
   sizers.main->Show(sizers.cli, true, true);
   sizers.main->Hide(sizers.transfer, true);
   sizers.main->Hide(sizers.history, true);
   sizers.main->Hide(sizers.wallet, true);
   sizers.main->Layout();
}

void GWallet::OnTransferMode(wxCommandEvent& WXUNUSED(event))
{
   //m_timer.Stop();
   sizers.main->Hide(sizers.home, true);
   sizers.main->Hide(sizers.cli, true);
   sizers.main->Show(sizers.transfer, true, true);
   sizers.main->Hide(sizers.history, true);
   sizers.main->Hide(sizers.wallet, true);
   sizers.main->Layout();
}

void GWallet::OnHistoryMode(wxCommandEvent& WXUNUSED(event))
{
   //m_timer.Stop();

   wxWindowDisabler disableAll;
   wxBusyInfo wait(_("Please wait, pulling the last history of your account ..."));
   wxTheApp->Yield();
   modes.p_history->DoHistory(strings.first_account_name.ToStdString());

   sizers.main->Hide(sizers.home, true);
   sizers.main->Hide(sizers.cli, true);
   sizers.main->Hide(sizers.transfer, true);
   sizers.main->Show(sizers.history, true, true);
   sizers.main->Hide(sizers.wallet, true);
   sizers.main->Layout();
}

void GWallet::OnWalletMode(wxCommandEvent& WXUNUSED(event))
{
   //m_timer.Stop();

   sizers.main->Hide(sizers.home, true);
   sizers.main->Hide(sizers.cli, true);
   sizers.main->Hide(sizers.transfer, true);
   sizers.main->Hide(sizers.history, true);
   sizers.main->Show(sizers.wallet, true, true);
   sizers.main->Layout();
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
   wxMessageDialog dialog(NULL, msg, _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);
   if (dialog.ShowModal() == wxID_OK)
      return;
}
