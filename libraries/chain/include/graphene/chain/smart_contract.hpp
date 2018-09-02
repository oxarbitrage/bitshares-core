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
#pragma once

#include <graphene/chain/database.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace graphene { namespace chain {

struct smart_contract_object : public abstract_object<smart_contract_object>
{
   static const uint8_t space_id = protocol_ids;
    static const uint8_t type_id  = smart_contract_object_type;

   //fc::time_point_sec time;
   account_id_type owner;
   //private_key_type private_key;
   std::string private_key;
   std::string script;
   std::string output;
   bool status;
};

struct by_id;
typedef multi_index_container<
   smart_contract_object,
   indexed_by<
      ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >
   >
> smart_contract_multi_index_type;

typedef generic_index<smart_contract_object, smart_contract_multi_index_type> smart_contract_index;


class smart_contract
        {
        public:
            smart_contract();
            virtual ~smart_contract();

            static void processSmartContracts(graphene::chain::database* database);

    static std::string getBalance( std::string account, std::string asset );
    static uint32_t getBlock( );
    static void transfer(uint32_t amount, std::string asset);
    static void wrenBind();
    static std::string wrenScript();

    static graphene::chain::database* _database;

};



        } } // graphene::chain

FC_REFLECT_DERIVED( graphene::chain::smart_contract_object, (graphene::db::object),
                    (owner)(private_key)(script)(output)(status))


