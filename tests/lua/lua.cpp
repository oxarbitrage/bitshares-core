/*
 * Copyright (c) 2018 oxarbitrage and contributors.
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
#include <graphene/app/api.hpp>
#include <graphene/utilities/tempdir.hpp>
#include <fc/crypto/digest.hpp>
#include "../common/database_fixture.hpp"
#define BOOST_TEST_MODULE Lua VM Tests
#include <boost/test/included/unit_test.hpp>
#include <graphene/lua/lua.hpp>
#include <graphene/utilities/key_conversion.hpp>
using namespace graphene::chain;
using namespace graphene::chain::test;
using namespace graphene::app;
BOOST_FIXTURE_TEST_SUITE( lua_tests, database_fixture )

   BOOST_AUTO_TEST_CASE(lua_test1) {
      try {
         ACTORS( (dan)(bob)(dreceiver1)(dreceiver2)(dreceiver3) )

         create_bitasset("USD", account_id_type());
         transfer(committee_account, dan_id, asset(100000));
         transfer(committee_account, bob_id, asset(200000));

         //uint32_t skip = database::skip_authority_check | database::skip_fork_db | database::skip_validate;

         //generate_block( skip );
         generate_block( );

         // script 1 - get me block number on each block
         std::string script1 = R"(
         print("---- script 1")
         block_num = Bitshares:getCurrentBlock()
         print (block_num)
         if block_num == 6 then Bitshares:quit() end
         )";
         auto created = db.create<graphene::lua::smart_contract_object>( [&]( graphene::lua::smart_contract_object& sco ) {
            sco.owner = dan_id;
            sco.private_key = dan_private_key;
            sco.script = script1;
            sco.output = "";
            sco.status = true;
         });
         generate_block(  );
         generate_block(  );
         generate_block(  );
         generate_block(  );
         generate_block(  );
         generate_block(  );

         // script 2 - get me block number, my balance and get out
         std::string script2 = R"(
         print("---- script 2")
         block_num = Bitshares:getCurrentBlock()
         my_balance = Bitshares:getBalance("dan", "BTS")
         print (block_num)
         print (my_balance)
         Bitshares:quit()
         )";
         created = db.create<graphene::lua::smart_contract_object>( [&]( graphene::lua::smart_contract_object& sco ) {
            sco.owner = dan_id;
            sco.private_key = dan_private_key;
            sco.script = script2;
            sco.output = "";
            sco.status = true;
         });

         generate_block(  );
         generate_block(  );
         generate_block(  );
         generate_block(  );


         // script 3 - get me block number, my balance, execute transfer in specified block then just get my balance
         std::string script3 = R"(
         print("---- script 3")
         block_num = Bitshares:getCurrentBlock()
         my_balance = Bitshares:getBalance("dan", "BTS")
         print (block_num)
         if block_num == 13 then
            Bitshares:transfer("dan", "bob", "100", "1.3.0")
         elseif block_num > 13 then
            print(my_balance)
            Bitshares:quit()
         end
         )";

         created = db.create<graphene::lua::smart_contract_object>( [&]( graphene::lua::smart_contract_object& sco ) {
            sco.owner = dan_id;
            sco.private_key = dan_private_key;
            sco.script = script3;
            sco.output = "";
            sco.status = true;
         });

         generate_block(  );

         auto dan_balance = db.get_balance( dan_id, asset_id_type() );
         BOOST_CHECK_EQUAL(dan_balance.amount.value, 999900);
         wdump((dan_balance));

         generate_block(  );
         generate_block(  );

         // script 4 - pay dividends to a group of accounts
         std::string script4 = R"(
         print("---- script 4")
         receivers = {}
         receivers[1] = "dreceiver1"
         receivers[2] = "dreceiver2"
         receivers[3] = "dreceiver3"

         block_num = Bitshares:getCurrentBlock()
         print (block_num)

         if block_num == 16 then

            for k,v in ipairs(receivers) do
                 -- print(v)
                 Bitshares:transfer("dan", v, "100", "1.3.0")
            end
         elseif block_num > 16 then
            Bitshares:quit()
         end
         )";


         created = db.create<graphene::lua::smart_contract_object>( [&]( graphene::lua::smart_contract_object& sco ) {
            sco.owner = dan_id;
            sco.private_key = dan_private_key;
            sco.script = script4;
            sco.output = "";
            sco.status = true;
         });

         generate_block(  );

         dan_balance = db.get_balance( dan_id, asset_id_type() );
         BOOST_CHECK_EQUAL(dan_balance.amount.value, 999600);

         auto dreceiver1_balance = db.get_balance( dreceiver1_id, asset_id_type() );
         auto dreceiver2_balance = db.get_balance( dreceiver2_id, asset_id_type() );
         auto dreceiver3_balance = db.get_balance( dreceiver3_id, asset_id_type() );
         BOOST_CHECK_EQUAL(dreceiver1_balance.amount.value, 100);
         BOOST_CHECK_EQUAL(dreceiver2_balance.amount.value, 100);
         BOOST_CHECK_EQUAL(dreceiver3_balance.amount.value, 100);

         generate_block(  );

         generate_block(  );

      }
      catch (fc::exception &e) {
         edump((e.to_detail_string()));
         throw;
      }
   }
BOOST_AUTO_TEST_SUITE_END()

