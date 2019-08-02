/*
 * Copyright (c) 2019 oxarbitrage and contributors.
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
#include <graphene/chain/database.hpp>

#include <graphene/custom_operations/custom_operations_plugin.hpp>
#include <graphene/custom_operations/custom_objects.hpp>
#include <graphene/custom_operations/custom_evaluators.hpp>

namespace graphene { namespace custom_operations {

account_contact_evaluator::account_contact_evaluator(database& db)
{
   _db = &db;
}

void_result account_contact_evaluator::do_evaluate( const account_contact_operation& op )
{
   return void_result();
}

object_id_type account_contact_evaluator::do_apply( const account_contact_operation& op )
{
   auto &index = _db->get_index_type<account_contact_index>().indices().get<by_custom_account>();

   auto itr = index.find(op.account);
   if( itr != index.end() )
   {
      _db->modify( *itr, [&]( account_contact_object& aco ){
         aco.account = op.account;
         aco.name = op.name;
         aco.email = op.email;
         aco.phone = op.phone;
         aco.address = op.address;
         aco.company = op.company;
         aco.url = op.url;
      });
      return itr->id;
   }
   else
   {
      auto created = _db->create<account_contact_object>( [&]( account_contact_object& aco ) {
         aco.account = op.account;
         aco.name = op.name;
         aco.email = op.email;
         aco.phone = op.phone;
         aco.address = op.address;
         aco.company = op.company;
         aco.url = op.url;
      });
      return created.id;
   }
}

create_htlc_eos_evaluator::create_htlc_eos_evaluator(database& db)
{
   _db = &db;
}

void_result create_htlc_eos_evaluator::do_evaluate( const create_htlc_eos_operation& op )
{
   return void_result();
}

object_id_type create_htlc_eos_evaluator::do_apply( const create_htlc_eos_operation& op )
{
   auto created = _db->create<htlc_bitshares_eos_object>( [&]( htlc_bitshares_eos_object& hbeo ) {
      hbeo.bitshares_account = op.bitshares_account;
      hbeo.eos_account = op.eos_account;
      hbeo.bitshares_amount = op.bitshares_amount;
      hbeo.eos_asset = op.eos_asset;
      hbeo.eos_amount = op.eos_amount;
      hbeo.expiration = op.expiration;
      hbeo.order_time = _db->head_block_time();
   });
   return created.id;
}

take_htlc_eos_evaluator::take_htlc_eos_evaluator(database& db)
{
   _db = &db;
}

void_result take_htlc_eos_evaluator::do_evaluate( const take_htlc_eos_operation& op )
{
   return void_result();
}

object_id_type take_htlc_eos_evaluator::do_apply( const take_htlc_eos_operation& op )
{
   auto &index = _db->get_index_type<htlc_orderbook_index>().indices().get<by_custom_id>();

   auto itr = index.find(op.htlc_order_id);
   //wdump((*itr));
   if( itr != index.end() )
   {
      _db->modify( *itr, [&]( htlc_bitshares_eos_object& htlc_object ){
         htlc_object.bitshares_account = itr->bitshares_account;
         htlc_object.eos_account = itr->eos_account;
         htlc_object.bitshares_amount = itr->bitshares_amount;
         htlc_object.eos_asset = itr->eos_asset;
         htlc_object.eos_amount = itr->eos_amount;
         htlc_object.expiration = itr->expiration;
         htlc_object.order_time = itr->order_time;
         htlc_object.active = false;
         htlc_object.taker = op.bitshares_account;
         htlc_object.close_time = _db->head_block_time();

      });
      return itr->id;
   }

}


} }