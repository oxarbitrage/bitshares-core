#pragma once

#include "bitshares.hpp"

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/wizard.h>
#include <wx/config.h>
#include <wx/grid.h>
#include <wx/xrc/xmlres.h>

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
   Bitshares bitshares;
   Modes modes;
   Strings strings;
   Welcome welcome;

protected:

   wxMenuBar* menubar;
   wxToolBar* toolbar;

   wxComboBox* t_accounts;
   wxComboBox* t_assets;
   wxStaticText* t_balance;

   wxPanel* main_panel;


private:

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GWallet"), wxT("wxFrame"));
      toolbar = XRCCTRL(*this,"toolbar",wxToolBar);
      t_accounts = XRCCTRL(*this,"t_accounts",wxComboBox);
      t_assets = XRCCTRL(*this,"t_assets",wxComboBox);
      t_balance = XRCCTRL(*this,"t_balance",wxStaticText);
      main_panel = XRCCTRL(*this,"main_panel",wxPanel);
   }

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
   void OnSendReceiveMode(wxCommandEvent& event);
   void OnHistoryMode(wxCommandEvent& event);
   void OnWalletMode(wxCommandEvent& event);

   void OnChangeAccount(wxCommandEvent& event);
   void OnChangeAsset(wxCommandEvent& event);

   void CreateEvents();

   void DoInitialConfig();

   void LoadWelcomeWidget();

   void SelectLanguage(int lang);
   wxLocale* m_locale;
};

