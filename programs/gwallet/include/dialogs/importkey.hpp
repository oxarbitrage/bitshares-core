#include <wx/wx.h>

#include "../gwallet.hpp"

class ImportKeyDialog : public wxDialog {
protected:
   wxTextCtrl* account;
   wxTextCtrl* key;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("ImportKeyDialog"), wxT("wxDialog"));
      account = XRCCTRL(*this,"account",wxTextCtrl);
      key = XRCCTRL(*this,"key",wxTextCtrl);
   }
   void OnOk(wxCommandEvent& event);
public:
   ImportKeyDialog(wxWindow *parent=NULL){
      InitWidgetsFromXRC((wxWindow *)parent);
      Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ImportKeyDialog::OnOk));
      ShowModal();
   }
};

