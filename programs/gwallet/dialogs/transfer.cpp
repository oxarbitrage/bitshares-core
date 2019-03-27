#include "../include/dialogs/transfer.hpp"
#include "../include/gwallet.hpp"
#include "../include/modes/wallet.hpp"

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/combo.h>

TransferDialog::TransferDialog(wxWindow* parent, wxWindowID id, const wxString& title,
        const wxPoint &position, const wxSize& size, long style) : wxDialog( parent, id, title, position, size, style)
{
   Wallet* p_Wallet = dynamic_cast<Wallet*>(GetParent());
   GWallet* p_GWallet = p_Wallet->p_GWallet;

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

   //wxArrayString itemComboBox1Strings;
   wxComboBox* itemComboBox1 = new wxComboBox( itemDialog1, wxID_ANY, p_GWallet->selected_account,
           wxDefaultPosition, wxDefaultSize, p_GWallet->strings_accounts, wxCB_DROPDOWN );
   itemBoxSizer6->Add(itemComboBox1, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

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

   wxTextCtrl* itemTextCtrl12 = new wxTextCtrl( itemDialog1, wxID_ANY, wxEmptyString,
           wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer9->Add(itemTextCtrl12, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

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

   wxTextCtrl* itemTextCtrl13 = new wxTextCtrl( itemDialog1, wxID_ANY, wxEmptyString,
           wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer7->Add(itemTextCtrl13, 7, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxComboBox* itemComboBox14 = new wxComboBox( itemDialog1, wxID_ANY, p_GWallet->selected_asset,
           wxDefaultPosition, wxDefaultSize, p_GWallet->strings_assets, wxCB_DROPDOWN );
   itemBoxSizer7->Add(itemComboBox14, 3, wxALIGN_CENTER_VERTICAL|wxALL, 0);

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

   wxTextCtrl* itemTextCtrl23 = new wxTextCtrl( itemDialog1, wxID_ANY, wxEmptyString,
           wxDefaultPosition, wxSize(-1, 100), wxTE_MULTILINE );
   itemBoxSizer21->Add(itemTextCtrl23, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer21->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer18, 0, wxGROW|wxALL, 0);

   itemBoxSizer18->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxCheckBox* itemCheckBox1 = new wxCheckBox( itemDialog1, ID_DIALOG_TRANSFER_BROADCAST,
           _("Broadcast Transaction"), wxDefaultPosition, wxDefaultSize, 0 );
   itemCheckBox1->SetValue(false);
   itemBoxSizer18->Add(itemCheckBox1, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxCheckBox* itemCheckBox2 = new wxCheckBox( itemDialog1, ID_DIALOG_TRANSFER_CLI,
           _("Show output in CLI mode"), wxDefaultPosition, wxDefaultSize, 0 );
   itemCheckBox2->SetValue(false);
   itemBoxSizer18->Add(itemCheckBox2, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

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
   Centre();
   ShowModal();
   Destroy();

}

void TransferDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{

}

void TransferDialog::OnCancel(wxCommandEvent & WXUNUSED(event))
{
   Close(true);
}
