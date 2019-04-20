#include <wx/wx.h>

#include "../gwallet.hpp"

class CancelOrderDialog: public wxDialog
{
public:
   CancelOrderDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition,
         const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

private:
   GWallet* p_GWallet;

   wxListBox* order;
   wxCheckBox* broadcast;
   wxCheckBox* cli;

   wxArrayString open_orders_strings;
   vector<object_id_type> open_orders_ids;

   void CreateEvents();
   void OnOk(wxCommandEvent& event);
   void DoOpenOrders();
};
