/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
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
#include <graphene/chain/smart_contract.hpp>
#include <Wren++.h>
#include <graphene/chain/database.hpp>

static graphene::chain::database* database_ptr;

namespace graphene { namespace chain {

void smart_contract::processSmartContracts(graphene::chain::database* database)
{

   wdump((database->get_core_asset().symbol));
   database_ptr = database;
   //const auto &sc_idx = d.get_index_type<smart_contract_index>();
   //const auto &sc_ids = sc_idx.indices().get<by_id>();

   //auto sc_itr = sc_ids.begin();

   //wdumps... itr i icr aca porque no entra! hasta manana!
   //wdump((sc_itr));
   //wdump((sc_ids.end()));

   //need to add an operation to upload contracts, by now i am going to add them here.

//   while (sc_itr != sc_ids.end()) {

   std::string script1 = R"(
         System.print(Bitshares.getBalance("1.2.0","1.3.0"))
         System.print(Bitshares.getBlock())
         System.print("hello from test1")
         Bitshares.Transfer(1000, "BTS")
   )";

      wrenpp::VM vm;

      wrenBind();
      //auto script = wrenScript() + sc_itr->script;
   auto script = wrenScript() +script1;


   //contract_id = sc_itr->id;

      auto res = vm.executeString(script);




  //    ++sc_itr;
//   }
}

void smart_contract::wrenBind() {

   wrenpp::beginModule( "main" )
      .beginClass( "Bitshares" )
      .bindFunction< decltype( &smart_contract::getBalance), &smart_contract::getBalance >( true, "getBalance(_,_)")
      .bindFunction< decltype( &smart_contract::getBlock), &smart_contract::getBlock >( true, "getBlock()")
      .bindFunction< decltype( &smart_contract::transfer), &smart_contract::transfer >( true, "Transfer(_,_)")
      .endClass()
   .endModule();
}

std::string smart_contract::getBalance( std::string account, std::string asset )
{
   //const database& d = database();

   //account_id_type account_id = get_account_from_string(account)->id;
   //asset_id_type asset_id = get_asset_from_string(asset)->id;
   account_id_type account_id = account_id_type(0);
   asset_id_type asset_id = asset_id_type(0);

   return fc::to_string(database_ptr->get_balance(account_id, asset_id).amount.value);
}

uint32_t smart_contract::getBlock( )
{
   //const database& d = database();
   //return database().get_core_asset().symbol;
   return database_ptr->head_block_num();
   //return d.
   //return "XX";
}

void smart_contract::transfer(uint32_t amount, std::string asset)
{

   const database& d = database();

           const auto& sc_idx = d.get_index_type<smart_contract_index>();
           const auto& sc_ids = sc_idx.indices().get<by_id>();

           /*
           auto contract = sc_ids.find(contract_id);

           if(contract != sc_ids.end())
           {
              wdump((contract->id));
              wdump((contract->private_key));
              wdump((amount));
              wdump((asset));
           }

           wdump((contract_id));
            */
}

std::string smart_contract::wrenScript() {

   return R"(
      class Bitshares {
         foreign static getBalance(account, asset)
         foreign static getBlock()
         foreign static Transfer(amount, asset)
      }

   )";
}



} } // graphene::chain
