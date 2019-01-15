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

#include <graphene/lua/lua.hpp>
#include <fc/smart_ref_impl.hpp>
#include <thread>

extern "C"
{
   #include <lua5.2/lua.h>
   #include <lua5.2/lauxlib.h>
   #include <lua5.2/lualib.h>
}

#include "LuaBridge/LuaBridge.h"

#include <graphene/app/database_api.hpp>
#include <graphene/app/util.hpp>

//#include <graphene/market_history/market_history_plugin.hpp>

using namespace luabridge;

namespace graphene { namespace lua {

namespace detail
{

class lua_plugin_impl
{
   public:

      lua_plugin_impl(lua_plugin& _plugin)
         : _self( _plugin )
      {

      }
      virtual ~lua_plugin_impl();

      lua_plugin& _self;
      std::string _lua_option = "whatever";
      object_id_type processing_contract_id;
      signed_block current_block;

      void newBlock( const signed_block& b );
      uint32_t getCurrentBlockNumber();
      uint32_t getCurrentTime();
      string getCurrentBlock();
      uint32_t getBalance(std::string account, std::string asset);
      std::string getTicker(const std::string& base, const std::string& quote);
      void transfer(std::string from, std::string to, std::string amount, std::string asset);
      void quit();

      graphene::chain::database& database()
      {
         return _self.database();
      }

      lua_State* L;

      const void createSomeContracts();

private:
      const account_object* get_account_from_string( const std::string& name_or_id );
      const asset_object* get_asset_from_string( const std::string& name_or_id );
      string price_to_string( const price& _price, const asset_object& _base, const asset_object& _quote );

};

void lua_plugin_impl::newBlock( const signed_block& b )
{
   graphene::chain::database& db = database();

   current_block= b;
   auto block_num = fc::to_string(b.block_num());

   auto& sc_ids = db.get_index_type< smart_contract_index >().indices().get<by_status>();
   auto itr = sc_ids.find( true );
   while( itr != sc_ids.end() && itr->status) {

      if (itr->status) {

         L = luaL_newstate();
         luaL_openlibs(L);

         getGlobalNamespace (L)
               .beginClass <graphene::lua::detail::lua_plugin_impl> ("Bitshares")
                  .addFunction("getBalance", &graphene::lua::detail::lua_plugin_impl::getBalance)
                  .addFunction("transfer", &graphene::lua::detail::lua_plugin_impl::transfer)
                  .addFunction("quit", &graphene::lua::detail::lua_plugin_impl::quit)
                  .addFunction("getCurrentBlockNumber", &graphene::lua::detail::lua_plugin_impl::getCurrentBlockNumber)
                  .addFunction("getCurrentTime", &graphene::lua::detail::lua_plugin_impl::getCurrentTime)
                  .addFunction("getCurrentBlock", &graphene::lua::detail::lua_plugin_impl::getCurrentBlock)
                  .addFunction("getTicker", &graphene::lua::detail::lua_plugin_impl::getTicker)
               .endClass();

         luabridge::Stack <lua_plugin_impl*>::push (L, this);
         lua_setglobal (L, "Bitshares");

         processing_contract_id = itr->id;

         auto errors = luaL_dostring(L, itr->script.c_str());
         auto stack_top = lua_gettop(L);
         if(stack_top > 0 && errors > 0) // print stack to console if any, for example syntax error
         {
            const char *str = lua_tostring(L, -1);
            wdump((str));

            // need to stop execution
            quit();
         }
         ++itr;
      }
   }
}

uint32_t lua_plugin_impl::getCurrentBlockNumber()
{
   return database().head_block_num();
}

uint32_t lua_plugin_impl::getCurrentTime()
{
    return database().head_block_time().sec_since_epoch();
}

string lua_plugin_impl::getCurrentBlock()
{
   return fc::json::to_string(current_block);
}

uint32_t lua_plugin_impl::getBalance(std::string account, std::string asset) {

   auto account_id = get_account_from_string(account)->id;
   auto asset_id = get_asset_from_string(asset)->id;
   auto balance = database().get_balance(account_id, asset_id);
   return balance.amount.value;
}

std::string lua_plugin_impl::getTicker(const std::string& base, const std::string& quote)
{
   //graphene::app::database_api db_api(database());
   //wdump((db_api.get_global_properties()));
   //auto ticker = db_api.get_ticker(base, quote);
   //wdump((ticker));
   //return ticker.latest;
   auto base_id = get_asset_from_string(base)->id;
   auto quote_id = get_asset_from_string(quote)->id;
   //wdump((base_id));
   //wdump((quote_id));
   if( base_id > quote_id ) std::swap( base_id, quote_id );
   const auto& ticker_idx = database().get_index_type<graphene::market_history::market_ticker_index>().indices().get<graphene::market_history::by_market>();
   auto itr = ticker_idx.find( std::make_tuple( base_id, quote_id ) );
   //market_ticker ticker;
   if( itr != ticker_idx.end() )
   {
      //wdump((*itr));
      price latest_price = asset(itr->latest_base, itr->base) / asset(itr->latest_quote, itr->quote);
      if (itr->base != get_asset_from_string(base)->id)
         latest_price = ~latest_price;
      auto latest = price_to_string(latest_price, *get_asset_from_string(base), *get_asset_from_string(quote));

      return latest;
   }
   return "";
}

void lua_plugin_impl::transfer(std::string from, std::string to, std::string amount, std::string asset)
{
   graphene::chain::database& db = database();

   auto& sc_idx = db.get_index_type<smart_contract_index>();
   auto& sc_ids = sc_idx.indices().get<by_status>();

   auto itr = sc_ids.lower_bound( 1 );
   while( itr != sc_ids.end() && itr->status)
   {
      if(itr->id == processing_contract_id) {

         signed_transaction trx;

         transfer_operation op;
         op.from = get_account_from_string(from)->id;
         op.to = get_account_from_string(to)->id;
         op.amount.asset_id = get_asset_from_string(asset)->id;
         op.amount.amount = std::stoi(amount);

         op.validate();
         trx.operations.push_back(op);

         //set_operation_fees( trx, database->get_global_properties().parameters.current_fees );
         trx.set_reference_block(db.get_dynamic_global_properties().head_block_id);
         trx.set_expiration(db.get_dynamic_global_properties().time + fc::seconds(1));

         trx.validate();

         trx.sign(itr->private_key, database().get_chain_id());

         // Some problems with database state here, need to fix.
         db.push_transaction(trx);
         wdump((trx));


         /*
         Doing it with broadcast segfault in veryfy authorities
         graphene::app::application app;
         auto nb_api = std::make_shared< graphene::app::network_broadcast_api >( app );

         try
         {
            nb_api->broadcast_transaction(trx);
         }
         catch (const fc::exception& e) {
            elog("Caught exception while broadcasting tx ${id}:  ${e}", ("id", trx.id().str())("e", e.to_detail_string()));
            throw;
         }
         */


         //wdump((db.get_balance(get_account_from_string(from)->id, get_asset_from_string(asset)->id)));

         trx.clear();

      }
      itr++;
   }
}
void lua_plugin_impl::quit() {
   graphene::chain::database &db = database();

   auto &sc_idx = db.get_index_type<smart_contract_index>();
   auto &sc_ids = sc_idx.indices().get<by_status>();

   auto itr = sc_ids.lower_bound(1);
   while (itr != sc_ids.end() && itr->status) {
      if (itr->id == processing_contract_id) {
         auto old_itr = itr;
         db.remove(*old_itr);
      }
      ++itr;
   }
}

const account_object* lua_plugin_impl::get_account_from_string( const std::string& name_or_id )
{
   // Todo: duplicated of database_api.cpp
   FC_ASSERT( name_or_id.size() > 0);
   const account_object* account = nullptr;
   if (std::isdigit(name_or_id[0]))
      account = database().find(fc::variant(name_or_id, 1).as<account_id_type>(1));
   else {
      const auto& idx = database().get_index_type<account_index>().indices().get<by_name>();
      auto itr = idx.find(name_or_id);
      if (itr != idx.end())
         account = &*itr;
   }
   FC_ASSERT( account, "no such account" );
   return account;
}

const asset_object* lua_plugin_impl::get_asset_from_string( const std::string& name_or_id )
{
   // Todo: duplicated of database_api.cpp
   FC_ASSERT( name_or_id.size() > 0);
   const asset_object* asset = nullptr;
   if (std::isdigit(name_or_id[0]))
      asset = database().find(fc::variant(name_or_id, 1).as<asset_id_type>(1));
   else {
      const auto& idx = database().get_index_type<asset_index>().indices().get<by_symbol>();
      auto itr = idx.find(name_or_id);
      if (itr != idx.end())
         asset = &*itr;
   }
   FC_ASSERT( asset, "no such asset" );
   return asset;
}

string lua_plugin_impl::price_to_string( const price& _price, const asset_object& _base, const asset_object& _quote )
{ try {
         if( _price.base.asset_id == _base.id && _price.quote.asset_id == _quote.id )
            return graphene::app::price_to_string( _price, _base.precision, _quote.precision );
         else if( _price.base.asset_id == _quote.id && _price.quote.asset_id == _base.id )
            return graphene::app::price_to_string( ~_price, _base.precision, _quote.precision );
         else
            FC_ASSERT( !"bad parameters" );
} FC_CAPTURE_AND_RETHROW( (_price)(_base)(_quote) ) }

const void lua_plugin_impl::createSomeContracts()
{
   std::string script1 = R"(
         print("---- script 1000")
         -- block_num = Bitshares:getCurrentBlock()
         -- print (block_num)
         ticker = Bitshares:getTicker("BTS", "USD")
         print(ticker)

         )";
   auto created = database().create<graphene::lua::smart_contract_object>( [&]( graphene::lua::smart_contract_object& sco ) {
      //sco.owner = dan_id;
      //sco.private_key = dan_private_key;
      sco.script = script1;
      sco.output = "";
      sco.status = true;
   });
}

lua_plugin_impl::~lua_plugin_impl()
{
   return;
}

} // end namespace detail

lua_plugin::lua_plugin() :
   my( new detail::lua_plugin_impl(*this) )
{
}

lua_plugin::~lua_plugin()
{
}

std::string lua_plugin::plugin_name()const
{
   return "lua";
}
std::string lua_plugin::plugin_description()const
{
   return "Do lua stuff.";
}

void lua_plugin::plugin_set_program_options(
   boost::program_options::options_description& cli,
   boost::program_options::options_description& cfg
   )
{
   cli.add_options()
         ("lua-option", boost::program_options::value<std::string>(), "Some lua option")
         ;
   cfg.add(cli);
}

void lua_plugin::plugin_initialize(const boost::program_options::variables_map& options)
{
   database().add_index< primary_index< smart_contract_index  > >();

   // use to load some initial contracts
   //my->createSomeContracts();

   database().applied_block.connect( [&]( const signed_block& b) {
      my->newBlock(b);
   });

   if (options.count("lua-option")) {
      my->_lua_option = options["lua-option"].as<std::string>();
   }
}

void lua_plugin::plugin_startup()
{
}

} }
