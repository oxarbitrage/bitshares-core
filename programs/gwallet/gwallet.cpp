#ifndef GWALLET_H
#define GWALLET_H
#include "include/gwallet.hpp"
#endif
#include "include/dialogs/importkey.hpp"
#include "include/modes/home.hpp"
#include "include/modes/wallet.hpp"
#include "include/modes/cli.hpp"
#include "include/modes/sendreceive.hpp"
#include "include/modes/history.hpp"

#include <wx/stattext.h>
#include <wx/statline.h>
#include <wx/busyinfo.h>
#include <wx/listctrl.h>
#include <wx/combobox.h>
#include <wx/notebook.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/hyperlink.h>

GWallet::GWallet(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 1000))
{
   // lets go full size
   wxTopLevelWindow::Maximize(true);

   // set a windows min size
   wxTopLevelWindow::SetMinSize(wxSize(500, 600));

   // current path
   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   directory = f.GetPath();

   // logo
   wxIcon application_icon(directory + wxT("/icons/btslogo.png"), wxBITMAP_TYPE_PNG);
   SetIcon(application_icon);

   CreateMenu();
   CreateEvents();
   CreateTool();

   // main window
   panel = new wxPanel(this, wxID_ANY);

   mainSizer = new wxBoxSizer(wxVERTICAL);
   panel->SetSizer(mainSizer);

   infoSizer = new wxBoxSizer(wxHORIZONTAL);
   mainSizer->Add(infoSizer, 0, wxGROW|wxALL);

   mainMsg = new wxStaticText(panel, wxID_STATIC, wxT("G-Wallet Offline"), wxDefaultPosition, wxDefaultSize);
   wxFont font = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
   mainMsg->SetFont(font);
   infoSizer->Add(mainMsg, 0, wxALL|wxALIGN_LEFT, 5);

   infoSizer->AddStretchSpacer();

   balanceMsg = new wxStaticText(panel, wxID_STATIC, wxT("0.00 BTS"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
   balanceMsg->SetFont(font);
   infoSizer->Add(balanceMsg, 0, wxALL|wxALIGN_RIGHT, 5);

   // status Bar
   CreateStatusBar(2);
   SetStatusText(wxT("Disconnected"), 0);

   // the config file
   config = new wxConfig(wxT("GWallet"));
   wxString str;
   if ( config->Read("wallet-file", &str) ) {
      wdump((str.ToStdString()));
   }
   else {
      LoadWelcomeWidget();
   }
   delete config;

   Centre();
}
void GWallet::OnQuit(wxCommandEvent & WXUNUSED(event))
{
   Close(true);
}

void GWallet::OnConnect(wxCommandEvent & WXUNUSED(event))
{
   wxWindowDisabler disableAll;
   wxBusyInfo wait(wxT("Please wait, connecting ..."));
   wxTheApp->Yield();

   //wallet.connect("wss://api.bitshares.bhuz.info/ws");
   wallet.Connect("wss://api.bitshares-kibana.info/ws");

   SetStatusText(wxT("Connected"), 0);
   SetStatusText(wxT("wss://api.bitshares-kibana.info/ws"), 1);


   wallet_m->Enable(ID_CONNECT, false);
   wallet_m->Enable(ID_DISCONNECT, true);

   itemToolBar->EnableTool(ID_ICON_CONNECT, false);
   itemToolBar->EnableTool(ID_ICON_DISCONNECT, true);

   itemToolBar->EnableTool(ID_ICON_HOME, true);
   itemToolBar->EnableTool(ID_ICON_COMMAND, true);
   itemToolBar->EnableTool(ID_ICON_HISTORY, true);
   itemToolBar->EnableTool(ID_ICON_SENDRECEIVE, true);
   itemToolBar->EnableTool(ID_ICON_WALLET, true);

   if( wallet.wallet_api_ptr->is_new() )
   {
      SetStatusText(wxT("Connected | New"));
      wallet_m->Enable(ID_SETPASSWORD, true);
      mainMsg->SetLabel("G-Wallet New");
   }
   else
   {
      mainMsg->SetLabel("G-Wallet Ready");

      if( wallet.wallet_api_ptr->is_locked() )
      {
         SetStatusText(wxT("Connected | Locked"));
         wallet_m->Enable(ID_UNLOCK, true);
         itemToolBar->EnableTool(ID_ICON_UNLOCK, true);
      }
      else
      {
         SetStatusText(wxT("Connected | Unlocked"));
         itemToolBar->EnableTool(ID_ICON_LOCK, true);
      }

      // accounts
      DoAccounts();

      //assets and balances
      DoAssets(first_account_name);


      Home *home = new Home(this);
      home->CreateControls();
      home->CreateEvents();

      Cli *cli = new Cli(this);
      cli->CreateControls();
      cli->CreateEvents();

      SendReceive *sendreceive = new SendReceive(this);
      sendreceive->CreateControls();
      sendreceive->CreateEvents();

      Wallet *wallet = new Wallet(this);
      wallet->CreateControls();
      wallet->CreateEvents();

      History *history = new History(this);
      history->CreateControls();
      history->CreateEvents();
      history->DoHistory(first_account_name);
   }
}

void GWallet::OnDisconnect(wxCommandEvent & WXUNUSED(event))
{
   try {
      wallet.Disconnect();
   }
   catch( const fc::exception& e )
   {
      OnError(wxT("Some problem when disconnecting, try again ..."));
   }
   SetStatusText(wxT("Disconnected"));
   itemToolBar->EnableTool(ID_ICON_CONNECT, true);
   itemToolBar->EnableTool(ID_ICON_DISCONNECT, false);
}

void GWallet::OnSetPassword(wxCommandEvent & WXUNUSED(event))
{
    wxPasswordEntryDialog dialog(this, "Enter password");
    if ( dialog.ShowModal() == wxID_OK )
    {
       wxString value = dialog.GetValue();

       wallet.wallet_api->set_password(value.ToStdString());
       SetStatusText(wxT("Connected | Locked"));

       itemToolBar->EnableTool(ID_ICON_UNLOCK, true);
    }
}
void GWallet::OnAbout(wxCommandEvent & WXUNUSED(event))
{
   wxString msg;
   msg.Printf(wxT("Secure and simple bitshares wallet build with %s"), wxVERSION_STRING);
   wxMessageBox(msg, wxT("About G-Wallet"), wxOK | wxICON_INFORMATION, this);
}

void GWallet::OnLock(wxCommandEvent & WXUNUSED(event))
{
   try
   {
      wallet.wallet_api->lock();
   }
   catch( const fc::exception& e )
   {
      OnError(wxT("Some problem when locking, try again ..."));
   }
   SetStatusText(wxT("Connected | Locked"));
   wallet_m->Enable(ID_UNLOCK, true);
   wallet_m->Enable(ID_LOCK, false);
   itemToolBar->EnableTool(ID_ICON_LOCK, false);
   itemToolBar->EnableTool(ID_ICON_UNLOCK, true);
}

void GWallet::OnUnlock(wxCommandEvent & WXUNUSED(event))
{
   wxPasswordEntryDialog dialog(this, "Enter password");
   if ( dialog.ShowModal() == wxID_OK )
   {
      wxString value = dialog.GetValue();

      try {
         wallet.wallet_api->unlock(value.ToStdString());
      }
      catch( const fc::exception& e )
      {
         OnError(wxT("Password is incorrect, please try again."));
      }

      SetStatusText(wxT("Connected | Unlocked"));
      wallet_m->Enable(ID_SETPASSWORD, false);
      wallet_m->Enable(ID_UNLOCK, false);
      wallet_m->Enable(ID_LOCK, true);
      wallet_m->Enable(ID_CONNECT, false);
      wallet_m->Enable(ID_DISCONNECT, true);
      wallet_m->Enable(ID_IMPORTKEY, true);

      itemToolBar->EnableTool(ID_ICON_CONNECT, false);
      itemToolBar->EnableTool(ID_ICON_DISCONNECT, true);
      itemToolBar->EnableTool(ID_ICON_LOCK, true);
      itemToolBar->EnableTool(ID_ICON_UNLOCK, false);
   }
}
void GWallet::OnImportKey(wxCommandEvent & WXUNUSED(event))
{
   ImportKeyDialog importKeyDialog( this, -1, _("Import key"), wxDefaultPosition, wxDefaultSize );
}

void GWallet::OnChangeAccount(wxCommandEvent & WXUNUSED(event))
{
   wxWindowDisabler disableAll;
   wxBusyInfo wait(wxT("Please wait, switching accounts ..."));
   wxTheApp->Yield();

   auto selected = combo_accounts->GetCurrentSelection();

   auto account_name = strings_accounts[selected];

   DoAssets(account_name.ToStdString());

   // same as bel;ow, need to change history mode here
   //DoHistory(account_name.ToStdString());

   // dont delete until fix with member function or soemthing
   //from->SetSelection(selected);
   //asset->SetSelection(0);
}

void GWallet::OnChangeAsset(wxCommandEvent & WXUNUSED(event))
{
   auto selected = combo_assets->GetCurrentSelection();

   auto asset_name = strings_assets[selected];
   auto balance = strings_balances[selected];
   auto precision = strings_precisions[selected];

   auto dividend = pow(10, std::stoi(precision.ToStdString()));

   stringstream pretty_balance;
   pretty_balance << fixed << std::setprecision(std::stoi(precision.ToStdString()))  << std::stod(balance.ToStdString())/dividend;

   balanceMsg->SetLabel(pretty_balance.str() + " " + asset_name);

   // dont delete until fix with member function or soemthing
   //asset->SetSelection(selected);

   infoSizer->Layout();
}

void GWallet::DoAssets(std::string account)
{
   combo_assets->Clear();
   strings_assets.Clear();

   int n = 0;
   std::string first_asset;
   std::string first_balance;
   std::string first_precision;

   auto my_balances = wallet.wallet_api_ptr->list_account_balances(account);
   for( auto& mb : my_balances ) {

      std::string asset_id = fc::to_string(mb.asset_id.space_id)
                             + "." + fc::to_string(mb.asset_id.type_id)
                             + "." + fc::to_string(mb.asset_id.instance.value);

      auto asset = wallet.wallet_api_ptr->get_asset(asset_id).symbol;

      if(n == 0) {
         first_asset = asset;
         first_balance = fc::to_string(mb.amount.value);
         first_precision = wallet.wallet_api_ptr->get_asset(asset_id).precision;

         // insert balance
         auto dividend = pow(10, wallet.wallet_api_ptr->get_asset(asset_id).precision);
         stringstream pretty_balance;
         pretty_balance << fixed << std::setprecision(wallet.wallet_api_ptr->get_asset(asset_id).precision)  << mb.amount.value/dividend;
         balanceMsg->SetLabel(pretty_balance.str() + " " + first_asset);
         infoSizer->Layout();
      }
      strings_assets.Add(asset);
      strings_balances.Add(fc::to_string(mb.amount.value));
      strings_precisions.Add(fc::to_string(wallet.wallet_api_ptr->get_asset(asset_id).precision));
      n++;
   }
   combo_assets->Set(strings_assets);
   combo_assets->SetSelection(0);
   combo_assets->Enable(true);
}

void GWallet::DoAccounts()
{
   auto my_accounts = wallet.wallet_api_ptr->list_my_accounts();
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

void GWallet::OnNew(wxCommandEvent & WXUNUSED(event))
{
   LoadWelcomeWidget();
}

void GWallet::LoadWelcomeWidget()
{
   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   wxString directory(f.GetPath());

   wxBitmap wizard_icon(directory + wxT("/icons/wizard.png"), wxBITMAP_TYPE_PNG);

   wizard = new wxWizard( 	panel, ID_WIZARD, wxT("Welcome to Bitshares G-Wallet"), wizard_icon, wxDefaultPosition, wxDEFAULT_DIALOG_STYLE);

   page1 = new Welcome1(wizard);
   page2 = new Welcome2(wizard);
   page3 = new Welcome3(wizard);
   page4 = new Welcome4(wizard);

   (*page1).Chain(page2).Chain(page3).Chain(page4);

   wizard->RunWizard(page1);
}

void GWallet::OnHomeMode(wxCommandEvent & WXUNUSED(event))
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

void GWallet::OnCommandMode(wxCommandEvent & WXUNUSED(event))
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

void GWallet::OnTransferMode(wxCommandEvent & WXUNUSED(event))
{
   //m_timer.Stop();
   mainSizer->Show(sizerTransferMode, true, true);
   mainSizer->Hide(sizerHomeMode, true);
   mainSizer->Hide(sizerCommandMode, true);
   mainSizer->Hide(sizerHistoryMode, true);
   mainSizer->Hide(sizerWalletMode, true);
   mainSizer->Layout();
}

void GWallet::OnHistoryMode(wxCommandEvent & WXUNUSED(event))
{
   //m_timer.Stop();
   mainSizer->Show(sizerHistoryMode, true, true);
   mainSizer->Hide(sizerHomeMode, true);
   mainSizer->Hide(sizerCommandMode, true);
   mainSizer->Hide(sizerTransferMode, true);
   mainSizer->Hide(sizerWalletMode, true);
   mainSizer->Layout();
}

void GWallet::OnWalletMode(wxCommandEvent & WXUNUSED(event))
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
   wxMessageDialog dialog( NULL, msg, wxT("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);
   if ( dialog.ShowModal() == wxID_OK )
      return;
}
