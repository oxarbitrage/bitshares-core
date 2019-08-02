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
#include <graphene/custom_operations/custom_objects.hpp>
#include <graphene/custom_operations/custom_operations.hpp>

namespace graphene { namespace custom_operations {

class account_contact_evaluator
{
   public:
      typedef account_contact_operation operation_type;
      database* _db;
      account_contact_evaluator(database& db);

      void_result do_evaluate( const account_contact_operation& o );
      object_id_type do_apply( const account_contact_operation& o ) ;
};

class create_htlc_eos_evaluator
{
   public:
      typedef create_htlc_eos_operation operation_type;
      database* _db;
      create_htlc_eos_evaluator(database& db);

      void_result do_evaluate( const create_htlc_eos_operation& o );
      object_id_type do_apply( const create_htlc_eos_operation& o ) ;
};

class take_htlc_eos_evaluator
{
   public:
      typedef take_htlc_eos_operation operation_type;
      database* _db;
      take_htlc_eos_evaluator(database& db);

      void_result do_evaluate( const take_htlc_eos_operation& o );
      object_id_type do_apply( const take_htlc_eos_operation& o ) ;
};

} }

FC_REFLECT_TYPENAME( graphene::custom_operations::account_contact_evaluator )
FC_REFLECT_TYPENAME( graphene::custom_operations::create_htlc_eos_evaluator )
FC_REFLECT_TYPENAME( graphene::custom_operations::take_htlc_eos_evaluator )

