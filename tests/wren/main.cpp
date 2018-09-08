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

#define BOOST_TEST_MODULE Elastic Search Database Tests
#include <boost/test/included/unit_test.hpp>

#include <graphene/wren/wren_plugin.hpp>

#include <graphene/utilities/key_conversion.hpp>

using namespace graphene::chain;
using namespace graphene::chain::test;
using namespace graphene::app;

BOOST_FIXTURE_TEST_SUITE( wren_tests, database_fixture )

BOOST_AUTO_TEST_CASE(wren_test1) {
   try {

      ACTORS( (dan)(bob) )

      generate_block();

      //account_id_type() do 3 ops
      create_bitasset("USD", account_id_type());

      wdump((db.get_chain_id()));

      transfer(committee_account, dan_id, asset(100000));
      transfer(committee_account, bob_id, asset(200000));
      //auto dan = create_account("dan");
      //auto bob = create_account("bob");

      wdump((dan));
      wdump((bob));
      generate_block();



      // create contracts
      std::string script1 = R"(
         System.print(Bitshares.getBalance("dan","1.3.0"))
         System.print(Bitshares.getBalance("bob","1.3.0"))
         System.print(Bitshares.getBlock())
         System.print("hello from test1")
         Bitshares.Transfer("dan", "bob", 1000, "1.3.0")
         //Bitshares.Exit()
   )";
      //fc::optional<fc::ecc::private_key> private_key = graphene::utilities::wif_to_key("5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3");

      //auto pk = private_key_type();
      //if(private_key.valid())
      //   pk = *private_key;

      auto created = db.create<graphene::wren::smart_contract_object>( [&]( graphene::wren::smart_contract_object& sco ) {
          sco.owner = dan_id;
          sco.private_key = dan_private_key;
          sco.script = script1;
          sco.output = "";
          sco.status = true;
      });

      wdump((created));

      std::string script2 = R"(
         System.print(Bitshares.getBlock())
         System.print("hello from test2")
         //Bitshares.Transfer("1.2.0", "1.2.1", 1, "CNY")
   )";
/*
      db.create<graphene::wren::smart_contract_object>( [&]( graphene::wren::smart_contract_object& sco ) {
          sco.owner = bob_id;
          sco.private_key = bob_private_key;
          sco.script = script2;
          sco.output = "";
          sco.status = true;
      });
*/
      generate_block();

      wdump((get_balance(bob_id, asset_id_type(0))));

      generate_block();

      wdump((get_balance(dan_id, asset_id_type(0))));

      generate_block();

      generate_block();

      generate_block();

      generate_block();

      generate_block();

   }
   catch (fc::exception &e) {
      edump((e.to_detail_string()));
      throw;
   }
}


BOOST_AUTO_TEST_SUITE_END()
