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
         aco.name = (op.extensions.value.name) ? *op.extensions.value.name : aco.name;
         aco.email = (op.extensions.value.email) ? *op.extensions.value.email : aco.email;
         aco.phone = (op.extensions.value.phone) ? *op.extensions.value.phone : aco.phone;
         aco.address = (op.extensions.value.address) ? *op.extensions.value.address : aco.address;
         aco.company = (op.extensions.value.company) ? *op.extensions.value.company : aco.company;
         aco.url = (op.extensions.value.url) ? *op.extensions.value.url : aco.url;
      });
      return itr->id;
   }
   else
   {
      auto created = _db->create<account_contact_object>( [&]( account_contact_object& aco ) {
         aco.account = op.account;
         aco.name = (op.extensions.value.name) ? *op.extensions.value.name : "";
         aco.email = (op.extensions.value.email) ? *op.extensions.value.email : "";
         aco.phone = (op.extensions.value.phone) ? *op.extensions.value.phone : "";
         aco.address = (op.extensions.value.address) ? *op.extensions.value.address : "";
         aco.company = (op.extensions.value.company) ? *op.extensions.value.company : "";
         aco.url = (op.extensions.value.url) ? *op.extensions.value.url : "";
      });
      return created.id;
   }
}

void_result custom_generic_evaluator::do_evaluate(const create_htlc_order_operation& op)
{
   FC_ASSERT(_db->get_balance(op.account, op.extensions.value.bitshares_amount->asset_id).amount >
         op.extensions.value.bitshares_amount->amount.value);
   return void_result();
}

object_id_type custom_generic_evaluator::do_apply(const create_htlc_order_operation& op)
{
   auto created = _db->create<htlc_order_object>( [&]( htlc_order_object& hoo ) {
      hoo.bitshares_account = op.account;
      hoo.blockchain = *op.extensions.value.blockchain;
      hoo.blockchain_account = *op.extensions.value.blockchain_account;
      hoo.bitshares_amount = *op.extensions.value.bitshares_amount;
      hoo.blockchain_asset = *op.extensions.value.blockchain_asset;
      hoo.blockchain_amount = *op.extensions.value.blockchain_amount;
      hoo.expiration = *op.extensions.value.expiration;
      if(op.extensions.value.tag.valid())
         hoo.tag = *op.extensions.value.tag;
      hoo.order_time = _db->head_block_time();
      hoo.active = true;
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

   auto htlc_order_id = *op.extensions.value.htlc_order_id;
   auto itr = index.find(htlc_order_id);
   if( itr != index.end() )
   {
      _db->modify( *itr, [&]( htlc_order_object& hoo ){
         hoo.bitshares_account = itr->bitshares_account;
         hoo.blockchain = itr->blockchain;
         hoo.blockchain_account = itr->blockchain_account;
         hoo.bitshares_amount = itr->bitshares_amount;
         hoo.blockchain_asset = itr->blockchain_asset;
         hoo.blockchain_amount = itr->blockchain_amount;
         hoo.expiration = itr->expiration;
         hoo.tag = itr->tag;
         hoo.order_time = itr->order_time;
         hoo.active = false;
         hoo.taker_bitshares_account = op.account;
         hoo.taker_blockchain_account = *op.extensions.value.blockchain_account;
         hoo.close_time = _db->head_block_time();
      });
   }
   return htlc_order_id;
}

} }
