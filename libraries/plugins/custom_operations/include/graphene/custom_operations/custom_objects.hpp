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
#pragma once

#include <boost/multi_index/composite_key.hpp>

#include <graphene/chain/database.hpp>


namespace graphene { namespace custom_operations {

using namespace chain;

#ifndef CUSTOM_OPERATIONS_SPACE_ID
#define CUSTOM_OPERATIONS_SPACE_ID 7
#endif

enum types { account_contact = 0 , create_htlc = 1, take_htlc = 2};

struct account_contact_object : public abstract_object<account_contact_object>
{
   static const uint8_t space_id = CUSTOM_OPERATIONS_SPACE_ID;
   static const uint8_t type_id  = account_contact;

   account_id_type account;
   string name;
   string email;
   string phone;
   string address;
   string company;
   string url;
};

struct htlc_bitshares_eos_object : public abstract_object<htlc_bitshares_eos_object>
{
   static const uint8_t space_id = CUSTOM_OPERATIONS_SPACE_ID;
   static const uint8_t type_id  = create_htlc;

   account_id_type bitshares_account;
   string eos_account;
   asset bitshares_amount;
   string eos_asset;
   uint64_t eos_amount;
   fc::time_point_sec expiration;

   fc::time_point_sec order_time;
   bool active;
   account_id_type taker;
   fc::time_point_sec close_time;

   // ...
};

struct by_custom_id;
struct by_custom_account;
typedef multi_index_container<
      account_contact_object,
      indexed_by<
            ordered_non_unique< tag<by_custom_id>, member< object, object_id_type, &object::id > >,
            ordered_unique< tag<by_custom_account>, member< account_contact_object, account_id_type, &account_contact_object::account > >

      >
> account_contact_multi_index_type;

typedef generic_index<account_contact_object, account_contact_multi_index_type> account_contact_index;

struct by_bitshares_account;
struct by_active;
typedef multi_index_container<
      htlc_bitshares_eos_object,
      indexed_by<
            ordered_non_unique< tag<by_custom_id>, member< object, object_id_type, &object::id > >,
            ordered_unique< tag<by_bitshares_account>, member< htlc_bitshares_eos_object, account_id_type, &htlc_bitshares_eos_object::bitshares_account > >,
            ordered_non_unique<
               tag<by_active>,
               composite_key<
                  htlc_bitshares_eos_object,
                  member<htlc_bitshares_eos_object, bool, &htlc_bitshares_eos_object::active>,
                  member<htlc_bitshares_eos_object, fc::time_point_sec, &htlc_bitshares_eos_object::expiration>
               >
            >

      >
> htlc_orderbook_multi_index_type;

typedef generic_index<htlc_bitshares_eos_object, htlc_orderbook_multi_index_type> htlc_orderbook_index;


} } //graphene::custom_operations


FC_REFLECT_DERIVED( graphene::custom_operations::account_contact_object, (graphene::db::object),
                    (account)(name)(email)(phone)(address)(company)(url))
FC_REFLECT_DERIVED( graphene::custom_operations::htlc_bitshares_eos_object, (graphene::db::object),
                    (bitshares_account)(eos_account)(bitshares_amount)(eos_asset)(eos_amount)(expiration)
                    (order_time)(active)(taker)(close_time))
FC_REFLECT_ENUM( graphene::custom_operations::types, (account_contact)(create_htlc)(take_htlc) )


