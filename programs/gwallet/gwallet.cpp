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
#include <wx/filectrl.h>

//GWallet::GWallet(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1200, 900))
GWallet::GWallet(const wxString& title)
{
   DoInitialConfig();

   wxImage::AddHandler(new wxPNGHandler);
   InitWidgetsFromXRC((wxWindow *)NULL);

   CreateEvents();
   DoState();

   // todo: delete all config, enable to start with an empty config, could be command line option
   //config->DeleteAll();

   Centre();
}

void GWallet::OnNew(wxCommandEvent& WXUNUSED(event))
{
   LoadWelcomeWidget();
}

void GWallet::OnOpen(wxCommandEvent& WXUNUSED(event))
{
   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   wxString defaultDir(f.GetPath());

   wxString wildcard = wxT("JSON files (*.json)|*.json");

   wxFileDialog dialog(this, _("Open a saved wallet"), defaultDir, wxT("wallet.json"), wildcard);
   if (dialog.ShowModal() == wxID_OK)
   {
      const wxString path = dialog.GetPath();
      config->Write("WalletPath", path);
      config->Flush();

      if(state.is_connected) {
         wxCommandEvent event_disconnect(wxEVT_COMMAND_MENU_SELECTED, XRCID("ID_DISCONNECT"));
         ProcessWindowEvent(event_disconnect);
         wxSleep(1);
      }

      wxCommandEvent event_connect(wxEVT_COMMAND_MENU_SELECTED, XRCID("ID_CONNECT"));
      ProcessWindowEvent(event_connect);
   }
}

void GWallet::OnSave(wxCommandEvent& WXUNUSED(event))
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

void GWallet::OnNetwork(wxCommandEvent& WXUNUSED(event))
{
   wxTextEntryDialog dialog(this, _("Enter server"), _("Websocket endpoint"));
   if ( dialog.ShowModal() == wxID_OK )
   {
      const wxString ws_server = dialog.GetValue();
      config->Write("Server", ws_server);
      config->Flush();

      if(state.is_connected) {
         wxCommandEvent event_disconnect(wxEVT_COMMAND_MENU_SELECTED, XRCID("ID_DISCONNECT"));
         ProcessWindowEvent(event_disconnect);
         wxSleep(1);
      }
      wxCommandEvent event_connect(wxEVT_COMMAND_MENU_SELECTED, XRCID("ID_CONNECT"));
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
   ImportKeyDialog importKeyDialog( this );
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
   const auto selected = t_assets->GetCurrentSelection();

   const auto asset_name = strings.assets[selected];
   const auto balance = strings.balances[selected].ToStdString();
   const auto precision = strings.precisions[selected].ToStdString();

   auto pretty_balance = DoPrettyBalance(std::stoi(precision), std::stod(balance));
   t_balance->SetLabel(pretty_balance + " " + asset_name);
   strings.selected_asset = asset_name;
}

void GWallet::DoInitialConfig()
{
   // full size
   //wxTopLevelWindow::Maximize(true);

   wxTopLevelWindow::SetMinSize(wxSize(600, 450));

   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   directory = f.GetPath();

   wxXmlResource::Get()->InitAllHandlers();
   wxXmlResource::Get()->LoadAllFiles(directory + wxT("/resources"));

   const wxIcon application_icon(directory + wxT("/icons/btslogo.png"), wxBITMAP_TYPE_PNG);
   SetIcon(application_icon);

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

   wxString i18n_dir(directory + wxFILE_SEP_PATH + wxT("i18n"));
   int lang;
   if (config->Read("Lang", &lang)) {
      m_locale = new wxLocale(lang);
      m_locale->Init(lang);
   }
   else {
      m_locale = new wxLocale(wxLANGUAGE_DEFAULT);
      m_locale->Init(wxLANGUAGE_DEFAULT);
   }
   m_locale->AddCatalogLookupPathPrefix(i18n_dir);
   m_locale->AddCatalog(wxT("gwallet"));
}

void GWallet::DoAssets(std::string account)
{
   t_assets->Clear();
   strings.assets.Clear();

   int n = 0;

   const auto& account_balances = bitshares.wallet_api_ptr->list_account_balances(account);
   for(auto& account_balance : account_balances) {

      std::string asset_id_string = std::string(object_id_type(account_balance.asset_id));

      const auto& asset_object = bitshares.wallet_api_ptr->get_asset(asset_id_string);
      const auto& asset_symbol = asset_object.symbol;
      const auto& precision = asset_object.precision;
      const auto& amount_value = account_balance.amount.value;

      if(n == 0) {
         auto pretty_balance = DoPrettyBalance(precision, amount_value);
         t_balance->SetLabel(pretty_balance + " " + asset_symbol);
      }
      strings.assets.Add(asset_symbol);
      strings.balances.Add(fc::to_string(amount_value));
      strings.precisions.Add(fc::to_string(precision));
      n++;
   }
   t_assets->Set(strings.assets);
   t_assets->SetSelection(0);
   t_assets->Enable(true);
}

void GWallet::DoAccounts()
{
   const auto& accounts = bitshares.wallet_api_ptr->list_my_accounts();
   int n = 0;

   for(auto& account : accounts) {
      auto name = account.name;
      if(n == 0) strings.first_account_name = name;
      strings.accounts.Add(name);
      n++;
   }
   t_accounts->Set(strings.accounts);
   t_accounts->SetSelection(0);
   t_accounts->Enable(true);
}

void GWallet::DoModes()
{
   Home *home = new Home(this);
   modes.p_home = home;

   Cli *cli = new Cli(this);
   modes.p_cli = cli;
   modes.p_cli->Hide();

   Wallet *wallet = new Wallet(this);
   modes.p_wallet = wallet;
   modes.p_wallet->Hide();

   SendReceive *sendreceive = new SendReceive(this);
   modes.p_sendreceive = sendreceive;
   modes.p_sendreceive->Hide();

   History *history = new History(this);
   modes.p_history = history;
   modes.p_history->Hide();
}

void GWallet::LoadWelcomeWidget()
{
   const wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   const wxString directory(f.GetPath());

   const wxBitmap wizard_icon(directory + wxT("/icons/wizard.png"), wxBITMAP_TYPE_PNG);

   welcome.wizard = new wxWizard(main_panel, ID_WIZARD, _("Welcome to Bitshares G-Wallet"),
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
   modes.p_home->Show(true);
   modes.p_cli->Hide();
   modes.p_sendreceive->Hide();
   modes.p_history->Hide();
   modes.p_wallet->Hide();
 }

void GWallet::OnCommandMode(wxCommandEvent& WXUNUSED(event))
{
   modes.p_home->Hide();
   modes.p_cli->Show(true);
   modes.p_sendreceive->Hide();
   modes.p_history->Hide();
   modes.p_wallet->Hide();
}

void GWallet::OnSendReceiveMode(wxCommandEvent& WXUNUSED(event))
{
   modes.p_home->Hide();
   modes.p_cli->Hide();
   modes.p_sendreceive->Show(true);
   modes.p_history->Hide();
   modes.p_wallet->Hide();
}

void GWallet::OnHistoryMode(wxCommandEvent& WXUNUSED(event))
{
   modes.p_home->Hide();
   modes.p_cli->Hide();
   modes.p_sendreceive->Hide();
   modes.p_history->Show(true);
   modes.p_wallet->Hide();

   wxWindowDisabler disableAll;
   wxBusyInfo wait(_("Please wait, pulling the last history of your account ..."));
   wxTheApp->Yield();

   modes.p_history->DoHistory(strings.first_account_name.ToStdString());
}

void GWallet::OnWalletMode(wxCommandEvent& WXUNUSED(event))
{
   modes.p_home->Hide();
   modes.p_cli->Hide();
   modes.p_sendreceive->Hide();
   modes.p_history->Hide();
   modes.p_wallet->Show(true);
}

void GWallet::CreateEvents()
{
   Connect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnNew));
   Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnOpen));
   Connect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnSave));
   Connect(wxID_NETWORK, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnNetwork));
   Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnQuit));
   Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnAbout));

   Connect(XRCID("m_lang"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnChangeLanguage));
   Connect(XRCID("m_connect"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnConnect));
   Connect(XRCID("m_set_password"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnSetPassword));
   Connect(XRCID("m_lock"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnLock));
   Connect(XRCID("m_unlock"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnUnlock));
   Connect(XRCID("m_import_key"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnImportKey));
   Connect(XRCID("m_disconnectT"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GWallet::OnDisconnect));

   Connect(XRCID("t_connect"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnConnect));
   Connect(XRCID("t_disconnect"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnDisconnect));
   Connect(XRCID("t_lock"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnLock));
   Connect(XRCID("t_unlock"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnUnlock));

   Connect(XRCID("t_home"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnHomeMode));
   Connect(XRCID("t_cli"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnCommandMode));
   Connect(XRCID("t_sendreceive"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnSendReceiveMode));
   Connect(XRCID("t_history"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnHistoryMode));
   Connect(XRCID("t_wallet"), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(GWallet::OnWalletMode));

   Connect(XRCID("t_accounts"), wxEVT_COMBOBOX, wxCommandEventHandler(GWallet::OnChangeAccount));
   Connect(XRCID("t_assets"), wxEVT_COMBOBOX, wxCommandEventHandler(GWallet::OnChangeAsset));

   Connect(XRCID("new_button"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GWallet::OnNew));
   Connect(XRCID("open_button"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GWallet::OnOpen));
   Connect(XRCID("connect_button"), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(GWallet::OnConnect));
}

void GWallet::OnError(wxString msg)
{
   wxMessageDialog dialog(NULL, msg, _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);
   if (dialog.ShowModal() == wxID_OK)
      return;
}

void GWallet::OnChangeLanguage(wxCommandEvent& WXUNUSED(event))
{
   wxArrayInt languageCodes;
   wxArrayString languageNames;

   languageCodes.Add(wxLANGUAGE_CHINESE);
   languageNames.Add(_("Chinese"));
   languageCodes.Add(wxLANGUAGE_SPANISH);
   languageNames.Add(_("Spanish"));
   languageCodes.Add(wxLANGUAGE_ENGLISH);
   languageNames.Add(_("English"));
   int lang = wxGetSingleChoiceIndex(_("Select language:"), _("Language"), languageNames);
   if(lang != -1)
   {
      SelectLanguage(languageCodes[lang]);
   }
}

void GWallet::SelectLanguage(int lang)
{
   delete m_locale;
   m_locale = new wxLocale(lang);
   if(m_locale->Init(lang)) {
      config->Write("Lang", lang);
      config->Flush();

      wxMessageDialog dialog(NULL,
            _("Language changed, however you need to restart G-Wallet for changes to take effect"),
            _("Success"), wxNO_DEFAULT | wxOK | wxICON_INFORMATION);
      if (dialog.ShowModal() == wxID_OK)
         return;
   }
}

std::string GWallet::DoPrettyBalance(int precision, double balance)
{
   const auto divisor = pow(10, precision);
   stringstream pretty_balance;
   pretty_balance << fixed << std::setprecision(precision) << balance/divisor;
   return pretty_balance.str();
}

void GWallet::DoState() {

   menubar = this->GetMenuBar();

   if(state.is_noconfig) {
      menubar->Enable(XRCID("wxID_NEW"), true);
      menubar->Enable(XRCID("wxID_OPEN"), true);
      menubar->Enable(XRCID("wxID_SAVE"), true);
      menubar->Enable(XRCID("wxID_NETWORK"), true);

      menubar->Enable(XRCID("m_connect"), true);
      menubar->Enable(XRCID("m_disconnect"), true);
      menubar->Enable(XRCID("m_set_password"), true);
      menubar->Enable(XRCID("m_lock"), true);
      menubar->Enable(XRCID("m_import"), true);
      menubar->Enable(XRCID("m_import_key"), true);

      toolbar->EnableTool(XRCID("t_connect"), true);
      toolbar->EnableTool(XRCID("t_disconnect"), true);
      toolbar->EnableTool(XRCID("t_lock"), true);
      toolbar->EnableTool(XRCID("t_unlock"), true);
      toolbar->EnableTool(XRCID("t_home"), true);
      toolbar->EnableTool(XRCID("t_cli"), true);
      toolbar->EnableTool(XRCID("t_history"), true);
      toolbar->EnableTool(XRCID("t_sendreceive"), true);
      toolbar->EnableTool(XRCID("t_wallet"), true);

      SetStatusText(_("No Config"), 0);
   }
   else if(state.is_connected) {
      menubar->Enable(XRCID("wxID_NEW"), true);
      menubar->Enable(XRCID("wxID_OPEN"), true);
      menubar->Enable(XRCID("wxID_SAVE"), true);
      menubar->Enable(XRCID("wxID_SAVE"), true);

      menubar->Enable(XRCID("m_connect"), false);
      menubar->Enable(XRCID("m_disconnect"), true);
      menubar->Enable(XRCID("m_setpassword"), false);
      menubar->Enable(XRCID("m_lock"), false);
      menubar->Enable(XRCID("m_unlock"), false);
      menubar->Enable(XRCID("m_import_key"), false);

      toolbar->EnableTool(XRCID("t_connect"), false);
      toolbar->EnableTool(XRCID("t_disconnect"), true);
      toolbar->EnableTool(XRCID("t_lock"), false);
      toolbar->EnableTool(XRCID("t_unlock"), false);
      toolbar->EnableTool(XRCID("t_home"), false);
      toolbar->EnableTool(XRCID("t_cli"), false);
      toolbar->EnableTool(XRCID("t_history"), false);
      toolbar->EnableTool(XRCID("t_sendreceive"), false);
      toolbar->EnableTool(XRCID("t_wallet"), false);

      if(state.is_new) {
         SetStatusText(_("Connected | New"));
         menubar->Enable(XRCID("m_set_password"), true);
      }
      else if(!state.is_account_linked) {
         SetStatusText(_("Connected | New"));
         menubar->Enable(XRCID("m_import_key"), false);
         menubar->Enable(XRCID("m_unlock"), true);
      }
      else if(state.is_locked) {
         SetStatusText(_("Connected | Locked"));

         menubar->Enable(XRCID("m_set_password"), false);
         menubar->Enable(XRCID("m_lock"), false);
         menubar->Enable(XRCID("m_unlock"), true);
         menubar->Enable(XRCID("m_import_key"), false);

         toolbar->EnableTool(XRCID("t_unlock"), true);
         toolbar->EnableTool(XRCID("t_cli"), true);
         toolbar->EnableTool(XRCID("t_home"), true);
      }
      else if(state.is_unlocked) {
         SetStatusText(_("Connected | Unlocked"));

         menubar->Enable(XRCID("m_setpassword"), false);
         menubar->Enable(XRCID("m_lock"), true);
         menubar->Enable(XRCID("m_unlock"), false);
         menubar->Enable(XRCID("m_import_key"), true);

         toolbar->EnableTool(XRCID("t_lock"), true);
         toolbar->EnableTool(XRCID("t_home"), true);
         toolbar->EnableTool(XRCID("t_cli"), true);
         toolbar->EnableTool(XRCID("t_history"), true);
         toolbar->EnableTool(XRCID("t_sendreceive"), true);
         toolbar->EnableTool(XRCID("t_wallet"), true);
      }
      wxString server;
      if(config->Read("Server", &server)) {
         int widths[] = {GetClientSize().x - 25 - GetTextExtent(server).x, -1};
         SetStatusWidths(2, widths);
         SetStatusText(server, 1);
      }
   }
   else if(!state.is_connected) {

      toolbar->EnableTool(XRCID("t_accounts"), false);
      toolbar->EnableTool(XRCID("t_assets"), false);

      toolbar->EnableTool(XRCID("t_home"), false);
      toolbar->EnableTool(XRCID("t_cli"), false);
      toolbar->EnableTool(XRCID("t_history"), false);
      toolbar->EnableTool(XRCID("t_sendreceive"), false);
      toolbar->EnableTool(XRCID("t_wallet"), false);
      toolbar->EnableTool(XRCID("t_connect"), true);
      toolbar->EnableTool(XRCID("t_disconnect"), false);
      toolbar->EnableTool(XRCID("t_lock"), false);
      toolbar->EnableTool(XRCID("t_unlock"), false);

      menubar->Enable(XRCID("m_connect"), true);
      menubar->Enable(XRCID("m_disconnect"), false);
      menubar->Enable(XRCID("m_setpassword"), false);
      menubar->Enable(XRCID("m_lock"), false);
      menubar->Enable(XRCID("m_unlock"), false);
      menubar->Enable(XRCID("m_import_key"), false);

      SetStatusText(_("Disconnected"), 0);
   }
}

