#include <wx/wx.h>
#include <wx/srchctrl.h>
#include <wx/grid.h>
#include <wx/xrc/xmlres.h>

class GWallet;

class GetOrderBook: public wxPanel
{
public:
   GetOrderBook(GWallet* gwallet);

protected:
   wxSearchCtrl* base;
   wxSearchCtrl* quote;
   wxChoice* limit;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   void OnOk(wxCommandEvent& event);
   void OnSearchAsset(wxCommandEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetOrderBook"), wxT("wxPanel"));
      base = XRCCTRL(*this,"base",wxSearchCtrl);
      quote = XRCCTRL(*this,"quote",wxSearchCtrl);
      limit = XRCCTRL(*this,"limit",wxChoice);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }
};

class GetOrderBookResponse: public wxPanel
{
public:
   GetOrderBookResponse(GWallet* gwallet, wxAny any_response, std::string response_type);
   wxGrid* response_grid;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("GetOrderBookResponse"), wxT("wxPanel"));
      response_grid = XRCCTRL(*this,"response_grid",wxGrid);
   }
};
