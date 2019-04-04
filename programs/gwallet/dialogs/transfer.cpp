#include "../include/dialogs/transfer.hpp"
#include "../include/modes/wallet.hpp"

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/combo.h>

TransferDialog::TransferDialog(wxWindow* parent, wxWindowID id, const wxString& title,
        const wxPoint &position, const wxSize& size, long style) : wxDialog( parent, id, title, position, size, style)
{
   Wallet* p_Wallet = dynamic_cast<Wallet*>(GetParent());
   p_GWallet = p_Wallet->p_GWallet;

   TransferDialog* itemDialog1 = this;

   wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
   itemDialog1->SetSizer(itemBoxSizer2);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Transfer asset to another bitshares account"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer5, 0, wxGROW|wxALL, 0);

   itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("From"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer5->Add(itemStaticText3, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer6, 0, wxGROW|wxALL, 0);

   itemBoxSizer6->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   from = new wxComboBox( itemDialog1, wxID_ANY, p_GWallet->selected_account,
         wxDefaultPosition, wxDefaultSize, p_GWallet->strings_accounts, wxCB_DROPDOWN|wxCB_READONLY );
   itemBoxSizer6->Add(from, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer6->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW|wxALL, 0);

   itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("To"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer4->Add(itemStaticText6, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer9, 0, wxGROW|wxALL, 0);

   itemBoxSizer9->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   to = new wxTextCtrl( itemDialog1, wxID_ANY, wxEmptyString,
         wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY) );
   itemBoxSizer9->Add(to, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer9->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer1, 0, wxGROW|wxALL, 0);

   itemBoxSizer1->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Amount"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer1->Add(itemStaticText5, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer1->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer7, 0, wxGROW|wxALL, 0);

   itemBoxSizer7->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   amount = new wxTextCtrl( itemDialog1, wxID_ANY, wxEmptyString,
         wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY|wxFILTER_NUMERIC) );
   itemBoxSizer7->Add(amount, 7, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   asset = new wxComboBox( itemDialog1, wxID_ANY, p_GWallet->selected_asset,
         wxDefaultPosition, wxDefaultSize, p_GWallet->strings_assets, wxCB_DROPDOWN|wxCB_READONLY );
   itemBoxSizer7->Add(asset, 3, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer7->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer17, 0, wxGROW|wxALL, 0);

   itemBoxSizer17->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText19 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Memo"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer17->Add(itemStaticText19, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer17->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer21, 0, wxGROW|wxALL, 0);

   itemBoxSizer21->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   memo = new wxTextCtrl( itemDialog1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, 100), wxTE_MULTILINE );
   itemBoxSizer21->Add(memo, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer21->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer18, 0, wxGROW|wxALL, 0);

   itemBoxSizer18->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   broadcast = new wxCheckBox( itemDialog1, ID_DIALOG_TRANSFER_BROADCAST,
         _("Broadcast Transaction"), wxDefaultPosition, wxDefaultSize, 0 );
   broadcast->SetValue(true);
   broadcast->Enable(false);
   itemBoxSizer18->Add(broadcast, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   cli = new wxCheckBox( itemDialog1, ID_DIALOG_TRANSFER_CLI,
         _("Show output in CLI mode"), wxDefaultPosition, wxDefaultSize, 0 );
   cli->SetValue(false);
   cli->Enable(false);
   itemBoxSizer18->Add(cli, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer18->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer8, 0, wxGROW|wxALL, 0);

   itemBoxSizer8->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticLine* itemStaticLine26 = new wxStaticLine( itemDialog1, wxID_STATIC,
         wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
   itemBoxSizer8->Add(itemStaticLine26, 10, wxGROW|wxALL, 0);

   itemBoxSizer8->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxStdDialogButtonSizer* itemStdDialogButtonSizer29 = new wxStdDialogButtonSizer;

   itemBoxSizer2->Add(itemStdDialogButtonSizer29, 0, wxGROW|wxALL, 5);
   wxButton* itemButton30 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStdDialogButtonSizer29->AddButton(itemButton30);

   wxButton* itemButton31 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStdDialogButtonSizer29->AddButton(itemButton31);

   itemStdDialogButtonSizer29->Realize();

   CreateEvents();

   Centre();
   ShowModal();
   Destroy();
}

void TransferDialog::CreateEvents()
{
   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(TransferDialog::OnOk));
}

void TransferDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
   auto from_v = p_GWallet->strings_accounts[from->GetCurrentSelection()].ToStdString();
   auto to_v = to->GetValue().ToStdString();
   auto amount_v = amount->GetValue().ToStdString();
   auto asset_v = p_GWallet->strings_assets[asset->GetCurrentSelection()].ToStdString();
   auto memo_v = memo->GetValue().ToStdString();

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_account(to_v);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(_("Account is invalid"));
      from->SetFocus();
      return;
   }

   try {
      auto result = p_GWallet->bitshares.wallet_api_ptr->transfer(from_v, to_v, amount_v, asset_v, memo_v, false);

      if (wxYES == wxMessageBox(fc::json::to_pretty_string(result.operations[0]), _("Confirm transfer?"),
            wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
         p_GWallet->bitshares.wallet_api_ptr->transfer(from_v, to_v, amount_v, asset_v, memo_v, true);
      }
   }
   catch (const fc::exception &e) {
      p_GWallet->OnError(e.to_detail_string());
   }
}

