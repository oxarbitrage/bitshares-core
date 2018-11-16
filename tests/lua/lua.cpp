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

   BOOST_AUTO_TEST_CASE(lua_contracts) {
      try {
         ACTORS( (dan)(bob)(dreceiver1)(dreceiver2)(dreceiver3)(feedproducer)(borrower)(seller) )

         const auto& bitusd = create_bitasset("USD", feedproducer_id);
         transfer(committee_account, dan_id, asset(100000));
         generate_block();
         transfer(committee_account, bob_id, asset(200000));
         generate_block(  );

         //const auto& eurusd = create_bitasset("EUR", feedproducer_id);
         const auto& core   = asset_id_type()(db);
         generate_block(  );
         generate_blocks(db.get_dynamic_global_properties().next_maintenance_time);
         generate_block(  );


         //uint32_t skip = database::skip_authority_check | database::skip_fork_db | database::skip_validate;

         //generate_block( skip );

         wdump((db.head_block_num()));

         // script 1 - get me block number on each block
         std::string script1 = R"(
         print("---- script 1")
         block_num = Bitshares:getCurrentBlockNumber()
         print (block_num)
         if block_num == 9 then Bitshares:quit() end
         )";
         db.create<graphene::lua::smart_contract_object>( [&]( graphene::lua::smart_contract_object& sco ) {
            sco.owner = dan_id;
            sco.private_key = dan_private_key;
            sco.script = script1;
            sco.output = "";
            sco.status = true;
         });
         generate_block(  );
         wdump((db.head_block_num()));
         generate_block(  );
         generate_block(  );
         generate_block(  );
         generate_block(  );
         generate_block(  );

         // script 2 - get me block number, my balance and get out
         std::string script2 = R"(
         print("---- script 2")
         block_num = Bitshares:getCurrentBlockNumber()
         my_balance = Bitshares:getBalance("dan", "BTS")
         print (block_num)
         print (my_balance)
         Bitshares:quit()
         )";
         db.create<graphene::lua::smart_contract_object>( [&]( graphene::lua::smart_contract_object& sco ) {
            sco.owner = dan_id;
            sco.private_key = dan_private_key;
            sco.script = script2;
            sco.output = "";
            sco.status = true;
         });

         generate_block(  );
         generate_block(  );
         generate_block(  );
         //generate_block(  );


         // script 3 - get me block number, my balance, execute transfer in specified block then just get my balance
         std::string script3 = R"(
         print("---- script 3")
         block_num = Bitshares:getCurrentBlockNumber()
         my_balance = Bitshares:getBalance("dan", "BTS")
         print (block_num)
         if block_num == 17 then
            Bitshares:transfer("dan", "bob", "100", "1.3.0")
         elseif block_num > 17 then
            print(my_balance)
            Bitshares:quit()
         end
         )";

         db.create<graphene::lua::smart_contract_object>( [&]( graphene::lua::smart_contract_object& sco ) {
            sco.owner = dan_id;
            sco.private_key = dan_private_key;
            sco.script = script3;
            sco.output = "";
            sco.status = true;
         });

         generate_block(  );


         auto dan_balance = db.get_balance( dan_id, asset_id_type() );
         BOOST_CHECK_EQUAL(dan_balance.amount.value, 499900);
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

         block_num = Bitshares:getCurrentBlockNumber()
         print (block_num)

         if block_num == 20 then

            for k,v in ipairs(receivers) do
                 -- print(v)
                 Bitshares:transfer("dan", v, "100", "1.3.0")
            end
         elseif block_num > 20 then
            Bitshares:quit()
         end
         )";


         db.create<graphene::lua::smart_contract_object>( [&]( graphene::lua::smart_contract_object& sco ) {
            sco.owner = dan_id;
            sco.private_key = dan_private_key;
            sco.script = script4;
            sco.output = "";
            sco.status = true;
         });

         generate_block(  );
         wdump((db.head_block_num()));

         dan_balance = db.get_balance( dan_id, asset_id_type() );
         BOOST_CHECK_EQUAL(dan_balance.amount.value, 499600);

         auto dreceiver1_balance = db.get_balance( dreceiver1_id, asset_id_type() );
         auto dreceiver2_balance = db.get_balance( dreceiver2_id, asset_id_type() );
         auto dreceiver3_balance = db.get_balance( dreceiver3_id, asset_id_type() );
         BOOST_CHECK_EQUAL(dreceiver1_balance.amount.value, 100);
         BOOST_CHECK_EQUAL(dreceiver2_balance.amount.value, 100);
         BOOST_CHECK_EQUAL(dreceiver3_balance.amount.value, 100);

         generate_block(  );

         generate_block(  );

         // script 5 - get_ticker - dont work in a test case
         std::string script5 = R"(
         print("---- script 5")

         block_num = Bitshares:getCurrentBlockNumber()
         print (block_num)

         if block_num == 23 then
            print(Bitshares:getTicker("BTS", "USD"))
         elseif block_num > 23 then
            Bitshares:quit()
         end
         )";


         db.create<graphene::lua::smart_contract_object>( [&]( graphene::lua::smart_contract_object& sco ) {
            sco.owner = dan_id;
            sco.private_key = dan_private_key;
            sco.script = script5;
            sco.output = "";
            sco.status = true;
         });

         generate_block(  );

         //current_feed.settlement_price = bitusd.amount( 2 ) / core.amount(5);
         //publish_feed( bitusd, feedproducer, current_feed );

         generate_block(  );

         // script 6 - use json (https://github.com/rxi/json.lua)
         std::string script6 = R"(
         package.path = package.path .. ";/home/alfredo/CLionProjects/lua/tests/lua/?.lua"
         json = require "json"
         print("---- script 6")

         block_num = Bitshares:getCurrentBlockNumber()
         print (block_num)

         if block_num == 25 then

            block = Bitshares:getCurrentBlock()
            print (block)

            myjson = json.decode(block)
            print (myjson["timestamp"])

            for k,v in pairs(myjson["transactions"][1]["operations"][1][2]) do
               print(k,v)
               if type(myjson["transactions"][1]["operations"][1][2][k]) == "table" then
                  for k2,v2 in pairs(myjson["transactions"][1]["operations"][1][2][k]) do
                     print(k2, v2)
                  end
               end
            end
            print (myjson["transactions"][1]["operations"][1][2])
         elseif block_num > 25 then
            Bitshares:quit()
         end
         )";

         db.create<graphene::lua::smart_contract_object>( [&]( graphene::lua::smart_contract_object& sco ) {
            sco.owner = dan_id;
            sco.private_key = dan_private_key;
            sco.script = script6;
            sco.output = "";
            sco.status = true;
         });

         generate_block();
         transfer(committee_account, bob_id, asset(200000));
         generate_block();
         generate_block();


         generate_block(  );
         generate_blocks(db.get_dynamic_global_properties().next_maintenance_time);
         generate_block(  );

         // script 7 - use persistence - http://lua-users.org/wiki/TablePersistence
         std::string script7 = R"(
         package.path = package.path .. ";/home/alfredo/CLionProjects/lua/tests/lua/?.lua"
         json = require "persistence"
         print("---- script 7")

         block_num = Bitshares:getCurrentBlockNumber()
         print (block_num)

         if block_num == 32 then

            t_original = {"test"};
            persistence.store("/home/alfredo/CLionProjects/lua/tests/lua/storage.lua", t_original);

         elseif block_num > 32 then
            t_restored = persistence.load("/home/alfredo/CLionProjects/lua/tests/lua/storage.lua");
            print(t_restored[1])
            Bitshares:quit()
         end
      )";

         db.create<graphene::lua::smart_contract_object>( [&]( graphene::lua::smart_contract_object& sco ) {
            sco.owner = dan_id;
            sco.private_key = dan_private_key;
            sco.script = script7;
            sco.output = "";
            sco.status = true;
         });
         generate_block(  );
         generate_blocks(db.get_dynamic_global_properties().next_maintenance_time);
         generate_block(  );
         generate_block();



         generate_block();
         //wdump((db.head_block_num()));
         generate_block();
         generate_block();
         generate_block();



      }
      catch (fc::exception &e) {
         edump((e.to_detail_string()));
         throw;
      }
   }
   BOOST_AUTO_TEST_CASE(lua_api) {
      try {
         ACTORS((bob))

         generate_block();
         generate_blocks(db.get_dynamic_global_properties().next_maintenance_time);
         generate_block();
         // script 1 - get me block number on each block
         std::string script1 = R"(
         print("hello world")
         )";
         graphene::app::smart_contract_api smart_contract_api(app);

         // upload
         auto up = smart_contract_api.upload_contract(bob_id, graphene::utilities::key_to_wif(bob_private_key), script1, true);
         BOOST_CHECK_EQUAL(up.instance(), 0);
         generate_block();

         // loop all
         auto scs = smart_contract_api.get_contracts();

         // fix this string casting
         BOOST_CHECK_EQUAL(fc::json::to_string(scs.begin()->id), "\"9.0.0\"");
         BOOST_CHECK_EQUAL(fc::json::to_string(scs.begin()->owner), "\"1.2.16\"");

         // get simple
         graphene::lua::smart_contract_object sc = smart_contract_api.get_contract(up);
         BOOST_CHECK_EQUAL(fc::json::to_string(sc.id), "\"9.0.0\"");
         BOOST_CHECK_EQUAL(fc::json::to_string(sc.owner), "\"1.2.16\"");

         generate_block();

         // update contract - stop executing
         auto up2 = smart_contract_api.update_contract(up, bob_id, graphene::utilities::key_to_wif(bob_private_key), script1, false);
         BOOST_CHECK_EQUAL(up2, true);

         generate_block();

         sc = smart_contract_api.get_contract(up);
         BOOST_CHECK_EQUAL(sc.status, false);

         // contract should not be executing anymore
         generate_block();
         generate_block();
      }
      catch (fc::exception &e) {
         edump((e.to_detail_string()));
         throw;
      }
   }

   BOOST_AUTO_TEST_CASE(lua_syntax_error) {
      try {
         ACTORS((bob))

         // script with syntax errors
         std::string script = R"(
            asyntaxerror;;ff
         )";
         graphene::app::smart_contract_api smart_contract_api(app);

         // upload fail
         GRAPHENE_REQUIRE_THROW( smart_contract_api.upload_contract(bob_id, graphene::utilities::key_to_wif(bob_private_key), script, true), fc::exception );
      }
      catch (fc::exception &e) {
         edump((e.to_detail_string()));
         throw;
      }
   }

BOOST_AUTO_TEST_SUITE_END()
