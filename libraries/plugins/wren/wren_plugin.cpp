/*
 * Copyright (c) 2018 oxarbitrage, and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <graphene/wren/wren_plugin.hpp>
#include <graphene/chain/impacted.hpp>
#include <graphene/chain/account_evaluator.hpp>
#include <fc/smart_ref_impl.hpp>

#include <graphene/chain/database.hpp>

#include <Wren++.h>

static graphene::wren::detail::wren_plugin_impl* pointer;
static graphene::chain::object_id_type contract_id;

namespace graphene { namespace wren {

namespace detail
{

class wren_plugin_impl
{
   public:
      wren_plugin_impl(wren_plugin& _plugin)
         : _self( _plugin )
      {
         pointer = this;

      }
      virtual ~wren_plugin_impl();

      bool onBlock( const signed_block& b );
      static std::string getBalance( std::string account, std::string asset );
      static std::string getBlock( );
      static void transfer(uint32_t amount, std::string asset);

      static void createContracts( );

      static const account_object* get_account_from_string( const std::string& name_or_id );
      static const asset_object* get_asset_from_string( const std::string& name_or_id );
      static void wrenBind();
      static std::string wrenScript();

      graphene::chain::database& database() const
      {
         return _self.database();
      }

      wren_plugin& _self;
      std::string _wren_option = "";

   private:
      // nada
};

wren_plugin_impl::~wren_plugin_impl()
{
   return;
}

bool wren_plugin_impl::onBlock( const signed_block& b )
{

   auto database = &pointer->database();

   const auto& sc_idx = database->get_index_type<smart_contract_index>();
   const auto& sc_ids = sc_idx.indices().get<by_id>();

   auto sc_itr = sc_ids.begin();

   while( sc_itr !=  sc_ids.end())
   {
      wrenpp::VM vm;

      graphene::wren::detail::wren_plugin_impl::wrenBind();
      auto script = graphene::wren::detail::wren_plugin_impl::wrenScript() + sc_itr->script;

      contract_id = sc_itr->id;

      vm.executeString( script );

      ++sc_itr;
   }

   return true;

}

std::string wren_plugin_impl::getBalance( std::string account, std::string asset )
{
   account_id_type account_id = get_account_from_string(account)->id;
   asset_id_type asset_id = get_asset_from_string(asset)->id;

   auto database = &pointer->database();
   return fc::to_string(database->get_balance(account_id, asset_id).amount.value);
}

const account_object* wren_plugin_impl::get_account_from_string( const std::string& name_or_id )
{
   auto database = &pointer->database();

   // Todo: duplicated of database_api.cpp
   FC_ASSERT( name_or_id.size() > 0);
   const account_object* account = nullptr;
   if (std::isdigit(name_or_id[0]))
      account = database->find(fc::variant(name_or_id, 1).as<account_id_type>(1));
   else {
      const auto& idx = database->get_index_type<account_index>().indices().get<by_name>();
      auto itr = idx.find(name_or_id);
      if (itr != idx.end())
         account = &*itr;
   }
   FC_ASSERT( account, "no such account" );
   return account;
}

const asset_object* wren_plugin_impl::get_asset_from_string( const std::string& name_or_id )
{
   auto database = &pointer->database();

   // Todo: duplicated of database_api.cpp
   FC_ASSERT( name_or_id.size() > 0);
   const asset_object* asset = nullptr;
   if (std::isdigit(name_or_id[0]))
      asset = database->find(fc::variant(name_or_id, 1).as<asset_id_type>(1));
   else {
      const auto& idx = database->get_index_type<asset_index>().indices().get<by_symbol>();
      auto itr = idx.find(name_or_id);
      if (itr != idx.end())
         asset = &*itr;
   }
   FC_ASSERT( asset, "no such asset" );
   return asset;
}

std::string wren_plugin_impl::getBlock( )
{
   auto database = &pointer->database();
   return fc::to_string(database->head_block_num());
}

void wren_plugin_impl::transfer(uint32_t amount, std::string asset)
{

   auto database = &pointer->database();

   const auto& sc_idx = database->get_index_type<smart_contract_index>();
   const auto& sc_ids = sc_idx.indices().get<by_id>();

   auto contract = sc_ids.find(contract_id);

   if(contract != sc_ids.end())
   {
      wdump((contract->id));
      wdump((contract->private_key));
      wdump((amount));
      wdump((asset));
   }

   wdump((contract_id));
}

void wren_plugin_impl::createContracts()
{
   auto database = &pointer->database();

   std::string script1 = R"(
         System.print(Bitshares.getBalance("1.2.0","1.3.0"))
         System.print(Bitshares.getBlock())
         System.print("hello from test1")
         Bitshares.Transfer(1000, "BTS")
   )";

   database->create<smart_contract_object>( [&]( smart_contract_object& sco ) {
       sco.owner = account_id_type(0);
       sco.private_key = "XXXX";
       sco.script = script1;
       sco.output = "";
       sco.status = true;
   });

   std::string script2 = R"(
         System.print(Bitshares.getBlock())
         System.print("hello from test2")
         Bitshares.Transfer(1, "CNY")
   )";

   database->create<smart_contract_object>( [&]( smart_contract_object& sco ) {
       sco.owner = account_id_type(0);
       sco.private_key = "YYYY";
       sco.script = script2;
       sco.output = "";
       sco.status = true;
   });

}


void wren_plugin_impl::wrenBind() {

   wrenpp::beginModule( "main" )
      .beginClass( "Bitshares" )
         .bindFunction< decltype( &detail::wren_plugin_impl::getBalance), &detail::wren_plugin_impl::getBalance >( true, "getBalance(_,_)")
         .bindFunction< decltype( &detail::wren_plugin_impl::getBlock), &detail::wren_plugin_impl::getBlock >( true, "getBlock()")
         .bindFunction< decltype( &detail::wren_plugin_impl::transfer), &detail::wren_plugin_impl::transfer >( true, "Transfer(_,_)")
      .endClass()
   .endModule();
}

std::string wren_plugin_impl::wrenScript() {
   return R"(
      class Bitshares {
         foreign static getBalance(account, asset)
         foreign static getBlock()
         foreign static Transfer(amount, asset)
      }

   )";
}

} // end namespace detail

wren_plugin::wren_plugin() :
   my( new detail::wren_plugin_impl(*this) )
{
}

wren_plugin::~wren_plugin()
{
}

std::string wren_plugin::plugin_name()const
{
   return "wren";
}
std::string wren_plugin::plugin_description()const
{
   return "the wren thing";
}

void wren_plugin::plugin_set_program_options(
   boost::program_options::options_description& cli,
   boost::program_options::options_description& cfg
   )
{
   cli.add_options()
         ("wren-option", boost::program_options::value<std::string>(), "a wren optioon")
         ;
   cfg.add(cli);
}

void wren_plugin::plugin_initialize(const boost::program_options::variables_map& options)
{

   database().add_index< primary_index< smart_contract_index  > >();
   graphene::wren::detail::wren_plugin_impl::createContracts();

   database().applied_block.connect( [&]( const signed_block& b) {
      if(!my->onBlock(b))
      {
         FC_THROW_EXCEPTION(graphene::chain::plugin_exception, "Error populating ES database, we are going to keep trying.");
      }
   } );

   if (options.count("wren-option")) {
      my->_wren_option = options["wren-option"].as<std::string>();
   }
}

void wren_plugin::plugin_startup()
{

   //wdump(("here?"));

}





} }
