#include "../include/wizards/welcome.hpp"

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include <wx/busyinfo.h>

Welcome1::Welcome1(wxWizard* parent, GWallet* gwallet) : wxWizardPageSimple(parent)
{
   p_GWallet = gwallet;

   wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

   mainSizer->Add(
   new wxStaticText(this, wxID_ANY,
         _("This wizard will help you setup a new wallet.\n"),
         wxDefaultPosition, wxDefaultSize, 0 )
   );

   mainSizer->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   wxStaticText* itemStaticText12 = new wxStaticText( this, wxID_STATIC,
         _("Click Next to get started..\nClick Cancel to start the program now."),
         wxDefaultPosition, wxDefaultSize, 0 );
   mainSizer->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALL, 5);

   SetSizer(mainSizer);
   mainSizer->Fit(this);
}

Welcome2::Welcome2(wxWizard* parent, GWallet* gwallet) : wxWizardPageSimple(parent)
{
   p_GWallet = gwallet;

   wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
   this->SetSizer(mainSizer);

   wxStaticText* itemStaticText4 = new wxStaticText( this, wxID_STATIC,
         _("First step to start using G-Wallet is to create a wallet file.\n"),
         wxDefaultPosition, wxDefaultSize, 0 );
   mainSizer->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALL, 5);

   wxStaticText* itemStaticText5 = new wxStaticText( this, wxID_STATIC,
         _("This file is used to store configuration options that will be used to start the application next\n"
           "time you use it.\n"),
           wxDefaultPosition, wxDefaultSize, 0 );
   mainSizer->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALL, 5);

   mainSizer->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   wxStaticText* itemStaticText6 = new wxStaticText( this, wxID_STATIC, _("Select wallet path and name:"),
         wxDefaultPosition, wxDefaultSize, 0 );
   mainSizer->Add(itemStaticText6, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   wxString directory(f.GetPath());

   wxBoxSizer *pathSizer = new wxBoxSizer(wxHORIZONTAL);
   mainSizer->Add(pathSizer, 0, wxGROW|wxALL);

   pathCtrl = new wxTextCtrl(this, -1, directory + wxT("/wallet.json"), wxDefaultPosition, wxDefaultSize, 0);

   pathSizer->Add(pathCtrl, 11, wxGROW|wxALL, 5);

   wxImage::AddHandler(new wxPNGHandler);
   wxBitmap folder_find_icon(directory + wxT("/icons/folder-find.png"), wxBITMAP_TYPE_PNG);

   wxBitmapButton* path_button = new wxBitmapButton(this, ID_WALLETPATH, folder_find_icon,
         wxDefaultPosition, wxSize(32, 32), wxBU_AUTODRAW);
   pathSizer->Add(path_button, 1, wxALL, 5);

   mainSizer->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   wxStaticText* itemStaticText12 = new wxStaticText( this, wxID_STATIC,
         _("Click on Next to continue with the configuration."), wxDefaultPosition, wxDefaultSize, 0 );
   mainSizer->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALL, 5);

   Connect(ID_WALLETPATH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Welcome2::OnPath));
   Connect(ID_WIZARD, wxEVT_WIZARD_PAGE_CHANGING, wxWizardEventHandler(Welcome2::OnWizardPageChanging));

   wxWindow* NextButton=FindWindowById(wxID_FORWARD,this);
   if (NextButton!=NULL) NextButton->Enable(false);

   mainSizer->Fit(this);
}

void Welcome2::OnPath(wxCommandEvent& WXUNUSED(event))
{
   const wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   const wxString directory(f.GetPath());

   wxDirDialog dialog(this, _("Select your wallet directory"), directory, wxDD_NEW_DIR_BUTTON);
   if (dialog.ShowModal() == wxID_OK)
   {
      const wxString path = dialog.GetPath();
      wxMessageBox(path);
      pathCtrl->SetValue(path + "/wallet.json");
   }
}

void Welcome2::OnWizardPageChanging(wxWizardEvent& event)
{
   const auto path = pathCtrl->GetValue();
   if(wxFileExists(path))
   {
      wxMessageDialog dialog( NULL, _("Wallet file exists, please close the welcome wizard and use File->Open "
                                      "from menu to use it."), _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);
      if (dialog.ShowModal() == wxID_OK)
      {
         event.Veto();
         return;
      }
   }
   p_GWallet->config->Write("WalletPath", path);
   p_GWallet->config->Flush();
}

Welcome3::Welcome3(wxWizard* parent, GWallet* gwallet) : wxWizardPageSimple(parent)
{
   p_GWallet = gwallet;

   wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

   wxStaticText* itemStaticText4 = new wxStaticText( this, wxID_STATIC,
         _("Select your server:"), wxDefaultPosition, wxDefaultSize, 0 );
   mainSizer->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALL, 5);

   wxBoxSizer *serverSizer = new wxBoxSizer(wxHORIZONTAL);
   mainSizer->Add(serverSizer, 0, wxGROW|wxALL);

   serverCtrl = new wxTextCtrl(this, -1, wxT("wss://api.bitshares-kibana.info/ws"), wxDefaultPosition, wxDefaultSize, 0);
   serverSizer->Add(serverCtrl, 11, wxGROW|wxALL, 5);

   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   wxString directory(f.GetPath());

   wxBitmap connect_icon(directory + wxT("/icons/network-connect-3.png"), wxBITMAP_TYPE_PNG);
   wxBitmapButton* testserver_button = new wxBitmapButton(this, ID_TESTSERVER,
         connect_icon, wxDefaultPosition, wxSize(32, 32), wxBU_AUTODRAW);

   serverSizer->Add(testserver_button, 1, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText5 = new wxStaticText( this, wxID_STATIC,
         _("Wallet password:"), wxDefaultPosition, wxDefaultSize, 0 );
   mainSizer->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALL, 5);

   passwordCtrl = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
   mainSizer->Add(passwordCtrl, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText6 = new wxStaticText( this, wxID_STATIC,
         _("Repeat password:"), wxDefaultPosition, wxDefaultSize, 0 );
   mainSizer->Add(itemStaticText6, 0, wxALIGN_LEFT|wxALL, 5);

   repeatpasswordCtrl = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
   mainSizer->Add(repeatpasswordCtrl, 0, wxGROW|wxALL, 5);

   Connect(ID_TESTSERVER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Welcome3::OnTestServer));
   Connect(ID_WIZARD, wxEVT_WIZARD_PAGE_CHANGING, wxWizardEventHandler(Welcome3::OnWizardPageChanging));

   SetSizer(mainSizer);
   mainSizer->Fit(this);
}

void Welcome3::OnTestServer(wxCommandEvent& WXUNUSED(event))
{
   const auto server = serverCtrl->GetValue();
   p_GWallet->config->Write("Server", server);
   p_GWallet->config->Flush();

   wxString path;
   if ( p_GWallet->config->Read("WalletPath", &path) ) {
      wxCommandEvent event_connect(wxEVT_COMMAND_MENU_SELECTED, ID_CONNECT);
      p_GWallet->ProcessWindowEvent(event_connect);
      try
      {
         p_GWallet->bitshares.wallet_api_ptr->get_global_properties();

         wxMessageDialog dialog(NULL, _("Connected to server"), _("Success"), wxNO_DEFAULT | wxOK | wxICON_INFORMATION);
         if (dialog.ShowModal() == wxID_OK)
            return;
      }
      catch(const fc::exception& e)
      {
         wxMessageDialog dialog(NULL, _("Some problem connecting"), _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);
         if ( dialog.ShowModal() == wxID_OK )
            return;
      }
   }
}

void Welcome3::OnWizardPageChanging(wxWizardEvent& event)
{
   const auto server = serverCtrl->GetValue();
   const auto password = passwordCtrl->GetValue();
   const auto repeatpassword = repeatpasswordCtrl->GetValue();

   if(password != repeatpassword) {
      wxMessageDialog dialog(NULL, _("Password and confirmation are not the same"),
            _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if(dialog.ShowModal() == wxID_OK)
      {
         event.Veto();
         return;
      }
   }
   else if(password == "" && repeatpassword == "" ) {
      wxMessageDialog dialog( NULL, _("Password or confirmation cant be empty"),
            _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if(dialog.ShowModal() == wxID_OK)
      {
         event.Veto();
         return;
      }
   }
   p_GWallet->config->Write("Server", server);
   p_GWallet->config->Flush();

   if(!p_GWallet->is_connected) {
      wxCommandEvent event_connect(wxEVT_COMMAND_MENU_SELECTED, ID_CONNECT);
      p_GWallet->ProcessWindowEvent(event_connect);
   }

   try {
      p_GWallet->bitshares.wallet_api_ptr->set_password(password.ToStdString());
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(e.to_detail_string());
   }
   try {
      p_GWallet->bitshares.wallet_api_ptr->unlock(password.ToStdString());
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(e.to_detail_string());
   }
   p_GWallet->is_locked = false;
   p_GWallet->is_new = false;
   p_GWallet->is_unlocked = true;
   p_GWallet->DoState();
}

Welcome4::Welcome4(wxWizard* parent, GWallet* gwallet) : wxWizardPageSimple(parent)
{
   p_GWallet = gwallet;

   wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

   wxStaticText* itemStaticText4 = new wxStaticText( this, wxID_STATIC,
         _("Import your key, we cant create account yet!"), wxDefaultPosition, wxDefaultSize, 0 );
   mainSizer->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALL, 5);

   wxBoxSizer *accountSizer = new wxBoxSizer(wxHORIZONTAL);
   mainSizer->Add(accountSizer, 0, wxGROW|wxALL);

   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   wxString directory(f.GetPath());

   wxBitmap identity(directory + wxT("/icons/identity.png"), wxBITMAP_TYPE_PNG);
   wxBitmapButton* accountButton = new wxBitmapButton(this, ID_ACCOUNT, identity,
         wxDefaultPosition, wxSize(32, 32), wxBU_AUTODRAW);

   accountCtrl = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
   accountSizer->Add(accountCtrl, 11, wxGROW|wxALL, 5);

   accountSizer->Add(accountButton, 1, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText5 = new wxStaticText( this, wxID_STATIC, _("Private key:"),
         wxDefaultPosition, wxDefaultSize, 0 );
   mainSizer->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALL, 5);

   wxBoxSizer *keySizer = new wxBoxSizer(wxHORIZONTAL);
   mainSizer->Add(keySizer, 0, wxGROW|wxALL);

   wxBitmap key(directory + wxT("/icons/key.png"), wxBITMAP_TYPE_PNG);
   wxBitmapButton* keyButton = new wxBitmapButton(this, ID_KEY, key, wxDefaultPosition,
         wxSize(32, 32), wxBU_AUTODRAW);

   keyCtrl = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
   keySizer->Add(keyCtrl, 11, wxGROW|wxALL, 5);

   keySizer->Add(keyButton, 1, wxGROW|wxALL, 5);

   Connect(ID_ACCOUNT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Welcome4::OnAccount));
   Connect(ID_KEY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Welcome4::OnKey));

   Connect(ID_WIZARD, wxEVT_WIZARD_PAGE_CHANGING, wxWizardEventHandler(Welcome4::OnWizardPageChanging));

   SetSizer(mainSizer);
   mainSizer->Fit(this);
}

void Welcome4::OnAccount(wxCommandEvent& WXUNUSED(event))
{
   auto account = accountCtrl->GetValue();
   if(account == "") {
      wxMessageDialog dialog(NULL, _("Account can't be empty"), _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);
      if(dialog.ShowModal() == wxID_OK) {
         return;
      }
   }
   else
   {
      try
      {
         p_GWallet->bitshares.wallet_api_ptr->get_account(account.ToStdString());
      }
      catch(const fc::exception& e)
      {
         p_GWallet->OnError(_("Account is invalid"));
         accountCtrl->SetFocus();
         return;
      }
      wxMessageBox(_("Account looks good"));
   }
}

void Welcome4::OnKey(wxCommandEvent& WXUNUSED(event))
{
   auto key = keyCtrl->GetValue();
   if(key == "") {
      wxMessageDialog dialog( NULL, _("Private key can't be empty"), _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if(dialog.ShowModal() == wxID_OK)
      {
         return;
      }
   }
   else {
      // Todo: do some validation on private key length and prefix?
   }
}

void Welcome4::OnWizardPageChanging(wxWizardEvent& event)
{
   auto account = accountCtrl->GetValue();
   auto key = keyCtrl->GetValue();

   if(account == "") {
      wxMessageDialog dialog( NULL, _("Account can't be empty"), _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if(dialog.ShowModal() == wxID_OK)
      {
         event.Veto();
      }
   }
   else
   {
      try
      {
         p_GWallet->bitshares.wallet_api_ptr->get_account(account.ToStdString());
      }
      catch(const fc::exception& e)
      {
         p_GWallet->OnError("Account is invalid");
         accountCtrl->SetFocus();
         return;
      }
   }

   if(key == "") {
      wxMessageDialog dialog( NULL, _("Private key can't be empty"), _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if(dialog.ShowModal() == wxID_OK)
      {
         event.Veto();
      }
   }
   else {
      // todo: do some validation on private key length and prefix?
   }

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->import_key(account.ToStdString(), key.ToStdString());
   }
   catch(const fc::exception& e)
   {
      //p_GWallet->OnError(_("Account/Key pair is invalid, please try again."));
      p_GWallet->OnError(e.to_detail_string());
      event.Veto();
   }

   p_GWallet->DoAccounts();
   p_GWallet->DoAssets(account.ToStdString());

   p_GWallet->config->Write("AllSet", true);
   p_GWallet->config->Flush();

   p_GWallet->is_account_linked = true;

   p_GWallet->DoState();

   if(!p_GWallet->modes_created) {
      p_GWallet->DoModes();
      p_GWallet->modes_created = true;
   }

}

