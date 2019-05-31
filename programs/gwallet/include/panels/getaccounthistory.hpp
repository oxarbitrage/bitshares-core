#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/srchctrl.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class GetAccountHistory: public wxPanel
{
public:
   GetAccountHistory(GWallet* gwallet);

protected:
   wxSearchCtrl* name;
   wxChoice* limit;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchAccount(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetAccountHistory"), wxT("wxPanel"));
      name = XRCCTRL(*this,"name",wxSearchCtrl);
      limit = XRCCTRL(*this,"limit",wxChoice);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class GetAccountHistoryResponse: public wxPanel
{
public:
   GetAccountHistoryResponse(GWallet* gwallet, wxAny any_response);
   wxGrid* response_grid;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetAccountHistoryResponse"), wxT("wxPanel"));
      response_grid = XRCCTRL(*this,"response_grid",wxGrid);
   }
};