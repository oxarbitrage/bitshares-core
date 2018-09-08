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

#include <graphene/utilities/key_conversion.hpp>

//static graphene::wren::detail::wren_plugin_impl* pointer;
static graphene::chain::object_id_type contract_id;

graphene::chain::database* dat;

namespace graphene { namespace wren {

namespace detail
{

class wren_plugin_impl
{
   public:
      wren_plugin_impl(wren_plugin& _plugin)
         : _self( _plugin )
      {
         //pointer = this;
      }
      virtual ~wren_plugin_impl();


     bool onBlock( const signed_block& b );
      static std::string getBalance( std::string account, std::string asset );
      static std::string getBlock( );
      static void transfer(std::string from, std::string to, uint32_t amount, std::string asset);

      static void createContracts( );

      static const account_object* get_account_from_string( const std::string& name_or_id );
      static const asset_object* get_asset_from_string( const std::string& name_or_id );
      static void wrenBind();
      static std::string wrenScript();


      static void set_operation_fees( signed_transaction& tx, const fee_schedule& s  );
      static bool isChainInSync(fc::time_point_sec block_time);

      static void quit();

      graphene::chain::database& database() const
      {
         return _self.database();
      }

      wren_plugin& _self;

   private:
      // nada
};

wren_plugin_impl::~wren_plugin_impl()
{
   return;
}

bool wren_plugin_impl::onBlock( const signed_block& b )
{

   //auto database = &pointer->database();
   graphene::chain::database& dat = database();

   if(isChainInSync(b.timestamp))
   {
      const auto& sc_ids = dat.get_index_type< smart_contract_index >().indices().get<by_id>();

      wdump((sc_ids.size()));

      for( const smart_contract_object& sc_itr : sc_ids ) {

         if(sc_itr.status) {

            wrenpp::VM vm;

            graphene::wren::detail::wren_plugin_impl::wrenBind();
            auto script = graphene::wren::detail::wren_plugin_impl::wrenScript() + sc_itr.script;

            contract_id = sc_itr.id;

            vm.executeString(script);
         }
      }
   }

   return true;

}

bool wren_plugin_impl::isChainInSync(fc::time_point_sec block_time)
{
   if((fc::time_point::now() - block_time) < fc::seconds(6))
      return true;
   return false;
   //return true;

}


std::string wren_plugin_impl::getBalance( std::string account, std::string asset )
{
   account_id_type account_id = get_account_from_string(account)->id;
   asset_id_type asset_id = get_asset_from_string(asset)->id;

   //auto database = &pointer->database();
   auto database = dat;
   return fc::to_string(database->get_balance(account_id, asset_id).amount.value);
}

const account_object* wren_plugin_impl::get_account_from_string( const std::string& name_or_id )
{
   //auto database = &pointer->database();
   auto database = dat;

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
   //auto database = &pointer->database();
   auto database = dat;

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
   //auto database = &pointer->database();
   auto database = dat;
   return fc::to_string(database->head_block_num());
}

void wren_plugin_impl::transfer(std::string from, std::string to, uint32_t amount, std::string asset)
{

   //auto database = &pointer->database();
   auto database = dat;

   const auto& sc_idx = database->get_index_type<smart_contract_index>();
   const auto& sc_ids = sc_idx.indices().get<by_id>();

   auto contract = sc_ids.find(contract_id);

   if(contract != sc_ids.end())
   {
      signed_transaction trx;

      transfer_operation op;
      op.from = get_account_from_string(from)->id;
      op.to = get_account_from_string(to)->id;
      op.amount.asset_id = get_asset_from_string(asset)->id;
      op.amount.amount = amount;

      try {
         op.validate();
      }
      catch (...){
         FC_THROW("invalid op");
      }

      trx.operations.push_back(op);

      set_operation_fees( trx, database->get_global_properties().parameters.current_fees );
      trx.set_reference_block( database->get_dynamic_global_properties().head_block_id );
      trx.set_expiration( database->get_dynamic_global_properties().time + fc::seconds(8) );

      trx.validate();

      //try {
         trx.sign(contract->private_key, database->get_chain_id());
      //}
      //catch( ... ) {
      //   FC_THROW("prob with the private key");
      //}

      //try {
         database->push_transaction(trx);
      //}
      //atch( ... ) {
       //  FC_THROW("pusging");
      //}

      //wdump((trx));
      database->get_balance(get_account_from_string(from)->id, get_asset_from_string(asset)->id);

      trx.clear();


   }

}

void wren_plugin_impl::quit()
{
   //auto database = &pointer->database();
   auto database = dat;

   const auto& sc_idx = database->get_index_type<smart_contract_index>();
   const auto& sc_ids = sc_idx.indices().get<by_id>();

   const auto contract = sc_ids.find(contract_id);

   if(contract != sc_ids.end())
   {
      database->remove( *contract );
   }

   //pointer = _self;
}


void wren_plugin_impl::set_operation_fees( signed_transaction& tx, const fee_schedule& s  )
{
   for( auto& op : tx.operations )
      s.set_fee(op);
}


void wren_plugin_impl::createContracts()
{
   //auto database = &pointer->database();
   auto database = dat;

   std::string script1 = R"(
         System.print(Bitshares.getBalance("alpha","1.3.0"))
         System.print(Bitshares.getBlock())
         System.print("hello from test1")
         Bitshares.Transfer("nathan", "alpha", 1000, "BTS")
         Bitshares.Exit()
   )";
   fc::optional<fc::ecc::private_key> private_key = graphene::utilities::wif_to_key("5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3");

   auto pk = private_key_type();
   if(private_key.valid())
      pk = *private_key;

   auto obj1 = database->create<smart_contract_object>( [&]( smart_contract_object& sco ) {
       sco.owner = account_id_type(0);
       sco.private_key = pk;
       sco.script = script1;
       sco.output = "";
       sco.status = true;
   });

   std::string script2 = R"(
         System.print(Bitshares.getBlock())
         System.print("hello from test2")
         //Bitshares.Transfer("1.2.0", "1.2.1", 1, "CNY")
   )";
/*
   auto obj2 = database->create<smart_contract_object>( [&]( smart_contract_object& sco ) {
       sco.owner = account_id_type(0);
       sco.private_key = pk;
       sco.script = script2;
       sco.output = "";
       sco.status = true;
   });
   */
}


void wren_plugin_impl::wrenBind() {

   wrenpp::beginModule( "main" )
      .beginClass( "Bitshares" )
         .bindFunction< decltype( &detail::wren_plugin_impl::getBalance), &detail::wren_plugin_impl::getBalance >( true, "getBalance(_,_)")
         .bindFunction< decltype( &detail::wren_plugin_impl::getBlock), &detail::wren_plugin_impl::getBlock >( true, "getBlock()")
         .bindFunction< decltype( &detail::wren_plugin_impl::transfer), &detail::wren_plugin_impl::transfer >( true, "Transfer(_,_,_,_)")
         .bindFunction< decltype( &detail::wren_plugin_impl::quit), &detail::wren_plugin_impl::quit >( true, "Exit()")
      .endClass()
   .endModule();
}

std::string wren_plugin_impl::wrenScript() {
   return R"(
      class Bitshares {
         foreign static getBalance(account, asset)
         foreign static getBlock()
         foreign static Transfer(from, to, amount, asset)
         foreign static Exit()
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
   auto default_priv_key = fc::ecc::private_key::regenerate(fc::sha256::hash(std::string("nathan")));
   cli.add_options()
           ("wren-private-key", boost::program_options::value<vector<string>>()->composing()->multitoken()->
                    DEFAULT_VALUE_VECTOR(std::make_pair(chain::public_key_type(default_priv_key.get_public_key()), graphene::utilities::key_to_wif(default_priv_key))),
            "Tuple of [PublicKey, WIF private key] (may specify multiple times)")
         ;
   cfg.add(cli);
}




void wren_plugin::plugin_initialize(const boost::program_options::variables_map& options)
{
   database().add_index< primary_index< smart_contract_index  > >();
   graphene::wren::detail::wren_plugin_impl::createContracts();


   database().applied_block.connect( [&]( const signed_block& b) {
       //graphene::wren::detail::wren_plugin_impl(this->my.get()->_self);
       graphene::wren::detail::wren_plugin_impl instance(this->my.get()->_self);
       dat = &instance.database();

      if(!my->onBlock(b))
      {
         FC_THROW_EXCEPTION(graphene::chain::plugin_exception, "Error populating ES database, we are going to keep trying.");
      }

   } );

   database().removed_objects.connect([&](const vector<object_id_type>& ids, const vector<const object*>& objs, const flat_set<account_id_type>& impacted_accounts) {
       //graphene::wren::detail::wren_plugin_impl(this->my.get()->_self);
       //pointer = my.get();
       graphene::wren::detail::wren_plugin_impl instance(this->my.get()->_self);
       dat = &instance.database();

   });


   if (options.count("wren-private-key")) {
      //handlePrivateKeys(options["private-key"]);

   }
}

void wren_plugin::plugin_startup()
{

   wdump(("wren plugin started"));

}





} }
