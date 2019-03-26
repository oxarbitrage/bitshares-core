#include "include/welcome.hpp"
#include "include/bitshares.hpp"

#include <wx/filename.h>
#include <wx/stdpaths.h>

Welcome1::Welcome1(wxWizard *parent) : wxWizardPageSimple(parent)
{
   wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

   mainSizer->Add(
   new wxStaticText(this, wxID_ANY,
           _("This wizard will help you setup a new wallet.\n You can skip this and do everything manually.\n"),
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

Welcome2::Welcome2(wxWizard *parent) : wxWizardPageSimple(parent)
{
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

   //wxButton* somebutton = new wxButton( this, ID_WALLETPATH, _("Change Path of wallet file"), wxDefaultPosition, wxDefaultSize, 0 );
   wxBitmapButton* somebutton = new wxBitmapButton(this, ID_WALLETPATH, folder_find_icon,
           wxDefaultPosition, wxSize(32, 32), wxBU_AUTODRAW);
   pathSizer->Add(somebutton, 1, wxALL, 5);

   mainSizer->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   wxStaticText* itemStaticText12 = new wxStaticText( this, wxID_STATIC,
           wxT("Click on Next to continue with the configuration."), wxDefaultPosition, wxDefaultSize, 0 );
   mainSizer->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALL, 5);

   Connect(ID_WALLETPATH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Welcome2::OnPath));
   Connect(ID_WIZARD, wxEVT_WIZARD_PAGE_CHANGING, wxWizardEventHandler(Welcome2::OnWizardPageChanging));

   wxWindow* NextButton=FindWindowById(wxID_FORWARD,this);
   if (NextButton!=NULL) NextButton->Enable(false);

   mainSizer->Fit(this);
}

void Welcome2::OnPath(wxCommandEvent & WXUNUSED(event))
{

   wxFileName f(wxStandardPaths::Get().GetExecutablePath());
   wxString directory(f.GetPath());

   wxDirDialog dialog(this, wxT("Select your wallet directory"), directory, wxDD_NEW_DIR_BUTTON);
   if (dialog.ShowModal() == wxID_OK)
   {
      wxString path = dialog.GetPath();
      wxMessageBox(path);
      pathCtrl->SetValue(path + "/wallet.json");
   }
}

void Welcome2::OnWizardPageChanging(wxWizardEvent & WXUNUSED(event))
{
   auto path = pathCtrl->GetValue();

   wxConfig* config = new wxConfig(wxT("GWallet"));
   config->Write("WalletPath", path);

   delete config;
}

Welcome3::Welcome3(wxWizard *parent) : wxWizardPageSimple(parent)
{
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

   //wxButton* somebutton = new wxButton( this, ID_WALLETPATH, _("Test endpoint"), wxDefaultPosition, wxDefaultSize, 0 );
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

void Welcome3::OnTestServer(wxCommandEvent & WXUNUSED(event))
{
   Bitshares wallet;
   wallet.Connect("wss://api.bitshares-kibana.info/ws");

   try
   {
      wallet.wallet_api_ptr->get_global_properties();

      wxMessageDialog dialog(NULL, "Connected to server", wxT("Success"), wxNO_DEFAULT | wxOK | wxICON_INFORMATION);
      if (dialog.ShowModal() == wxID_OK)
         return;
   }
   catch( const fc::exception& e )
   {
      wxMessageDialog dialog( NULL, "Some problem connecting", wxT("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);
      if ( dialog.ShowModal() == wxID_OK )
         return;
   }
}

void Welcome3::OnWizardPageChanging(wxWizardEvent& event)
{
   auto server = serverCtrl->GetValue();
   auto password = passwordCtrl->GetValue();
   auto repeatpassword = repeatpasswordCtrl->GetValue();

   if(password != repeatpassword) {
      wxMessageDialog dialog( NULL, wxT("Password and confirmation are not the same"),
              _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if ( dialog.ShowModal() == wxID_OK )
      {
         event.Veto();
      }
   }
   else if(password == "" && repeatpassword == "" ) {
      wxMessageDialog dialog( NULL, wxT("Password or confirmation cant be empty"),
              _("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if ( dialog.ShowModal() == wxID_OK )
      {
         event.Veto();
      }
   }
   wxConfig* config = new wxConfig(wxT("GWallet"));
   config->Write("Server", server);

   delete config;
}

Welcome4::Welcome4(wxWizard *parent) : wxWizardPageSimple(parent)
{
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

   accountCtrl = new wxTextCtrl(this, -1, wxT("oxarbitrage"), wxDefaultPosition, wxDefaultSize, 0);
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

void Welcome4::OnAccount(wxCommandEvent & WXUNUSED(event))
{

}

void Welcome4::OnKey(wxCommandEvent & WXUNUSED(event))
{

}

void Welcome4::OnWizardPageChanging(wxWizardEvent& event)
{
   auto account = accountCtrl->GetValue();
   auto key = keyCtrl->GetValue();


   if(account == "") {
      wxMessageDialog dialog( NULL, wxT("Account can't be empty"), wxT("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if ( dialog.ShowModal() == wxID_OK )
      {
         event.Veto();
      }
   }
   else if(key == "") {
      wxMessageDialog dialog( NULL, wxT("Private key can't be empty"), wxT("Error"), wxNO_DEFAULT|wxOK|wxICON_ERROR);

      if ( dialog.ShowModal() == wxID_OK )
      {
         event.Veto();
      }
   }
   /*
   wxConfig* config = new wxConfig(wxT("GWallet"));
   config->Write("Server", server);

   delete config;
   */
}

