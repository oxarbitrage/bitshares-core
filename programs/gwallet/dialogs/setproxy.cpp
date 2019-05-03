#include "../include/dialogs/setproxy.hpp"
#include "../include/modes/wallet.hpp"

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/combo.h>

SetProxyDialog::SetProxyDialog(wxWindow* parent, wxWindowID id, const wxString& title,
      const wxPoint &position, const wxSize& size, long style) : wxDialog( parent, id, title, position, size, style)
{
   Wallet* p_Wallet = dynamic_cast<Wallet*>(GetParent());
   p_GWallet = p_Wallet->p_GWallet;

   SetProxyDialog* itemDialog1 = this;

   DoVotingAccount();

   wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
   itemDialog1->SetSizer(itemBoxSizer2);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   wxStaticText* itemStaticText1 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Change the proxy of your account"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer3->Add(itemStaticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer6, 0, wxGROW|wxALL, 0);

   itemBoxSizer6->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Account to modify"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer6->Add(itemStaticText8, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer6->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer10, 0, wxGROW|wxALL, 0);

   itemBoxSizer10->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   account_to_modify = new wxComboBox( itemDialog1, wxID_ANY, p_GWallet->strings.selected_account,
         wxDefaultPosition, wxDefaultSize, p_GWallet->strings.accounts, wxCB_DROPDOWN|wxCB_READONLY );
   itemBoxSizer10->Add(account_to_modify, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer10->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer15, 0, wxGROW|wxALL, 0);

   itemBoxSizer15->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText17 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Voting account"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer15->Add(itemStaticText17, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer15->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer19, 0, wxGROW|wxALL, 0);

   itemBoxSizer19->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   voting_account = new wxSearchCtrl( itemDialog1, wxID_ANY, current_voting_account, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer19->Add(voting_account, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer19->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW|wxALL, 0);

   itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   broadcast = new wxCheckBox( itemDialog1, wxID_ANY,
         _("Broadcast transaction"), wxDefaultPosition, wxDefaultSize, 0 );
   broadcast->SetValue(true);
   broadcast->Enable(false);
   itemBoxSizer4->Add(broadcast, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   cli = new wxCheckBox( itemDialog1, wxID_ANY,
         _("Show output in CLI mode"), wxDefaultPosition, wxDefaultSize, 0 );
   cli->SetValue(false);
   cli->Enable(false);
   itemBoxSizer4->Add(cli, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer11, 0, wxGROW|wxALL, 0);

   itemBoxSizer11->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticLine* itemStaticLine13 = new wxStaticLine( itemDialog1, wxID_STATIC,
         wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
   itemBoxSizer11->Add(itemStaticLine13, 10, wxGROW|wxALL, 0);

   itemBoxSizer11->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxStdDialogButtonSizer* itemStdDialogButtonSizer18 = new wxStdDialogButtonSizer;

   itemBoxSizer2->Add(itemStdDialogButtonSizer18, 0, wxGROW|wxALL, 5);
   wxButton* itemButton19 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStdDialogButtonSizer18->AddButton(itemButton19);

   wxButton* itemButton20 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStdDialogButtonSizer18->AddButton(itemButton20);

   itemStdDialogButtonSizer18->Realize();

   CreateEvents();

   Centre();
   ShowModal();
   Destroy();
}

void SetProxyDialog::CreateEvents()
{
   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SetProxyDialog::OnOk));
}

void SetProxyDialog::DoVotingAccount()
{
   //const auto selected_account = p_GWallet->strings.accounts[account_to_modify->GetCurrentSelection()].ToStdString();
   const auto selected_account = p_GWallet->strings.selected_account.ToStdString();

   try
   {
      const auto account = p_GWallet->bitshares.wallet_api_ptr->get_account(selected_account);
      const auto account_id_string = std::string(object_id_type(account.options.voting_account));
      current_voting_account = p_GWallet->bitshares.wallet_api_ptr->get_account(account_id_string).name;
   }
   catch(const fc::exception& e)
   {

   }
}

void SetProxyDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto account_value = p_GWallet->strings.accounts[account_to_modify->GetCurrentSelection()].ToStdString();
   const auto voting_account_value = voting_account->GetValue().ToStdString();

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_account(account_value);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(_("Account is invalid"));
      voting_account->SetFocus();
      return;
   }

   try
   {
      auto result = p_GWallet->bitshares.wallet_api_ptr->set_voting_proxy(account_value, voting_account_value, false);
      if (wxYES == wxMessageBox(fc::json::to_pretty_string(result.operations[0]), _("Confirm update of voting account?"),
            wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
         p_GWallet->bitshares.wallet_api_ptr->set_voting_proxy(account_value, voting_account_value, true);
         Close(true);
      }
   }
   catch (const fc::exception &e) {
      p_GWallet->OnError(e.to_detail_string());
   }
}



