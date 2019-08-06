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

custom_generic_evaluator::custom_generic_evaluator(database& db)
{
   _db = &db;
}

void_result custom_generic_evaluator::do_evaluate(const account_contact_operation& op)
{
   return void_result();
}

object_id_type custom_generic_evaluator::do_apply(const account_contact_operation& op)
{
   auto &index = _db->get_index_type<account_contact_index>().indices().get<by_custom_account>();

   auto itr = index.find(op.account);
   if( itr != index.end() )
   {
      _db->modify( *itr, [&]( account_contact_object& aco ){
         aco.account = op.account;
         aco.name = *op.extensions.value.name;
         aco.email = *op.extensions.value.email;
         aco.phone = *op.extensions.value.phone;
         aco.address = *op.extensions.value.address;
         aco.company = *op.extensions.value.company;
         aco.url = *op.extensions.value.url;
      });
      return itr->id;
   }
   else
   {
      auto created = _db->create<account_contact_object>( [&]( account_contact_object& aco ) {
         aco.account = op.account;
         aco.name = *op.extensions.value.name;
         aco.email = *op.extensions.value.email;
         aco.phone = *op.extensions.value.phone;
         aco.address = *op.extensions.value.address;
         aco.company = *op.extensions.value.company;
         aco.url = *op.extensions.value.url;
      });
      return created.id;
   }
}

void_result custom_generic_evaluator::do_evaluate(const create_htlc_order_operation& op)
{
   return void_result();
}

object_id_type custom_generic_evaluator::do_apply(const create_htlc_order_operation& op)
{
   auto created = _db->create<htlc_order_object>( [&]( htlc_order_object& hbeo ) {
      hbeo.bitshares_account = op.bitshares_account;
      hbeo.blockchain = op.blockchain;
      hbeo.blockchain_account = *op.extensions.value.blockchain_account;
      hbeo.bitshares_amount = *op.extensions.value.bitshares_amount;
      hbeo.blockchain_asset = *op.extensions.value.blockchain_asset;
      hbeo.blockchain_amount = *op.extensions.value.blockchain_amount;
      hbeo.expiration = *op.extensions.value.expiration;
      hbeo.order_time = _db->head_block_time();
   });
   return created.id;
}

void_result custom_generic_evaluator::do_evaluate(const take_htlc_order_operation& op)
{
   return void_result();
}

object_id_type custom_generic_evaluator::do_apply(const take_htlc_order_operation& op)
{
   auto &index = _db->get_index_type<htlc_orderbook_index>().indices().get<by_custom_id>();

   auto itr = index.find(op.htlc_order_id);
   if( itr != index.end() )
   {
      _db->modify( *itr, [&]( htlc_order_object& htlc_object ){
         htlc_object.bitshares_account = itr->bitshares_account;
         htlc_object.blockchain = itr->blockchain;
         htlc_object.blockchain_account = itr->blockchain_account;
         htlc_object.bitshares_amount = itr->bitshares_amount;
         htlc_object.blockchain_asset = itr->blockchain_asset;
         htlc_object.blockchain_amount = itr->blockchain_amount;
         htlc_object.expiration = itr->expiration;
         htlc_object.order_time = itr->order_time;
         htlc_object.active = false;
         htlc_object.taker_bitshares_account = op.bitshares_account;
         htlc_object.taker_eos_account = *op.extensions.value.blockchain_account;
         htlc_object.close_time = _db->head_block_time();
      });
      return itr->id;
   }
}

} }
