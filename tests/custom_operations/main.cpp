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

#include <graphene/app/api.hpp>
#include <graphene/utilities/tempdir.hpp>
#include <fc/crypto/digest.hpp>

#include <graphene/custom_operations/custom_operations_plugin.hpp>

#include "../common/database_fixture.hpp"

#define BOOST_TEST_MODULE Custom operations plugin tests
#include <boost/test/included/unit_test.hpp>

using namespace graphene::chain;
using namespace graphene::chain::test;
using namespace graphene::app;
using namespace graphene::custom_operations;

BOOST_FIXTURE_TEST_SUITE( custom_operation_tests, database_fixture )

BOOST_AUTO_TEST_CASE(custom_operations_account_contact_test)
{
try {
   ACTORS((nathan)(alice));

   custom_operations_api custom_operations_api(app);

   generate_block();

   enable_fees();
   signed_transaction trx;
   set_expiration(db, trx);

   int64_t init_balance(10000 * GRAPHENE_BLOCKCHAIN_PRECISION);

   transfer(committee_account, nathan_id, asset(init_balance));
   transfer(committee_account, alice_id, asset(init_balance));

   // nathan adds account data via custom operation
   {
      custom_operation op;

      account_contact_operation contact;
      contact.account = nathan_id;
      contact.name = "Nathan";
      contact.email = "nathan@nathan.com";
      contact.phone = "+1 434343434343";
      contact.address = "";
      contact.company = "Bitshares";
      contact.url = "http://nathan.com/";

      formatted_custom_operation<account_contact_operation> fco;
      fco.type = types::account_contact;
      fco.data = contact;

      auto packed = fc::raw::pack(fco);

      op.payer = nathan_id;
      op.data = packed;
      op.fee = db.get_global_properties().parameters.current_fees->calculate_fee(op);
      trx.operations.push_back(op);
      sign(trx, nathan_private_key);
      PUSH_TX(db, trx, ~0);
      trx.clear();
   }

   // alice adds account data via custom operation
   {
      custom_operation op;
      account_contact_operation contact;
      contact.account = alice_id;
      contact.name = "Alice";
      contact.email = "alice@alice.com";
      contact.phone = "";
      contact.address = "Some Street 456, Somewhere";
      contact.company = "";
      contact.url = "http://alice.com/";

      formatted_custom_operation<account_contact_operation> fco;
      fco.type = types::account_contact;
      fco.data = contact;

      auto packed = fc::raw::pack(fco);

      op.payer = alice_id;
      op.data = packed;
      op.fee = db.get_global_properties().parameters.current_fees->calculate_fee(op);
      trx.operations.push_back(op);
      sign(trx, alice_private_key);
      PUSH_TX(db, trx, ~0);
      trx.clear();
   }

   generate_block();

   // check nathan account data with the api
   account_contact_object contact_results_nathan = custom_operations_api.get_contact_info("nathan");
   BOOST_CHECK_EQUAL(contact_results_nathan.account.instance.value, 16 );
   BOOST_CHECK_EQUAL(contact_results_nathan.name, "Nathan");
   BOOST_CHECK_EQUAL(contact_results_nathan.email, "nathan@nathan.com");
   BOOST_CHECK_EQUAL(contact_results_nathan.phone, "+1 434343434343");
   BOOST_CHECK_EQUAL(contact_results_nathan.address, "");
   BOOST_CHECK_EQUAL(contact_results_nathan.company, "Bitshares");
   BOOST_CHECK_EQUAL(contact_results_nathan.url, "http://nathan.com/");

   // check alice account data with the api
   account_contact_object contact_results_alice = custom_operations_api.get_contact_info("alice");
   BOOST_CHECK_EQUAL(contact_results_alice.account.instance.value, 17 );
   BOOST_CHECK_EQUAL(contact_results_alice.name, "Alice");
   BOOST_CHECK_EQUAL(contact_results_alice.email, "alice@alice.com");
   BOOST_CHECK_EQUAL(contact_results_alice.phone, "");
   BOOST_CHECK_EQUAL(contact_results_alice.address, "Some Street 456, Somewhere");
   BOOST_CHECK_EQUAL(contact_results_alice.company, "");
   BOOST_CHECK_EQUAL(contact_results_alice.url, "http://alice.com/");

   //set_expiration(db, trx);

   // alice update her data
   {
      custom_operation op;
      account_contact_operation contact;
      contact.account = alice_id;
      contact.name = "Alice Smith";
      contact.email = "alicesmith@alice.com";
      contact.phone = "+1 1111 11 1111";
      contact.address = "Some Street 456, Somewhere";
      contact.company = "";
      contact.url = "http://alice.com/";

      formatted_custom_operation<account_contact_operation> fco;
      fco.type = types::account_contact;
      fco.data = contact;

      auto packed = fc::raw::pack(fco);

      op.payer = alice_id;
      op.data = packed;
      op.fee = db.get_global_properties().parameters.current_fees->calculate_fee(op);
      trx.operations.push_back(op);
      sign(trx, alice_private_key);
      PUSH_TX(db, trx, ~0);
      trx.clear();
   }

   generate_block();

   // check alice account updates with the api
   contact_results_alice = custom_operations_api.get_contact_info("alice");
   BOOST_CHECK_EQUAL(contact_results_alice.account.instance.value, 17 );
   BOOST_CHECK_EQUAL(contact_results_alice.name, "Alice Smith");
   BOOST_CHECK_EQUAL(contact_results_alice.email, "alicesmith@alice.com");
   BOOST_CHECK_EQUAL(contact_results_alice.phone, "+1 1111 11 1111");
   BOOST_CHECK_EQUAL(contact_results_alice.address, "Some Street 456, Somewhere");
   BOOST_CHECK_EQUAL(contact_results_alice.company, "");
   BOOST_CHECK_EQUAL(contact_results_alice.url, "http://alice.com/");

   // alice try to update nathan data
   {
      custom_operation op;
      account_contact_operation contact;
      contact.account = nathan_id;
      contact.name = "Not my account";
      contact.phone = "Fake phone";
      contact.email = "Fake email";

      formatted_custom_operation<account_contact_operation> fco;
      fco.type = graphene::custom_operations::types::account_contact;
      fco.data = contact;

      auto packed = fc::raw::pack(fco);

      op.payer = alice_id;
      op.data = packed;
      op.fee = db.get_global_properties().parameters.current_fees->calculate_fee(op);
      trx.operations.push_back(op);
      sign(trx, alice_private_key);
      PUSH_TX(db, trx, ~0);
      trx.clear();
   }
   generate_block();

   // operation will pass but data will be unchanged, exception was produced in plug in
   contact_results_nathan = custom_operations_api.get_contact_info("nathan");
   BOOST_CHECK(contact_results_nathan.account.instance.value == 16 );
   BOOST_CHECK(contact_results_nathan.name != "Not my account");
   BOOST_CHECK(contact_results_nathan.phone != "Fake phone");
   BOOST_CHECK(contact_results_nathan.email != "Fake email");
}
catch (fc::exception &e) {
   edump((e.to_detail_string()));
   throw;
} }

BOOST_AUTO_TEST_CASE(custom_operations_htlc_bitshares_eos_test)
{ try {

   ACTORS((nathan)(alice));

   custom_operations_api custom_operations_api(app);

   generate_block();

   enable_fees();
   signed_transaction trx;
   set_expiration(db, trx);

   int64_t init_balance(10000 * GRAPHENE_BLOCKCHAIN_PRECISION);

   transfer(committee_account, nathan_id, asset(init_balance));
   transfer(committee_account, alice_id, asset(init_balance));

   enable_fees();

   {
      custom_operation op;
      htlc_bitshares_eos_operation htlc;
      htlc.bitshares_account = alice_id;
      htlc.eos_account = "dan-larimer";
      asset bitshares_amount;
      htlc.bitshares_amount = asset(10);
      htlc.eos_asset = "EOS";
      htlc.eos_amount = 10;
      htlc.expiration = db.head_block_time() + 3600;

      formatted_custom_operation<htlc_bitshares_eos_operation> fco;
      fco.type = graphene::custom_operations::types::htlc;
      fco.data = htlc;

      auto packed = fc::raw::pack(fco);

      op.payer = alice_id;
      op.data = packed;
      op.fee = db.get_global_properties().parameters.current_fees->calculate_fee(op);
      trx.operations.push_back(op);
      sign(trx, alice_private_key);
      PUSH_TX(db, trx, ~0);
      trx.clear();
   }
   generate_block();

   vector<htlc_bitshares_eos_object> htlc_offers_results_alice = custom_operations_api.get_account_htlc_offers("alice");

   wdump((fc::json::to_string(htlc_offers_results_alice)));

}
catch (fc::exception &e) {
   edump((e.to_detail_string()));
   throw;
} }


BOOST_AUTO_TEST_SUITE_END()
