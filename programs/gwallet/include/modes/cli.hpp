#include <wx/wx.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class Cli : public wxPanel
{
public:
    Cli(GWallet* gwallet);

   Cli(wxWindow *parent=NULL){
      InitWidgetsFromXRC((wxWindow *)parent);
   }
   GWallet* p_GWallet;

protected:
   wxTextCtrl* command;
   wxButton* run;
   wxButton* clear;
   wxTextCtrl* output;

private:
   void OnCliCommand(wxCommandEvent& event);
   void OnCliClear(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Cli"), wxT("wxPanel"));
      command = XRCCTRL(*this,"command",wxTextCtrl);
      run = XRCCTRL(*this,"run",wxButton);
      clear = XRCCTRL(*this,"clear",wxButton);
      output = XRCCTRL(*this,"output",wxTextCtrl);
   }
};
