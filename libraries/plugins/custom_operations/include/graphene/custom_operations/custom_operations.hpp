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

#include <graphene/chain/database.hpp>

#include "custom_objects.hpp"

namespace graphene { namespace custom_operations {

using namespace std;
using graphene::protocol::account_id_type;

struct account_contact_operation : chain::base_operation
{
   struct ext
   {
      optional<string> name;
      optional<string> email;
      optional<string> phone;
      optional<string> address;
      optional<string> company;
      optional<string> url;
   };

   account_id_type fee_payer;
   account_id_type account;

   graphene::protocol::extension<ext> extensions;

   void validate()const;
};

struct create_htlc_order_operation : chain::base_operation
{
   struct ext
   {
      optional<string> blockchain_account;
      optional<asset> bitshares_amount;
      optional<string> blockchain_asset;
      optional<uint32_t> blockchain_amount;
      optional<fc::time_point_sec> expiration;
   };

   account_id_type fee_payer;
   account_id_type bitshares_account;
   blockchains blockchain;

   graphene::protocol::extension<ext> extensions;

   void validate()const;
};

struct take_htlc_order_operation : chain::base_operation
{
   struct ext
   {
      optional<string> blockchain_account;
      optional<fc::time_point_sec> expiration;
   };

   account_id_type fee_payer;
   account_id_type bitshares_account;
   object_id_type htlc_order_id;

   graphene::protocol::extension<ext> extensions;

   void validate()const;
};

} } //graphene::custom_operations

FC_REFLECT( graphene::custom_operations::account_contact_operation::ext, (name)(email)(phone)(address)(company)(url) )
FC_REFLECT_TYPENAME( graphene::protocol::extension<graphene::custom_operations::account_contact_operation::ext> )
FC_REFLECT( graphene::custom_operations::account_contact_operation, (fee_payer)(account)(extensions) )

FC_REFLECT( graphene::custom_operations::create_htlc_order_operation::ext, (blockchain_account)(bitshares_amount)
            (blockchain_asset)(blockchain_amount)(expiration) )
FC_REFLECT_TYPENAME( graphene::protocol::extension<graphene::custom_operations::create_htlc_order_operation::ext> )
FC_REFLECT( graphene::custom_operations::create_htlc_order_operation, (fee_payer)(bitshares_account)(blockchain)(extensions) )

FC_REFLECT( graphene::custom_operations::take_htlc_order_operation::ext, (blockchain_account)(expiration) )
FC_REFLECT_TYPENAME( graphene::protocol::extension<graphene::custom_operations::take_htlc_order_operation::ext> )
FC_REFLECT( graphene::custom_operations::take_htlc_order_operation, (fee_payer)(bitshares_account)(htlc_order_id)
            (extensions) )

GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::custom_operations::account_contact_operation )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::custom_operations::create_htlc_order_operation )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::custom_operations::take_htlc_order_operation )
