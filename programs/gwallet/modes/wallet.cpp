#include "../include/modes/wallet.hpp"
#include "../include/dialogs/transfer.hpp"

Wallet::Wallet(GWallet* gwallet) : wxFrame()
{
   p_GWallet = gwallet;
}

void Wallet::CreateControls()
{
   p_GWallet->sizerWalletMode = new wxBoxSizer(wxVERTICAL);
   p_GWallet->mainSizer->Add(p_GWallet->sizerWalletMode, 0, wxGROW|wxALL);

   wxStaticBox* itemStaticBoxSizer1Static = new wxStaticBox(p_GWallet->panel, wxID_ANY, _("Select a Command"));
   wxStaticBoxSizer* itemStaticBoxSizer1 = new wxStaticBoxSizer(itemStaticBoxSizer1Static, wxVERTICAL);

   wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer2, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText3 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC, _("Transfer an amount from one account to another."), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer2->Add(itemStaticText3, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxButton* itemButton4 = new wxButton( itemStaticBoxSizer1->GetStaticBox(), ID_WALLET_TRANSFER, _("Transfer"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer2->Add(itemButton4, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer5, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText6 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC, _("Place a limit order attempting to sell one asset for another."), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer5->Add(itemStaticText6, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxButton* itemButton7 = new wxButton( itemStaticBoxSizer1->GetStaticBox(), ID_WALLET_SELLASSET, _("Sell asset"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer5->Add(itemButton7, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer8, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText9 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC, _("Borrow an asset or update the debt/collateral ratio for the loan."), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer8->Add(itemStaticText9, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxButton* itemButton10 = new wxButton( itemStaticBoxSizer1->GetStaticBox(), ID_WALLET_BORROWASSET, _("Borrow asset"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer8->Add(itemButton10, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer11, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText12 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC, _("Cancel an existing order."), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer11->Add(itemStaticText12, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxButton* itemButton13 = new wxButton( itemStaticBoxSizer1->GetStaticBox(), ID_WALLET_CANCELORDER, _("Cancel order"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer11->Add(itemButton13, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
   itemStaticBoxSizer1->Add(itemBoxSizer14, 0, wxGROW|wxALL, 5);

   wxStaticText* itemStaticText15 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC, _("Set the voting proxy for an account."), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer14->Add(itemStaticText15, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   wxButton* itemButton16 = new wxButton( itemStaticBoxSizer1->GetStaticBox(), ID_WALLET_SETPROXY, _("Update proxy"), wxDefaultPosition, wxDefaultSize, 0 );
   itemBoxSizer14->Add(itemButton16, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   p_GWallet->sizerWalletMode->Add(itemStaticBoxSizer1, 0, wxALL, 5);

   p_GWallet->mainSizer->Hide(p_GWallet->sizerWalletMode, true);
   p_GWallet->mainSizer->Layout();
}

void Wallet::CreateEvents()
{
   p_GWallet->panel->Connect(ID_WALLET_TRANSFER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnTransfer), NULL, this);
   p_GWallet->panel->Connect(ID_WALLET_SELLASSET, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnSellAsset), NULL, this);
   p_GWallet->panel->Connect(ID_WALLET_BORROWASSET, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnBorrowAsset), NULL, this);
   p_GWallet->panel->Connect(ID_WALLET_CANCELORDER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnCancelOrder), NULL, this);
   p_GWallet->panel->Connect(ID_WALLET_SETPROXY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Wallet::OnSetProxy), NULL, this);
}

void Wallet::OnTransfer(wxCommandEvent& event)
{
   TransferDialog TransferDialog( this, -1, _("Transfer asset"), wxDefaultPosition, wxSize(-1, 500) );
}

void Wallet::OnSellAsset(wxCommandEvent& event)
{

}

void Wallet::OnBorrowAsset(wxCommandEvent& event)
{

}

void Wallet::OnCancelOrder(wxCommandEvent& event)
{

}

void Wallet::OnSetProxy(wxCommandEvent& event)
{

}
