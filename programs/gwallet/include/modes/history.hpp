#include <wx/wx.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class History : public wxPanel
{
public:

    History(GWallet* gwallet);
    void DoHistory(std::string account);

    GWallet* p_GWallet;

protected:
   wxGrid* grid;

private:
   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("History"), wxT("wxPanel"));
      grid = XRCCTRL(*this,"grid",wxGrid);
   }

};
