#include "../include/dialogs/importkey.hpp"
#include "../include/gwallet.hpp"

#include <wx/wx.h>

void ImportKeyDialog::OnOk(wxCommandEvent &WXUNUSED(event)) {

   if(!account->IsEmpty() && !key->IsEmpty()) {
      const auto acct = account->GetValue();
      const auto pkey = key->GetValue();

      GWallet *p_GWallet = dynamic_cast<GWallet *>(GetParent());

      try {
         p_GWallet->bitshares.wallet_api_ptr->import_key(acct.ToStdString(), pkey.ToStdString());
      }
      catch (const fc::exception &e) {
         //p_GWallet->OnError(_("Account/Key pair is invalid, please try again."));
         p_GWallet->OnError(e.to_detail_string());
         return;
      }

      p_GWallet->config->Write("AllSet", true);
      p_GWallet->state.is_account_linked = true;
      p_GWallet->config->Flush();

      p_GWallet->DoAccounts();

      Close(true);

      wxMessageDialog dialog(NULL, _("Account imported"), _("Success"), wxNO_DEFAULT | wxOK | wxICON_INFORMATION);
      if (dialog.ShowModal() == wxID_OK)
         return;
   } else {
      wxMessageDialog dialog(NULL, _("Account and key can not be empty"), _("Error"),
                             wxNO_DEFAULT | wxOK | wxICON_ERROR);
      if (dialog.ShowModal() == wxID_OK)
         return;
   }
}
