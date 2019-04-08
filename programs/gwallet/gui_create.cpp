#include "include/gwallet.hpp"

void GWallet::CreateMenu()
{
   navi.menubar = new wxMenuBar;

   navi.file = new wxMenu;
   navi.file->Append(wxID_NEW, wxT("&New\tCtrl+N"));
   navi.file->Append(wxID_OPEN, wxT("&Open\tCtrl+O"));
   navi.file->Append(wxID_SAVE, wxT("&Save\tCtrl+S"));
   navi.file->Append(wxID_NETWORK, wxT("Conn&ection\tCtrl+E"));
   navi.file->AppendSeparator();
   navi.file->Append(wxID_EXIT, wxT("&Quit\tCtrl+Q"));
   navi.menubar->Append(navi.file, wxT("&File"));

   navi.wallet = new wxMenu;
   navi.wallet->Append(ID_CONNECT, wxT("&Connect\tCtrl+C"));
   navi.wallet->Append(ID_DISCONNECT, wxT("&Disconnect\tCtrl+D"));
   navi.wallet->AppendSeparator();
   navi.wallet->Append(ID_SETPASSWORD, wxT("&Set password\tCtrl+P"));
   navi.wallet->Append(ID_LOCK, wxT("&Lock\tCtrl+L"));
   navi.wallet->Append(ID_UNLOCK, wxT("&Unlock\tCtrl+U"));
   navi.wallet->Append(ID_IMPORTKEY, wxT("&Import Key\tCtrl+K"));
   navi.menubar->Append(navi.wallet, wxT("&Wallet"));

   navi.help = new wxMenu;
   navi.help->Append(wxID_ABOUT, wxT("&About"));
   navi.menubar->Append(navi.help, wxT("&Help"));

   SetMenuBar(navi.menubar);
}

void GWallet::CreateTool()
{
   navi.itemToolBar = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL|wxWIZARD_VALIGN_BOTTOM, wxID_ANY );

   wxImage::AddHandler(new wxPNGHandler);
   wxBitmap connect_icon(directory + wxT("/icons/network-transmit.png"), wxBITMAP_TYPE_PNG);
   wxBitmap disconnect_icon(directory + wxT("/icons/network-offline.png"), wxBITMAP_TYPE_PNG);
   wxBitmap lock_icon(directory + wxT("/icons/object-locked.png"), wxBITMAP_TYPE_PNG);
   wxBitmap unlock_icon(directory + wxT("/icons/object-unlocked.png"), wxBITMAP_TYPE_PNG);

   wxBitmap home_icon(directory + wxT("/icons/go-home-5.png"), wxBITMAP_TYPE_PNG);
   wxBitmap command_icon(directory + wxT("/icons/utilities-terminal-6.png"), wxBITMAP_TYPE_PNG);
   wxBitmap transfer_icon(directory + wxT("/icons/emblem-money.png"), wxBITMAP_TYPE_PNG);
   wxBitmap history_icon(directory + wxT("/icons/x-office-spreadsheet-2.png"), wxBITMAP_TYPE_PNG);
   wxBitmap wallet_icon(directory + wxT("/icons/wallet-open.png"), wxBITMAP_TYPE_PNG);

   navi.itemToolBar->AddTool(ID_ICON_CONNECT, wxT("Connect"), connect_icon, wxT("Connect"));
   navi.itemToolBar->EnableTool(ID_ICON_CONNECT, true);
   navi.itemToolBar->AddTool(ID_ICON_DISCONNECT, wxT("Disconnect"), disconnect_icon, wxT("Disconnect"));
   navi.itemToolBar->EnableTool(ID_ICON_DISCONNECT, false);
   navi.itemToolBar->AddTool(ID_ICON_LOCK, wxT("Lock"), lock_icon, wxT("Lock"));
   navi.itemToolBar->EnableTool(ID_ICON_LOCK, false);
   navi.itemToolBar->AddTool(ID_ICON_UNLOCK, wxT("Unlock"), unlock_icon, wxT("Unlock"));
   navi.itemToolBar->EnableTool(ID_ICON_UNLOCK, false);

   navi.itemToolBar->AddSeparator();

   navi.itemToolBar->AddTool(ID_ICON_HOME, wxT("HOME"), home_icon, wxT("Home"));
   navi.itemToolBar->EnableTool(ID_ICON_HOME, false);
   navi.itemToolBar->AddTool(ID_ICON_COMMAND, wxT("Command Mode"), command_icon, wxT("Console"));
   navi.itemToolBar->EnableTool(ID_ICON_COMMAND, false);
   navi.itemToolBar->AddTool(ID_ICON_SENDRECEIVE, wxT("Send/Receive Mode"), transfer_icon, wxT("Send/Receive"));
   navi.itemToolBar->EnableTool(ID_ICON_SENDRECEIVE, false);
   navi.itemToolBar->AddTool(ID_ICON_HISTORY, wxT("Account History Mode"), history_icon, wxT("Account History"));
   navi.itemToolBar->EnableTool(ID_ICON_HISTORY, false);
   navi.itemToolBar->AddTool(ID_ICON_WALLET, wxT("Wallet Mode"), wallet_icon, wxT("Wallet"));
   navi.itemToolBar->EnableTool(ID_ICON_WALLET, false);

   navi.itemToolBar->AddStretchableSpace();

   wxArrayString itemComboBoxStrings;
   strings.combo_accounts = new wxComboBox( navi.itemToolBar, ID_COMBO_ACCOUNTS, wxT("MY ACCOUNTS"),
           wxDefaultPosition, wxDefaultSize, itemComboBoxStrings, wxCB_DROPDOWN );
   navi.itemToolBar->AddControl(strings.combo_accounts, wxT("MY ACCOUNTS"));
   strings.combo_accounts->Enable(false);

   navi.itemToolBar->AddSeparator();

   strings.combo_assets = new wxComboBox( navi.itemToolBar, ID_COMBO_ASSETS, wxT("MY ASSETS"),
           wxDefaultPosition, wxDefaultSize, itemComboBoxStrings, wxCB_DROPDOWN );
   navi.itemToolBar->AddControl(strings.combo_assets, wxT("MY ASSETS"));
   strings.combo_assets->Enable(false);

   navi.itemToolBar->Realize();
   this->SetToolBar(navi.itemToolBar);
}

void GWallet::DoState() {

   /*
   wdump((is_noconfig));
   wdump((is_connected));
   wdump((is_locked));
   wdump((is_unlocked));
   wdump((is_new));
   wdump((is_account_linked));
   */

   if(state.is_noconfig) {
      navi.file->Enable(wxID_NEW, true);
      navi.file->Enable(wxID_OPEN, true);
      navi.file->Enable(wxID_SAVE, false);
      navi.file->Enable(wxID_NETWORK, false);

      navi.wallet->Enable(ID_CONNECT, false);
      navi.wallet->Enable(ID_DISCONNECT, false);
      navi.wallet->Enable(ID_SETPASSWORD, false);
      navi.wallet->Enable(ID_LOCK, false);
      navi.wallet->Enable(ID_UNLOCK, false);
      navi.wallet->Enable(ID_IMPORTKEY, false);

      navi.itemToolBar->EnableTool(ID_ICON_CONNECT, false);
      navi.itemToolBar->EnableTool(ID_ICON_DISCONNECT, false);
      navi.itemToolBar->EnableTool(ID_ICON_LOCK, false);
      navi.itemToolBar->EnableTool(ID_ICON_UNLOCK, false);
      navi.itemToolBar->EnableTool(ID_ICON_HOME, false);
      navi.itemToolBar->EnableTool(ID_ICON_COMMAND, false);
      navi.itemToolBar->EnableTool(ID_ICON_HISTORY, false);
      navi.itemToolBar->EnableTool(ID_ICON_SENDRECEIVE, false);
      navi.itemToolBar->EnableTool(ID_ICON_WALLET, false);

      SetStatusText(_("No Config"), 0);
   }
   else if(state.is_connected) {

      navi.file->Enable(wxID_NEW, true);
      navi.file->Enable(wxID_OPEN, true);
      navi.file->Enable(wxID_SAVE, true);
      navi.file->Enable(wxID_NETWORK, true);

      navi.wallet->Enable(ID_CONNECT, false);
      navi.wallet->Enable(ID_DISCONNECT, true);
      navi.wallet->Enable(ID_SETPASSWORD, false);
      navi.wallet->Enable(ID_LOCK, false);
      navi.wallet->Enable(ID_UNLOCK, false);
      navi.wallet->Enable(ID_IMPORTKEY, false);

      navi.itemToolBar->EnableTool(ID_ICON_CONNECT, false);
      navi.itemToolBar->EnableTool(ID_ICON_DISCONNECT, true);
      navi.itemToolBar->EnableTool(ID_ICON_LOCK, false);
      navi.itemToolBar->EnableTool(ID_ICON_UNLOCK, false);

      navi.itemToolBar->EnableTool(ID_ICON_HOME, false);
      navi.itemToolBar->EnableTool(ID_ICON_COMMAND, false);
      navi.itemToolBar->EnableTool(ID_ICON_HISTORY, false);
      navi.itemToolBar->EnableTool(ID_ICON_SENDRECEIVE, false);
      navi.itemToolBar->EnableTool(ID_ICON_WALLET, false);

      if(state.is_new) {
         SetStatusText(_("Connected | New"));
         navi.wallet->Enable(ID_SETPASSWORD, true);
         strings.main->SetLabel(_("G-Wallet New"));
      }
      else if(!state.is_account_linked) {
         SetStatusText(_("Connected | New"));
         navi.wallet->Enable(ID_IMPORTKEY, false);
         navi.wallet->Enable(ID_UNLOCK, true);
         strings.main->SetLabel(_("G-Wallet New"));
      }
      else if(state.is_locked) {
         SetStatusText(_("Connected | Locked"));

         navi.wallet->Enable(ID_SETPASSWORD, false);
         navi.wallet->Enable(ID_LOCK, false);
         navi.wallet->Enable(ID_UNLOCK, true);
         navi.wallet->Enable(ID_IMPORTKEY, false);

         navi.itemToolBar->EnableTool(ID_ICON_UNLOCK, true);
         navi.itemToolBar->EnableTool(ID_ICON_COMMAND, true);
         navi.itemToolBar->EnableTool(ID_ICON_HOME, true);
         strings.main->SetLabel(_("G-Wallet Ready"));
      }
      else if(state.is_unlocked) {
         SetStatusText(_("Connected | Unlocked"));

         navi.wallet->Enable(ID_SETPASSWORD, false);
         navi.wallet->Enable(ID_LOCK, true);
         navi.wallet->Enable(ID_UNLOCK, false);
         navi.wallet->Enable(ID_IMPORTKEY, true);

         navi.itemToolBar->EnableTool(ID_ICON_LOCK, true);
         navi.itemToolBar->EnableTool(ID_ICON_HOME, true);
         navi.itemToolBar->EnableTool(ID_ICON_COMMAND, true);
         navi.itemToolBar->EnableTool(ID_ICON_HISTORY, true);
         navi.itemToolBar->EnableTool(ID_ICON_SENDRECEIVE, true);
         navi.itemToolBar->EnableTool(ID_ICON_WALLET, true);
         strings.main->SetLabel(_("G-Wallet Ready"));
      }

      wxString server;
      if(config->Read("Server", &server)) {
         int widths[] = {GetClientSize().x - 25 - GetTextExtent(server).x, -1};
         SetStatusWidths(2, widths);
         SetStatusText(server, 1);
      }
   }
   else if(!state.is_connected) {

      strings.combo_accounts->Enable(false);
      strings.combo_assets->Enable(false);

      navi.itemToolBar->EnableTool(ID_ICON_HOME, false);
      navi.itemToolBar->EnableTool(ID_ICON_COMMAND, false);
      navi.itemToolBar->EnableTool(ID_ICON_HISTORY, false);
      navi.itemToolBar->EnableTool(ID_ICON_SENDRECEIVE, false);
      navi.itemToolBar->EnableTool(ID_ICON_WALLET, false);

      navi.itemToolBar->EnableTool(ID_ICON_CONNECT, true);
      navi.itemToolBar->EnableTool(ID_ICON_DISCONNECT, false);
      navi.itemToolBar->EnableTool(ID_ICON_LOCK, false);
      navi.itemToolBar->EnableTool(ID_ICON_UNLOCK, false);

      navi.wallet->Enable(ID_CONNECT, true);
      navi.wallet->Enable(ID_DISCONNECT, false);
      navi.wallet->Enable(ID_SETPASSWORD, false);
      navi.wallet->Enable(ID_LOCK, false);
      navi.wallet->Enable(ID_UNLOCK, false);
      navi.wallet->Enable(ID_IMPORTKEY, false);

      SetStatusText(_("Disconnected"), 0);
   }
}

void GWallet::CreateInfo()
{
   sizers.info = new wxBoxSizer(wxHORIZONTAL);
   sizers.main->Add(sizers.info, 0, wxGROW|wxALL);

   wxFont font = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

   strings.main = new wxStaticText(panel, wxID_STATIC, wxT("G-Wallet Offline"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
   strings.main->SetFont(font);
   sizers.info->Add(strings.main, 0, wxALL|wxALIGN_LEFT, 5);

   sizers.info->AddStretchSpacer();

   strings.balance = new wxStaticText(panel, wxID_STATIC, wxT("0.00 BTS"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
   strings.balance->SetFont(font);
   sizers.info->Add(strings.balance, 0, wxALL|wxALIGN_RIGHT, 5);
}

void GWallet::CreateMain()
{
   panel = new wxPanel(this, wxID_ANY);
   sizers.main = new wxBoxSizer(wxVERTICAL);
   panel->SetSizer(sizers.main);
}
