#include <wx/wx.h>
#include <wx/hyperlink.h>

#ifndef GWALLET_H
#include "../gwallet.hpp"
#endif

class Home : public wxPanel
{
public:
   Home(GWallet* gwallet);
   void DoInitialData();
   void DoAccount();

   GWallet* p_GWallet;

protected:
   wxStaticText* head_block_num;
   wxStaticText* head_block_age;
   wxStaticText* next_maintenance_time;
   wxStaticText* partcipation;
   wxStaticText* chain_id;
   wxStaticText* name;
   wxStaticText* id;
   wxStaticText* referrer;
   wxStaticText* registrar;
   wxStaticText* voting_as;
   wxStaticText* total_ops;
   wxStaticText* client_version;
   wxStaticText* graphene_revision;
   wxStaticText* revision_age;
   wxStaticText* fc_revision;
   wxStaticText* fc_revision_age;
   wxStaticText* compile_date;
   wxStaticText* boost_revision;
   wxStaticText* openssl_version;
   wxStaticText* os_build;
   wxStaticText* usd_bts;
   wxStaticText* cny_bts;
   wxStaticText* eur_bts;
   wxStaticText* gold_bts;
   wxStaticText* silver_bts;
   wxStaticText* btc_bts;

private:

   void OnWitness(wxHyperlinkEvent& event);
   void OnCommittee(wxHyperlinkEvent& event);

   void InitWidgetsFromXRC(wxWindow *parent){
      wxXmlResource::Get()->LoadObject(this,parent,wxT("Home"), wxT("wxPanel"));
      head_block_num = XRCCTRL(*this,"head_block_num",wxStaticText);
      head_block_age = XRCCTRL(*this,"head_block_age",wxStaticText);
      next_maintenance_time = XRCCTRL(*this,"next_maintenance_time",wxStaticText);
      partcipation = XRCCTRL(*this,"partcipation",wxStaticText);
      chain_id = XRCCTRL(*this,"chain_id",wxStaticText);
      name = XRCCTRL(*this,"name",wxStaticText);
      id = XRCCTRL(*this,"id",wxStaticText);
      referrer = XRCCTRL(*this,"referrer",wxStaticText);
      registrar = XRCCTRL(*this,"registrar",wxStaticText);
      voting_as = XRCCTRL(*this,"voting_as",wxStaticText);
      total_ops = XRCCTRL(*this,"total_ops",wxStaticText);
      client_version = XRCCTRL(*this,"client_version",wxStaticText);
      graphene_revision = XRCCTRL(*this,"graphene_revision",wxStaticText);
      revision_age = XRCCTRL(*this,"revision_age",wxStaticText);
      fc_revision = XRCCTRL(*this,"fc_revision",wxStaticText);
      fc_revision_age = XRCCTRL(*this,"fc_revision_age",wxStaticText);
      compile_date = XRCCTRL(*this,"compile_date",wxStaticText);
      boost_revision = XRCCTRL(*this,"boost_revision",wxStaticText);
      openssl_version = XRCCTRL(*this,"openssl_version",wxStaticText);
      os_build = XRCCTRL(*this,"os_build",wxStaticText);
      usd_bts = XRCCTRL(*this,"usd_bts",wxStaticText);
      cny_bts = XRCCTRL(*this,"cny_bts",wxStaticText);
      eur_bts = XRCCTRL(*this,"eur_bts",wxStaticText);
      gold_bts = XRCCTRL(*this,"gold_bts",wxStaticText);
      silver_bts = XRCCTRL(*this,"silver_bts",wxStaticText);
      btc_bts = XRCCTRL(*this,"btc_bts",wxStaticText);
   }
};
