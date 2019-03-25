#include "../include/modes/cli.hpp"

#include <wx/hyperlink.h>

Cli::Cli(GWallet* gwallet) : wxFrame()
{
   p_GWallet = gwallet;
}

void Cli::CreateControls()
{
   p_GWallet->sizerCommandMode = new wxBoxSizer(wxVERTICAL);
   p_GWallet->mainSizer->Add(p_GWallet->sizerCommandMode, 0, wxGROW|wxALL);

   wxBoxSizer* sizerCommandCtrls = new wxBoxSizer(wxHORIZONTAL);
   p_GWallet->sizerCommandMode->Add(sizerCommandCtrls, 0, wxGROW|wxALL);

   sendCliText = new wxTextCtrl( p_GWallet->panel, ID_CLITEXT, _("about"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
   sizerCommandCtrls->Add(sendCliText, 10, wxGROW|wxALL, 5);

   wxButton* sendCliButton = new wxButton( p_GWallet->panel, ID_CLISEND, _("Run Command"), wxDefaultPosition, wxDefaultSize, 0 );
   sizerCommandCtrls->Add(sendCliButton, 1, wxGROW|wxALL, 5);

   clearCliButton = new wxButton( p_GWallet->panel, ID_CLICLEAR, _("Clear Console"), wxDefaultPosition, wxDefaultSize, 0 );
   sizerCommandCtrls->Add(clearCliButton, 1, wxGROW|wxALL, 5);

   itemTextCLI = new wxTextCtrl( p_GWallet->panel, ID_CLI, wxEmptyString, wxDefaultPosition, wxSize(-1, 300), wxTE_MULTILINE );
   p_GWallet->sizerCommandMode->Add(itemTextCLI, 0, wxGROW|wxALL, 5);

   p_GWallet->mainSizer->Hide(p_GWallet->sizerCommandMode, true);
   p_GWallet->mainSizer->Layout();
}

void Cli::CreateEvents()
{
   p_GWallet->panel->Connect(ID_CLITEXT, wxEVT_TEXT_ENTER, wxCommandEventHandler(Cli::OnCliCommand), NULL, this);
   p_GWallet->panel->Connect(ID_CLISEND, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Cli::OnCliCommand), NULL, this);
   p_GWallet->panel->Connect(ID_CLICLEAR, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Cli::OnCliClear), NULL, this);
}

void Cli::OnCliCommand(wxCommandEvent & WXUNUSED(event))
{
   auto line = sendCliText->GetValue();
   auto api_id = p_GWallet->wallet.wallet_cli->register_api(p_GWallet->wallet.wallet_api);

   try {
      fc::variants line_variants = fc::json::variants_from_string(line.ToStdString());
      auto command = line_variants[0].get_string();
      auto arguments_variants = fc::variants( line_variants.begin()+1,line_variants.end());
      auto output = fc::json::to_pretty_string(p_GWallet->wallet.wallet_cli->receive_call(api_id, command, arguments_variants));

      std::string prompt = "";
      if( p_GWallet->wallet.wallet_api_ptr->is_new() )
      {
         prompt = "new >>> ";
      }
      else
      {
         if( p_GWallet->wallet.wallet_api_ptr->is_locked() ) {
            prompt = "locked >>> ";
         }
         else {
            prompt = "unlocked >>> ";
         }
      }

      itemTextCLI->AppendText(prompt + line + "\n");
      itemTextCLI->AppendText(output + "\n");
      itemTextCLI->AppendText(prompt + "\n");
   }
   catch( const fc::exception& e )
   {
      p_GWallet->OnError(e.to_detail_string());
   }
}

void Cli::OnCliClear(wxCommandEvent & WXUNUSED(event))
{
   itemTextCLI->SetValue("");
}

