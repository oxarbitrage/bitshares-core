#include <wx/wx.h>

#include "../gwallet.hpp"

class CancelOrderDialog: public wxDialog
{
public:
   CancelOrderDialog(wxWindow* parent);

protected:
   wxListBox* order;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

private:
   GWallet* p_GWallet;

   wxArrayString open_orders_strings;
   vector<object_id_type> open_orders_ids;

   void OnOk(wxCommandEvent& event);
   void DoOpenOrders();

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("CancelOrderDialog"), wxT("wxDialog"));
      order = XRCCTRL(*this,"order",wxListBox);
      broadcast = XRCCTRL(*this,"broadcast",wxCheckBox);
      cli = XRCCTRL(*this,"cli",wxCheckBox);
   }

};
