#include "../include/modes/cli.hpp"

#include <wx/hyperlink.h>

Cli::Cli(GWallet* gwallet) : wxFrame()
{
   p_GWallet = gwallet;
}

void Cli::CreateControls()
{
   p_GWallet->sizers.cli = new wxBoxSizer(wxVERTICAL);
   p_GWallet->sizers.main->Add(p_GWallet->sizers.cli, 0, wxGROW|wxALL);

   wxBoxSizer* sizerCommandCtrls = new wxBoxSizer(wxHORIZONTAL);
   p_GWallet->sizers.cli->Add(sizerCommandCtrls, 0, wxGROW|wxALL);

   sendCliText = new wxTextCtrl( p_GWallet->panel, ID_CLITEXT, _("about"),
         wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
   sizerCommandCtrls->Add(sendCliText, 10, wxGROW|wxALL, 5);

   wxButton* sendCliButton = new wxButton( p_GWallet->panel, ID_CLISEND, _("Run Command"),
         wxDefaultPosition, wxDefaultSize, 0 );
   sizerCommandCtrls->Add(sendCliButton, 1, wxGROW|wxALL, 5);

   clearCliButton = new wxButton( p_GWallet->panel, ID_CLICLEAR, _("Clear Console"),
         wxDefaultPosition, wxDefaultSize, 0 );
   sizerCommandCtrls->Add(clearCliButton, 1, wxGROW|wxALL, 5);

   itemTextCLI = new wxTextCtrl( p_GWallet->panel, ID_CLI, wxEmptyString,
         wxDefaultPosition, wxSize(-1, 500), wxTE_MULTILINE );
   p_GWallet->sizers.cli->Add(itemTextCLI, 0, wxGROW|wxALL, 5);

   p_GWallet->sizers.main->Hide(p_GWallet->sizers.cli, true);
   p_GWallet->sizers.main->Layout();
}

void Cli::CreateEvents()
{
   p_GWallet->panel->Connect(ID_CLITEXT, wxEVT_TEXT_ENTER,
         wxCommandEventHandler(Cli::OnCliCommand), NULL, this);
   p_GWallet->panel->Connect(ID_CLISEND, wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(Cli::OnCliCommand), NULL, this);
   p_GWallet->panel->Connect(ID_CLICLEAR, wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(Cli::OnCliClear), NULL, this);
}

void Cli::OnCliCommand(wxCommandEvent & WXUNUSED(event))
{
   const auto line = sendCliText->GetValue();
   const auto wallet_api = fc::api<graphene::wallet::wallet_api>(p_GWallet->bitshares.wallet_api_ptr);
   const auto api_id = p_GWallet->bitshares.wallet_cli->register_api(wallet_api);

   try {
      const fc::variants line_variants = fc::json::variants_from_string(line.ToStdString());
      const auto command = line_variants[0].get_string();
      const auto arguments_variants = fc::variants( line_variants.begin()+1,line_variants.end());
      const auto response = p_GWallet->bitshares.wallet_cli->receive_call(api_id, command, arguments_variants);
      const auto output = fc::json::to_pretty_string(response);

      std::string prompt = "";
      if( p_GWallet->bitshares.wallet_api_ptr->is_new() )
      {
         prompt = "new >>> ";
      }
      else
      {
         if( p_GWallet->bitshares.wallet_api_ptr->is_locked() ) {
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
   catch(const fc::exception& e)
   {
      p_GWallet->OnError(e.to_detail_string());
   }
}

void Cli::OnCliClear(wxCommandEvent & WXUNUSED(event))
{
   itemTextCLI->SetValue("");
}
