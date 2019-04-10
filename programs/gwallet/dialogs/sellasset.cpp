#include "../include/dialogs/sellasset.hpp"
#include "../include/modes/wallet.hpp"

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/combo.h>

SellAssetDialog::SellAssetDialog(wxWindow* parent, wxWindowID id, const wxString& title,
      const wxPoint &position, const wxSize& size, long style) : wxDialog( parent, id, title, position, size, style)
{
   Wallet* p_Wallet = dynamic_cast<Wallet*>(GetParent());
   p_GWallet = p_Wallet->p_GWallet;

   SellAssetDialog* itemDialog1 = this;

   wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
   itemDialog1->SetSizer(itemBoxSizer2);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Buy or sell asset"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer5, 0, wxGROW|wxALL, 0);

   itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Seller account"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer5->Add(itemStaticText7, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer9, 0, wxGROW|wxALL, 0);

   itemBoxSizer9->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxArrayString itemComboBox1Strings;
   seller = new wxComboBox( itemDialog1, wxID_ANY, p_GWallet->strings.selected_account,
         wxDefaultPosition, wxDefaultSize, p_GWallet->strings.accounts, wxCB_DROPDOWN|wxCB_READONLY );
   itemBoxSizer9->Add(seller, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer9->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer14, 0, wxGROW|wxALL, 5);

   itemBoxSizer14->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText16 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Amount to sell"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer14->Add(itemStaticText16, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer14->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer18, 0, wxGROW|wxALL, 0);

   itemBoxSizer18->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   sell_amount = new wxTextCtrl( itemDialog1, wxID_ANY,
         wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY|wxFILTER_NUMERIC) );
   itemBoxSizer18->Add(sell_amount, 6, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   sell_asset = new wxComboBox( itemDialog1, wxID_ANY, p_GWallet->strings.selected_asset,
         wxDefaultPosition, wxDefaultSize, p_GWallet->strings.assets, wxCB_DROPDOWN, wxTextValidator(wxFILTER_EMPTY) );
   itemBoxSizer18->Add(sell_asset, 3, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer18->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

   itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Minimum amount to receive"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer4->Add(itemStaticText6, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer1, 0, wxGROW|wxALL, 0);

   itemBoxSizer1->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   receive_amount = new wxTextCtrl( itemDialog1, wxID_ANY,
         wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_EMPTY|wxFILTER_NUMERIC) );
   itemBoxSizer1->Add(receive_amount, 6, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   receive_asset = new wxSearchCtrl( itemDialog1, wxID_ANY,
         wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer1->Add(receive_asset, 3, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer1->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer6, 0, wxGROW|wxALL, 0);

   itemBoxSizer6->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText12 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Timeout"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer6->Add(itemStaticText12, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer6->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer15, 0, wxGROW|wxALL, 0);

   itemBoxSizer15->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   date = new wxDatePickerCtrl( itemDialog1, wxID_ANY,
         wxDateTime(), wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
   itemBoxSizer15->Add(date, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   time = new wxTimePickerCtrl( itemDialog1, wxID_ANY,
         wxDateTime(), wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
   itemBoxSizer15->Add(time, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer15->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer21, 0, wxGROW|wxALL, 0);

   itemBoxSizer21->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   fill_or_kill = new wxCheckBox( itemDialog1, wxID_ANY,
         _("Fill or Kill"), wxDefaultPosition, wxDefaultSize, 0 );
   fill_or_kill->SetValue(false);
   itemBoxSizer21->Add(fill_or_kill, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer21->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer27, 0, wxGROW|wxALL, 0);

   itemBoxSizer27->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   broadcast = new wxCheckBox( itemDialog1, wxID_ANY,
         _("Broadcast transaction"), wxDefaultPosition, wxDefaultSize, 0 );
   broadcast->SetValue(true);
   broadcast->Enable(false);
   itemBoxSizer27->Add(broadcast, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   cli = new wxCheckBox( itemDialog1, wxID_ANY,
         _("Show output in CLI mode"), wxDefaultPosition, wxDefaultSize, 0 );
   cli->SetValue(false);
   cli->Enable(false);
   itemBoxSizer27->Add(cli, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer27->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer22, 0, wxGROW|wxALL, 5);

   itemBoxSizer22->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticLine* itemStaticLine25 = new wxStaticLine( itemDialog1, wxID_STATIC,
         wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
   itemBoxSizer22->Add(itemStaticLine25, 10, wxGROW|wxALL, 0);

   itemBoxSizer22->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxStdDialogButtonSizer* itemStdDialogButtonSizer30 = new wxStdDialogButtonSizer;

   itemBoxSizer2->Add(itemStdDialogButtonSizer30, 0, wxGROW|wxALL, 5);
   wxButton* itemButton31 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStdDialogButtonSizer30->AddButton(itemButton31);

   wxButton* itemButton32 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStdDialogButtonSizer30->AddButton(itemButton32);

   itemStdDialogButtonSizer30->Realize();

   CreateEvents();

   Centre();
   ShowModal();
   Destroy();
}

void SellAssetDialog::CreateEvents()
{
   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SellAssetDialog::OnOk));
}

void SellAssetDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto seller_v = p_GWallet->strings.accounts[seller->GetCurrentSelection()].ToStdString();
   const auto sell_amount_v = sell_amount->GetValue().ToStdString();
   const auto sell_asset_v = p_GWallet->strings.assets[sell_asset->GetCurrentSelection()].ToStdString();
   const auto receive_amount_v = receive_amount->GetValue().ToStdString();
   const auto receive_asset_v = receive_asset->GetValue().ToStdString();
   const auto fill_or_kill_v = receive_asset->GetValue();
   bool fill_or_kill_vv;
   if(fill_or_kill_v == "true")
      fill_or_kill_vv = true;
   else
      fill_or_kill_vv = false;

   const auto date_v = date->GetValue().ToUTC().GetValue().ToLong()/1000;
   const auto time_second = time->GetValue().ToUTC().GetSecond();
   const auto time_minute = time->GetValue().ToUTC().GetMinute();
   const auto time_hour = time->GetValue().ToUTC().GetHour();

   const auto time_v = (time_hour*3600) + (time_minute*60) + time_second;

   auto now = wxDateTime::Now().ToUTC().GetValue().ToLong()/1000;

   try
   {
      p_GWallet->bitshares.wallet_api_ptr->get_asset(receive_asset_v);
   }
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(_("Asset is invalid"));
      receive_asset->SetFocus();
      return;
   }

   try {
      auto result = p_GWallet->bitshares.wallet_api_ptr->sell_asset(seller_v, sell_amount_v, sell_asset_v,
            receive_amount_v, receive_asset_v, date_v + time_v - now, fill_or_kill_vv, false);

      if (wxYES == wxMessageBox(fc::json::to_pretty_string(result.operations[0]), _("Confirm Sell Asset?"),
                                wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
         p_GWallet->bitshares.wallet_api_ptr->sell_asset(seller_v, sell_amount_v, sell_asset_v, receive_amount_v,
               receive_asset_v, date_v + time_v - now, fill_or_kill_vv, true);
      }
   }
   catch (const fc::exception &e) {
      p_GWallet->OnError(e.to_detail_string());
   }
}


