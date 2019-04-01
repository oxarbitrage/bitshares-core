#include "../include/dialogs/importkey.hpp"
#include "../include/gwallet.hpp"

#include <wx/wx.h>
#include <wx/statline.h>


ImportKeyDialog::ImportKeyDialog(wxWindow* parent, wxWindowID id, const wxString& title,
      const wxPoint &position, const wxSize& size, long style ) : wxDialog( parent, id, title, position, size, style)
{
   ImportKeyDialog* itemDialog1 = this;

   wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
   itemDialog1->SetSizer(itemBoxSizer2);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer1, 0, wxGROW|wxALL, 0);

   itemBoxSizer1->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Account Name"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer1->Add(itemStaticText3, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer1->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer5, 0, wxGROW|wxALL, 0);

   itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   account = new wxTextCtrl( itemDialog1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer5->Add(account, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);

   wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer10, 0, wxGROW|wxALL, 0);

   itemBoxSizer10->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText12 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Private Key"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer10->Add(itemStaticText12, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer10->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer14, 0, wxGROW|wxALL, 0);

   itemBoxSizer14->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   key = new wxTextCtrl( itemDialog1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer14->Add(key, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer14->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer18, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

   itemBoxSizer18->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer23, 0, wxGROW|wxALL, 0);

   itemBoxSizer23->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticLine* itemStaticLine4 = new wxStaticLine( itemDialog1, wxID_STATIC,
         wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
   itemBoxSizer23->Add(itemStaticLine4, 10, wxGROW|wxALL, 0);

   itemBoxSizer23->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer25, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

   itemBoxSizer25->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 0);

   itemBoxSizer3->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer3->Add(itemBoxSizer13, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxButton* itemButton14 = new wxButton( itemDialog1, ID_IMPORTKEY_OK,
         _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer13->Add(itemButton14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer13->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1);

   wxButton* itemButton15 = new wxButton( itemDialog1, ID_IMPORTKEY_CANCEL,
         _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer13->Add(itemButton15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer3->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer16, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

   itemBoxSizer16->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

   Connect(ID_IMPORTKEY_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImportKeyDialog::OnOk));
   Connect(ID_IMPORTKEY_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImportKeyDialog::OnCancel));
   Centre();
   ShowModal();
   Destroy();
}

void ImportKeyDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
   auto acct = account->GetValue();
   auto pkey = key->GetValue();

   GWallet* p_GWallet = dynamic_cast<GWallet*>(GetParent());

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->import_key(acct.ToStdString(), pkey.ToStdString());
   }
   catch( const fc::exception& e )
   {
      p_GWallet->OnError(wxT("Account/Key pair is invalid, please try again."));
   }
}

void ImportKeyDialog::OnCancel(wxCommandEvent & WXUNUSED(event))
{
   Close(true);
}
