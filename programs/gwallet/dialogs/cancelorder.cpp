#include "../include/dialogs/cancelorder.hpp"
#include "../include/modes/wallet.hpp"

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/combo.h>

CancelOrderDialog::CancelOrderDialog(wxWindow* parent, wxWindowID id, const wxString& title,
      const wxPoint &position, const wxSize& size, long style) : wxDialog( parent, id, title, position, size, style)
{
   Wallet* p_Wallet = dynamic_cast<Wallet*>(GetParent());
   p_GWallet = p_Wallet->p_GWallet;

   CancelOrderDialog* itemDialog1 = this;

   DoOpenOrders();

   wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
   itemDialog1->SetSizer(itemBoxSizer2);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

   wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Cancel an open order"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer5, 0, wxGROW|wxALL, 0);

   itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC,
         _("Order to cancel"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer5->Add(itemStaticText7, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer9, 0, wxGROW|wxALL, 0);

   itemBoxSizer9->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   order = new wxListBox( itemDialog1, wxID_ANY,
         wxDefaultPosition, wxDefaultSize, open_orders_strings, wxLB_SINGLE );
   itemBoxSizer9->Add(order, 10, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer9->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer1, 0, wxGROW|wxALL, 0);

   itemBoxSizer1->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   broadcast = new wxCheckBox( itemDialog1, wxID_ANY,
         _("Broadcast transaction"), wxDefaultPosition, wxDefaultSize, 0 );
   broadcast->SetValue(true);
   broadcast->Enable(false);
   itemBoxSizer1->Add(broadcast, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   cli = new wxCheckBox( itemDialog1, wxID_ANY,
         _("Show output in CLI mode"), wxDefaultPosition, wxDefaultSize, 0 );
   cli->SetValue(false);
   cli->Enable(false);
   itemBoxSizer1->Add(cli, 5, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer1->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
   itemBoxSizer2->Add(itemBoxSizer8, 0, wxGROW|wxALL, 0);

   itemBoxSizer8->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   wxStaticLine* itemStaticLine10 = new wxStaticLine( itemDialog1, wxID_STATIC,
         wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
   itemBoxSizer8->Add(itemStaticLine10, 10, wxGROW|wxALL, 0);

   itemBoxSizer8->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 0);

   itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);

   wxStdDialogButtonSizer* itemStdDialogButtonSizer14 = new wxStdDialogButtonSizer;

   itemBoxSizer2->Add(itemStdDialogButtonSizer14, 0, wxGROW|wxALL, 5);
   wxButton* itemButton15 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
   if(open_orders_strings.IsEmpty())
      itemButton15->Enable(false);
   itemStdDialogButtonSizer14->AddButton(itemButton15);

   wxButton* itemButton16 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
   itemStdDialogButtonSizer14->AddButton(itemButton16);

   itemStdDialogButtonSizer14->Realize();

   CreateEvents();

   Centre();
   ShowModal();
   Destroy();
}

void CancelOrderDialog::CreateEvents()
{
   Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CancelOrderDialog::OnOk));
}

void CancelOrderDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
   const auto order_id = open_orders_ids[order->GetSelection()];

   try {
      auto result = p_GWallet->bitshares.wallet_api_ptr->cancel_order(order_id, false);
      if (wxYES == wxMessageBox(fc::json::to_pretty_string(result.operations[0]), _("Confirm cancel order?"),
                                wxNO_DEFAULT | wxYES_NO | wxICON_QUESTION, this)) {
         p_GWallet->bitshares.wallet_api_ptr->cancel_order(order_id, true);
         Close(true);
      }
   }
   catch (const fc::exception &e) {
      p_GWallet->OnError(e.to_detail_string());
   }
}

void CancelOrderDialog::DoOpenOrders()
{
   auto full = p_GWallet->bitshares.wallet_api_ptr->get_full_account(p_GWallet->strings.selected_account.ToStdString());
   if(full.limit_orders.size() > 0) {
      for (auto &limit_order : full.limit_orders) {

         auto assets = p_GWallet->bitshares.database_api->get_assets({
            std::string(object_id_type(limit_order.amount_for_sale().asset_id)),
            std::string(object_id_type(limit_order.amount_to_receive().asset_id))
         });

         auto pretty_balance_for_sale = p_GWallet->DoPrettyBalance(
               assets[0]->precision, limit_order.amount_for_sale().amount.value);
         auto pretty_balance_to_receive = p_GWallet->DoPrettyBalance(
               assets[1]->precision, limit_order.amount_to_receive().amount.value);

         open_orders_strings.Add(std::string(object_id_type(limit_order.id)) + "- Selling "+ pretty_balance_for_sale + " " +
               assets[0]->symbol + " for " + pretty_balance_to_receive + " " + assets[1]->symbol);
         open_orders_ids.push_back(limit_order.id);
      }
   }
}
