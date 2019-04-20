#pragma once

#include "bitshares.hpp"

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/wizard.h>
#include <wx/config.h>
#include <wx/grid.h>

class Home;
class Cli;
class SendReceive;
class History;
class Wallet;

class Welcome2;
class Welcome3;
class Welcome4;

struct States {
   bool is_noconfig = false;
   bool is_connected = false;
   bool is_new = false;
   bool is_locked = false;
   bool is_unlocked = false;
   bool is_account_linked = false;
   bool modes_created = false;
};

struct Sizers {
   wxBoxSizer* main;
   wxBoxSizer* info;
   wxBoxSizer* cli;
   wxBoxSizer* history;
   wxBoxSizer* transfer;
   wxBoxSizer* home;
   wxBoxSizer* wallet;
};

struct Modes {
   Home* p_home;
   Cli* p_cli;
   SendReceive* p_sendreceive;
   History* p_history;
   Wallet* p_wallet;
};

struct Strings {
   wxStaticText* main;
   wxStaticText* balance;

   wxComboBox* combo_accounts;
   wxComboBox* combo_assets;

   wxArrayString accounts;
   wxArrayString assets;
   wxArrayString balances;
   wxArrayString precisions;

   wxString first_account_name;
   wxString selected_account;
   wxString selected_asset;
};

struct Welcome {
   wxWizard* wizard;
   wxWizardPageSimple* page1;
   Welcome2* page2;
   Welcome3* page3;
   Welcome4* page4;
};

struct Navigation {
   wxToolBar* itemToolBar;
   wxMenuBar* menubar;
   wxMenu* wallet;
   wxMenu* file;
   wxMenu* help;
};

class GWallet : public wxFrame
{
public:
   GWallet(const wxString& title);

   void DoState();
   void OnError(wxString msg);
   void DoAssets(std::string account);
   void DoAccounts();
   void DoModes();

   std::string DoPrettyBalance(int precision, double balance);

   wxConfig* config;
   wxString directory;
   States state;
   wxPanel* panel;
   Sizers sizers;
   Bitshares bitshares;
   Navigation navi;
   Modes modes;
   Strings strings;
   Welcome welcome;

private:

   void OnNew(wxCommandEvent& event);
   void OnOpen(wxCommandEvent& event);
   void OnSave(wxCommandEvent& event);
   void OnNetwork(wxCommandEvent& event);
   void OnQuit(wxCommandEvent& event);
   void OnAbout(wxCommandEvent& event);
   void OnChangeLanguage(wxCommandEvent& event);

   void OnConnect(wxCommandEvent& event);
   void OnDisconnect(wxCommandEvent& event);
   void OnSetPassword(wxCommandEvent& event);
   void OnLock(wxCommandEvent& event);
   void OnUnlock(wxCommandEvent& event);
   void OnImportKey(wxCommandEvent& event);

   void OnHomeMode(wxCommandEvent& event);
   void OnCommandMode(wxCommandEvent& event);
   void OnTransferMode(wxCommandEvent& event);
   void OnHistoryMode(wxCommandEvent& event);
   void OnWalletMode(wxCommandEvent& event);

   void OnChangeAccount(wxCommandEvent& event);
   void OnChangeAsset(wxCommandEvent& event);

   void CreateMenu();
   void CreateTool();
   void CreateEvents();
   void CreateInfo();
   void CreateMain();

   void DoInitialConfig();

   void LoadWelcomeWidget();

   void SelectLanguage(int lang);
   wxLocale* m_locale;
};

const int ID_CONNECT = 105;
const int ID_DISCONNECT = 106;
const int ID_SETPASSWORD = 107;
const int ID_LOCK = 109;
const int ID_UNLOCK = 110;
const int ID_IMPORTKEY = 111;
const int ID_LANG = 112;

const int ID_ICON_CONNECT = 113;
const int ID_ICON_DISCONNECT = 114;
const int ID_ICON_LOCK = 115;
const int ID_ICON_UNLOCK = 116;

const int ID_COMBO_ACCOUNTS = 122;
const int ID_COMBO_ASSETS = 123;

const int ID_ICON_HOME = 124;
const int ID_ICON_COMMAND = 125;
const int ID_ICON_SENDRECEIVE = 126;
const int ID_ICON_HISTORY = 127;
const int ID_ICON_WALLET = 128;

const int ID_VIEW_WITNESSES = 4225;
const int ID_VIEW_COMMITTEE = 4226;
