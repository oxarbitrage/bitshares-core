#include "../include/dialogs/borrowasset.hpp"
#include "../include/modes/wallet.hpp"

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/combo.h>

BorrowAssetDialog::BorrowAssetDialog(wxWindow* parent, wxWindowID id, const wxString& title,
      const wxPoint &position, const wxSize& size, long style) : wxDialog( parent, id, title, position, size, style)
{
   Wallet* p_Wallet = dynamic_cast<Wallet*>(GetParent());
   p_GWallet = p_Wallet->p_GWallet;

   BorrowAssetDialog* itemDialog1 = this;

   wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
   itemDialog1->SetSizer(itemBoxSizer2);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer1, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   wxStaticText* itemStaticText2 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Borrow Asset"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer1->Add(itemStaticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 0);

   itemBoxSizer3->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Seller account"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer3->Add(itemStaticText5, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer3->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer7, 0, wxGROW|wxALL, 0);

   itemBoxSizer7->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   seller = new wxComboBox( itemDialog1, wxID_ANY, p_GWallet->strings.selected_account,
         wxDefaultPosition, wxDefaultSize, p_GWallet->strings.accounts, wxCB_DROPDOWN|wxCB_READONLY );
   itemBoxSizer7->Add(seller, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer7->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW|wxALL, 0);

   itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Amount to borrow"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer4->Add(itemStaticText6, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer8, 0, wxGROW|wxALL, 0);

   itemBoxSizer8->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   borrow_amount = new wxTextCtrl( itemDialog1, wxID_ANY,
         wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY|wxFILTER_NUMERIC) );
   itemBoxSizer8->Add(borrow_amount, 7, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   borrow_asset = new wxSearchCtrl( itemDialog1, wxID_ANY,
         wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer8->Add(borrow_asset, 3, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer8->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer5, 0, wxGROW|wxALL, 0);

   itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText15 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Amount of collateral"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer5->Add(itemStaticText15, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer17, 0, wxGROW|wxALL, 0);

   itemBoxSizer17->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   collateral_amount = new wxTextCtrl( itemDialog1, wxID_ANY,
         wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY|wxFILTER_NUMERIC) );
   itemBoxSizer17->Add(collateral_amount, 7, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText20 = new wxStaticText( itemDialog1, wxID_STATIC,
         _(" BTS"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer17->Add(itemStaticText20, 3, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer17->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer23, 0, wxGROW|wxALL, 0);

   itemBoxSizer23->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   broadcast = new wxCheckBox( itemDialog1, wxID_ANY,
         _("Broadcast transaction"), wxDefaultPosition, wxDefaultSize, 0 );
   broadcast->SetValue(true);
   broadcast->Enable(false);
   itemBoxSizer23->Add(broadcast, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   cli = new wxCheckBox( itemDialog1, wxID_ANY,
         _("Show output in CLI mode"), wxDefaultPosition, wxDefaultSize, 0 );
   cli->SetValue(false);
   cli->Enable(false);
   itemBoxSizer23->Add(cli, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer23->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer29 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer29, 0, wxGROW|wxALL, 0);

   itemBoxSizer29->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticLine* itemStaticLine31 = new wxStaticLine( itemDialog1, wxID_STATIC,
         wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
   itemBoxSizer29->Add(itemStaticLine31, 10, wxGROW|wxALL, 0);

   itemBoxSizer29->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxStdDialogButtonSizer* itemStdDialogButtonSizer34 = new wxStdDialogButtonSizer;

   itemBoxSizer2->Add(itemStdDialogButtonSizer34, 0, wxGROW|wxALL, 5);
   wxButton* itemButton35 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStdDialogButtonSizer34->AddButton(itemButton35);

   wxButton* itemButton36 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStdDialogButtonSizer34->AddButton(itemButton36);

   itemStdDialogButtonSizer34->Realize();

   CreateEvents();

   Centre();
   ShowModal();
   Destroy();
}

void BorrowAssetDialog::CreateEvents()
{
   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(BorrowAssetDialog::OnOk));
}

void BorrowAssetDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto seller_value = p_GWallet->strings.accounts[seller->GetCurrentSelection()].ToStdString();
   const auto borrow_amount_value = borrow_amount->GetValue().ToStdString();
   const auto borrow_asset_value = borrow_asset->GetValue().ToStdString();
   const auto collateral_amount_value = collateral_amount->GetValue().ToStdString();

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_asset(borrow_asset_value);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(_("Asset is invalid"));
      borrow_asset->SetFocus();
      return;
   }

   try {
      auto result = p_GWallet->bitshares.wallet_api_ptr->borrow_asset(seller_value, borrow_amount_value,
            borrow_asset_value, collateral_amount_value, false);

      if (wxYES == wxMessageBox(fc::json::to_pretty_string(result.operations[0]), _("Confirm Borrow Asset?"),
                                wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
         auto result = p_GWallet->bitshares.wallet_api_ptr->borrow_asset(seller_value, borrow_amount_value,
               borrow_asset_value, collateral_amount_value, false);
      }
   }
   catch (const fc::exception &e) {
      p_GWallet->OnError(e.to_detail_string());
   }

}



