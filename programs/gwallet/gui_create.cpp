#include "include/gwallet.hpp"

void GWallet::CreateMenu()
{
   menubar = new wxMenuBar;

   file = new wxMenu;
   file->Append(wxID_NEW, wxT("&New\tCtrl+N"));
   file->Append(wxID_OPEN, wxT("&Open\tCtrl+O"));
   file->Append(wxID_SAVE, wxT("&Save\tCtrl+S"));
   file->Append(wxID_NETWORK, wxT("Conn&ection\tCtrl+E"));
   file->AppendSeparator();
   file->Append(wxID_EXIT, wxT("&Quit\tCtrl+Q"));
   menubar->Append(file, wxT("&File"));

   wallet_m = new wxMenu;
   wallet_m->Append(ID_CONNECT, wxT("&Connect\tCtrl+C"));
   wallet_m->Append(ID_DISCONNECT, wxT("&Disconnect\tCtrl+D"));
   wallet_m->AppendSeparator();
   wallet_m->Append(ID_SETPASSWORD, wxT("&Set password\tCtrl+P"));
   wallet_m->Append(ID_LOCK, wxT("&Lock\tCtrl+L"));
   wallet_m->Append(ID_UNLOCK, wxT("&Unlock\tCtrl+U"));
   wallet_m->Append(ID_IMPORTKEY, wxT("&Import Key\tCtrl+K"));
   menubar->Append(wallet_m, wxT("&Wallet"));

   help = new wxMenu;
   help->Append(wxID_ABOUT, wxT("&About"));
   menubar->Append(help, wxT("&Help"));

   SetMenuBar(menubar);
}

void GWallet::CreateTool()
{
   itemToolBar = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL|wxWIZARD_VALIGN_BOTTOM, wxID_ANY );

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

   itemToolBar->AddTool(ID_ICON_CONNECT, wxT("Connect"), connect_icon, wxT("Connect"));
   itemToolBar->EnableTool(ID_ICON_CONNECT, true);
   itemToolBar->AddTool(ID_ICON_DISCONNECT, wxT("Disconnect"), disconnect_icon, wxT("Disconnect"));
   itemToolBar->EnableTool(ID_ICON_DISCONNECT, false);
   itemToolBar->AddTool(ID_ICON_LOCK, wxT("Lock"), lock_icon, wxT("Lock"));
   itemToolBar->EnableTool(ID_ICON_LOCK, false);
   itemToolBar->AddTool(ID_ICON_UNLOCK, wxT("Unlock"), unlock_icon, wxT("Unlock"));
   itemToolBar->EnableTool(ID_ICON_UNLOCK, false);

   itemToolBar->AddSeparator();

   itemToolBar->AddTool(ID_ICON_HOME, wxT("HOME"), home_icon, wxT("Home"));
   itemToolBar->EnableTool(ID_ICON_HOME, false);
   itemToolBar->AddTool(ID_ICON_COMMAND, wxT("Command Mode"), command_icon, wxT("Console"));
   itemToolBar->EnableTool(ID_ICON_COMMAND, false);
   itemToolBar->AddTool(ID_ICON_SENDRECEIVE, wxT("Send/Receive Mode"), transfer_icon, wxT("Send/Receive"));
   itemToolBar->EnableTool(ID_ICON_SENDRECEIVE, false);
   itemToolBar->AddTool(ID_ICON_HISTORY, wxT("Account History Mode"), history_icon, wxT("Account History"));
   itemToolBar->EnableTool(ID_ICON_HISTORY, false);
   itemToolBar->AddTool(ID_ICON_WALLET, wxT("Wallet Mode"), wallet_icon, wxT("Wallet"));
   itemToolBar->EnableTool(ID_ICON_WALLET, false);

   itemToolBar->AddStretchableSpace();

   wxArrayString itemComboBoxStrings;
   combo_accounts = new wxComboBox( itemToolBar, ID_COMBO_ACCOUNTS, wxT("MY ACCOUNTS"),
           wxDefaultPosition, wxDefaultSize, itemComboBoxStrings, wxCB_DROPDOWN );
   itemToolBar->AddControl(combo_accounts, wxT("MY ACCOUNTS"));
   combo_accounts->Enable(false);

   itemToolBar->AddSeparator();

   combo_assets = new wxComboBox( itemToolBar, ID_COMBO_ASSETS, wxT("MY ASSETS"),
           wxDefaultPosition, wxDefaultSize, itemComboBoxStrings, wxCB_DROPDOWN );
   itemToolBar->AddControl(combo_assets, wxT("MY ASSETS"));
   combo_assets->Enable(false);

   itemToolBar->Realize();
   this->SetToolBar(itemToolBar);
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

   if (is_noconfig) {
      file->Enable(wxID_NEW, true);
      file->Enable(wxID_OPEN, true);
      file->Enable(wxID_SAVE, false);
      file->Enable(wxID_NETWORK, false);

      wallet_m->Enable(ID_CONNECT, false);
      wallet_m->Enable(ID_DISCONNECT, false);
      wallet_m->Enable(ID_SETPASSWORD, false);
      wallet_m->Enable(ID_LOCK, false);
      wallet_m->Enable(ID_UNLOCK, false);
      wallet_m->Enable(ID_IMPORTKEY, false);

      itemToolBar->EnableTool(ID_ICON_CONNECT, false);
      itemToolBar->EnableTool(ID_ICON_DISCONNECT, false);
      itemToolBar->EnableTool(ID_ICON_LOCK, false);
      itemToolBar->EnableTool(ID_ICON_UNLOCK, false);
      itemToolBar->EnableTool(ID_ICON_HOME, false);
      itemToolBar->EnableTool(ID_ICON_COMMAND, false);
      itemToolBar->EnableTool(ID_ICON_HISTORY, false);
      itemToolBar->EnableTool(ID_ICON_SENDRECEIVE, false);
      itemToolBar->EnableTool(ID_ICON_WALLET, false);

      SetStatusText(_("No Config"), 0);
   }
   else if (is_connected) {

      file->Enable(wxID_NEW, true);
      file->Enable(wxID_OPEN, true);
      file->Enable(wxID_SAVE, true);
      file->Enable(wxID_NETWORK, true);

      wallet_m->Enable(ID_CONNECT, false);
      wallet_m->Enable(ID_DISCONNECT, true);
      wallet_m->Enable(ID_SETPASSWORD, false);
      wallet_m->Enable(ID_LOCK, false);
      wallet_m->Enable(ID_UNLOCK, false);
      wallet_m->Enable(ID_IMPORTKEY, false);

      itemToolBar->EnableTool(ID_ICON_CONNECT, false);
      itemToolBar->EnableTool(ID_ICON_DISCONNECT, true);
      itemToolBar->EnableTool(ID_ICON_LOCK, false);
      itemToolBar->EnableTool(ID_ICON_UNLOCK, false);

      itemToolBar->EnableTool(ID_ICON_HOME, false);
      itemToolBar->EnableTool(ID_ICON_COMMAND, false);
      itemToolBar->EnableTool(ID_ICON_HISTORY, false);
      itemToolBar->EnableTool(ID_ICON_SENDRECEIVE, false);
      itemToolBar->EnableTool(ID_ICON_WALLET, false);

      if (is_new) {
         SetStatusText(_("Connected | New"));
         wallet_m->Enable(ID_SETPASSWORD, true);
         mainMsg->SetLabel(_("G-Wallet New"));
      }
      else if (!is_account_linked) {
         SetStatusText(_("Connected | New"));
         wallet_m->Enable(ID_IMPORTKEY, false);
         //wallet_m->Enable(ID_LOCK, false);
         wallet_m->Enable(ID_UNLOCK, true);
         mainMsg->SetLabel(_("G-Wallet New"));
      }
      else if (is_locked) {
         SetStatusText(_("Connected | Locked"));

         wallet_m->Enable(ID_SETPASSWORD, false);
         wallet_m->Enable(ID_LOCK, false);
         wallet_m->Enable(ID_UNLOCK, true);
         wallet_m->Enable(ID_IMPORTKEY, false);

         itemToolBar->EnableTool(ID_ICON_UNLOCK, true);
         itemToolBar->EnableTool(ID_ICON_COMMAND, true);
         itemToolBar->EnableTool(ID_ICON_HOME, true);
         mainMsg->SetLabel(_("G-Wallet Ready"));
      }
      else if (is_unlocked) {
         SetStatusText(_("Connected | Unlocked"));

         wallet_m->Enable(ID_SETPASSWORD, false);
         wallet_m->Enable(ID_LOCK, true);
         wallet_m->Enable(ID_UNLOCK, false);
         wallet_m->Enable(ID_IMPORTKEY, true);

         itemToolBar->EnableTool(ID_ICON_LOCK, true);
         itemToolBar->EnableTool(ID_ICON_HOME, true);
         itemToolBar->EnableTool(ID_ICON_COMMAND, true);
         itemToolBar->EnableTool(ID_ICON_HISTORY, true);
         itemToolBar->EnableTool(ID_ICON_SENDRECEIVE, true);
         itemToolBar->EnableTool(ID_ICON_WALLET, true);
         mainMsg->SetLabel(_("G-Wallet Ready"));
      }

      wxString server;
      if(config->Read("Server", &server)) {
         int widths[] = {GetClientSize().x - 25 - GetTextExtent(server).x, -1};
         SetStatusWidths(2, widths);
         SetStatusText(server, 1);
      }
   }
   else if (!is_connected) {

      combo_accounts->Enable(false);
      combo_assets->Enable(false);

      itemToolBar->EnableTool(ID_ICON_HOME, false);
      itemToolBar->EnableTool(ID_ICON_COMMAND, false);
      itemToolBar->EnableTool(ID_ICON_HISTORY, false);
      itemToolBar->EnableTool(ID_ICON_SENDRECEIVE, false);
      itemToolBar->EnableTool(ID_ICON_WALLET, false);

      itemToolBar->EnableTool(ID_ICON_CONNECT, true);
      itemToolBar->EnableTool(ID_ICON_DISCONNECT, false);
      itemToolBar->EnableTool(ID_ICON_LOCK, false);
      itemToolBar->EnableTool(ID_ICON_UNLOCK, false);

      wallet_m->Enable(ID_CONNECT, true);
      wallet_m->Enable(ID_DISCONNECT, false);
      wallet_m->Enable(ID_SETPASSWORD, false);
      wallet_m->Enable(ID_LOCK, false);
      wallet_m->Enable(ID_UNLOCK, false);
      wallet_m->Enable(ID_IMPORTKEY, false);

      SetStatusText(_("Disconnected"), 0);
   }
}

void GWallet::CreateInfo()
{
   infoSizer = new wxBoxSizer(wxHORIZONTAL);
   mainSizer->Add(infoSizer, 0, wxGROW|wxALL);

   wxFont font = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

   mainMsg = new wxStaticText(panel, wxID_STATIC, wxT("G-Wallet Offline"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
   mainMsg->SetFont(font);
   infoSizer->Add(mainMsg, 0, wxALL|wxALIGN_LEFT, 5);

   infoSizer->AddStretchSpacer();

   balanceMsg = new wxStaticText(panel, wxID_STATIC, wxT("0.00 BTS"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
   balanceMsg->SetFont(font);
   infoSizer->Add(balanceMsg, 0, wxALL|wxALIGN_RIGHT, 5);
}

void GWallet::CreateMain()
{
   panel = new wxPanel(this, wxID_ANY);
   mainSizer = new wxBoxSizer(wxVERTICAL);
   panel->SetSizer(mainSizer);
}
