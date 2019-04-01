#pragma once

#include "bitshares.hpp"
#include "welcome.hpp"

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/wizard.h>
#include <wx/config.h>
#include <wx/grid.h>

class Wallet;
class History;
class SendReceive;

class GWallet : public wxFrame
{
public:
   GWallet(const wxString& title);

   void OnError(wxString msg);

   wxConfig* config;
   wxString directory;

   wxPanel* panel;
   wxBoxSizer* mainSizer;
   wxBoxSizer* infoSizer;
   wxBoxSizer* sizerCommandMode;
   wxBoxSizer* sizerHistoryMode;
   wxBoxSizer* sizerTransferMode;
   wxBoxSizer* sizerHomeMode;
   wxBoxSizer* sizerWalletMode;

   wxStaticText* mainMsg;
   wxStaticText* balanceMsg;

   Bitshares bitshares;

   wxComboBox* combo_accounts;
   wxArrayString strings_accounts;
   wxComboBox* combo_assets;
   wxArrayString strings_assets;
   wxArrayString strings_balances;
   wxArrayString strings_precisions;
   std::string first_account_name;
   wxString selected_account;
   wxString selected_asset;

   wxWizard* wizard;
   wxWizardPageSimple* page1;
   Welcome2* page2;
   Welcome3* page3;
   Welcome4* page4;

private:

   void OnNew(wxCommandEvent& event);
   void OnOpen(wxCommandEvent& event);
   void OnSave(wxCommandEvent& event);
   void OnNetwork(wxCommandEvent& event);
   void OnQuit(wxCommandEvent& event);
   void OnAbout(wxCommandEvent& event);

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

   void DoAssets(std::string account);
   void DoAccounts();

   void LoadWelcomeWidget();

   wxMenuBar* menubar;
   wxMenu* file;
   wxMenu* wallet_m;
   wxMenu* help;
   wxToolBar* itemToolBar;

   bool is_connected = false;

   Wallet* p_wallet;
   History* p_history;
   SendReceive* p_sendreceive;
};

const int ID_CONNECT = 105;
const int ID_DISCONNECT = 106;
const int ID_SETPASSWORD = 107;
const int ID_LOCK = 109;
const int ID_UNLOCK = 110;
const int ID_IMPORTKEY = 111;

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
