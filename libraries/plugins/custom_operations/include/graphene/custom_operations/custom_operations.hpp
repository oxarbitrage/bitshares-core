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
   account_id_type fee_payer;
   account_id_type account;
   string name;
   string email;
   string phone;
   string address;
   string company;
   string url;

   void validate()const;
};

struct create_htlc_eos_operation : chain::base_operation
{
   account_id_type fee_payer;
   account_id_type bitshares_account;
   string eos_account;
   asset bitshares_amount;
   string eos_asset;
   int eos_amount;
   fc::time_point_sec expiration;

   void validate()const;
};

struct take_htlc_eos_operation : chain::base_operation
{
   account_id_type fee_payer;
   account_id_type bitshares_account;
   string eos_account;
   object_id_type htlc_order_id;
   fc::time_point_sec expiration;

   void validate()const;
};

} } //graphene::custom_operations

FC_REFLECT( graphene::custom_operations::account_contact_operation, (fee_payer)(account)(name)(email)(phone)(address)
            (company)(url) )
FC_REFLECT( graphene::custom_operations::create_htlc_eos_operation, (fee_payer)(bitshares_account)(eos_account)
            (bitshares_amount)(eos_asset)(eos_amount)(expiration) )
FC_REFLECT( graphene::custom_operations::take_htlc_eos_operation, (fee_payer)(bitshares_account)(eos_account)
            (htlc_order_id)(expiration) )