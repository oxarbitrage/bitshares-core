#include <wx/wx.h>
#include <wx/wizard.h>
#include <wx/config.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class Welcome1 : public wxWizardPageSimple
{
public:
   Welcome1(wxWizard *parent, GWallet* gwallet);

   GWallet* p_GWallet;
private:

};

class Welcome2 : public wxWizardPageSimple
{
public:
   Welcome2(wxWizard *parent, GWallet* gwallet);
   void OnPath(wxCommandEvent & event);
   void OnWizardPageChanging(wxWizardEvent& event);

   GWallet* p_GWallet;
   wxTextCtrl *pathCtrl;
private:

};
const int ID_WALLETPATH = 1101;
const int ID_WIZARD = 1102;

class Welcome3 : public wxWizardPageSimple
{
public:
   Welcome3(wxWizard *parent, GWallet* gwallet);
   void OnWizardPageChanging(wxWizardEvent& event);
   void OnTestServer(wxCommandEvent& event);

   GWallet* p_GWallet;
   wxTextCtrl *serverCtrl;
   wxTextCtrl *passwordCtrl;
   wxTextCtrl *repeatpasswordCtrl;
private:

};

const int ID_TESTSERVER = 1103;

class Welcome4 : public wxWizardPageSimple
{
public:
   Welcome4(wxWizard *parent, GWallet* gwallet);

   GWallet* p_GWallet;
   wxTextCtrl *accountCtrl;
   wxTextCtrl *keyCtrl;

   void OnWizardPageChanging(wxWizardEvent& event);
   void OnAccount(wxCommandEvent& event);
   void OnKey(wxCommandEvent& event);
private:

};

const int ID_ACCOUNT = 1104;
const int ID_KEY = 1105;
