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

#include <graphene/custom_operations/custom_operations_plugin.hpp>

#include <fc/crypto/hex.hpp>
#include <iostream>
#include <graphene/app/database_api.hpp>


namespace graphene { namespace custom_operations {

namespace detail
{

class custom_operations_plugin_impl
{
   public:
   custom_operations_plugin_impl(custom_operations_plugin& _plugin)
         : _self( _plugin )
      {  }
      virtual ~custom_operations_plugin_impl();

      void onBlock( const signed_block& b );

      graphene::chain::database& database()
      {
         return _self.database();
      }

      custom_operations_plugin& _self;

      std::string _plugin_option = "";

   private:

};

void custom_operations_plugin_impl::onBlock( const signed_block& b )
{
   for(auto trx : b.transactions)
   {
      for(auto op : trx.operations)
      {
         if(op.is_type<custom_operation>()) {

            variant op_object;
            op.visit(fc::from_static_variant(op_object, FC_PACK_MAX_DEPTH));
            auto co = op_object.as<custom_operation>(FC_PACK_MAX_DEPTH);

            try {
               if (co.data.data()[0] == graphene::custom_operations::types::account_contact) {

                  formatted_custom_operation<account_contact_operation> custom_operation_unpacked;
                  custom_operation_unpacked = fc::raw::unpack<formatted_custom_operation<account_contact_operation>>(co.data);

                  account_contact_operation account_contact_op;
                  account_contact_op.fee_payer = co.fee_payer();
                  account_contact_op.account = custom_operation_unpacked.data.account;
                  account_contact_op.name = custom_operation_unpacked.data.name;
                  account_contact_op.email = custom_operation_unpacked.data.email;
                  account_contact_op.phone = custom_operation_unpacked.data.phone;
                  account_contact_op.address = custom_operation_unpacked.data.address;
                  account_contact_op.company = custom_operation_unpacked.data.company;
                  account_contact_op.url = custom_operation_unpacked.data.url;

                  account_contact_op.validate();
                  account_contact_evaluator evaluator(database());
                  evaluator.do_evaluate(account_contact_op);
                  evaluator.do_apply(account_contact_op);
               }
               else if (co.data.data()[0] == graphene::custom_operations::types::create_htlc) {

                  formatted_custom_operation<create_htlc_eos_operation> custom_operation_unpacked;
                  custom_operation_unpacked = fc::raw::unpack<formatted_custom_operation<create_htlc_eos_operation>>(co.data);

                  create_htlc_eos_operation htlc_bitshares_eos_op;
                  htlc_bitshares_eos_op.fee_payer = co.fee_payer();
                  htlc_bitshares_eos_op.bitshares_account = custom_operation_unpacked.data.bitshares_account;
                  htlc_bitshares_eos_op.eos_account = custom_operation_unpacked.data.eos_account;
                  htlc_bitshares_eos_op.bitshares_amount = custom_operation_unpacked.data.bitshares_amount;
                  htlc_bitshares_eos_op.eos_asset = custom_operation_unpacked.data.eos_asset;
                  htlc_bitshares_eos_op.eos_amount = custom_operation_unpacked.data.eos_amount;
                  htlc_bitshares_eos_op.expiration = custom_operation_unpacked.data.expiration;

                  htlc_bitshares_eos_op.validate();
                  create_htlc_eos_evaluator evaluator(database());
                  evaluator.do_evaluate(htlc_bitshares_eos_op);
                  evaluator.do_apply(htlc_bitshares_eos_op);
               }
               else if (co.data.data()[0] == graphene::custom_operations::types::take_htlc) {

                  formatted_custom_operation<take_htlc_eos_operation> custom_operation_unpacked;
                  custom_operation_unpacked = fc::raw::unpack<formatted_custom_operation<take_htlc_eos_operation>>(co.data);

                  take_htlc_eos_operation htlc_bitshares_eos_op;
                  htlc_bitshares_eos_op.fee_payer = co.fee_payer();
                  htlc_bitshares_eos_op.bitshares_account = custom_operation_unpacked.data.bitshares_account;
                  htlc_bitshares_eos_op.eos_account = custom_operation_unpacked.data.eos_account;
                  htlc_bitshares_eos_op.htlc_order_id = custom_operation_unpacked.data.htlc_order_id;
                  htlc_bitshares_eos_op.expiration = custom_operation_unpacked.data.expiration;

                  htlc_bitshares_eos_op.validate();
                  take_htlc_eos_evaluator evaluator(database());
                  evaluator.do_evaluate(htlc_bitshares_eos_op);
                  evaluator.do_apply(htlc_bitshares_eos_op);
               }
            }
            catch (fc::exception e) {
               //FC_THROW_EXCEPTION(plugin_exception, "failed on unpack, validate() or evaluator");
               //elog((""));
               elog((e.to_detail_string()));
               continue;
            }
         }
      }
   }
}

custom_operations_plugin_impl::~custom_operations_plugin_impl()
{
   return;
}

} // end namespace detail

custom_operations_plugin::custom_operations_plugin() :
   my( new detail::custom_operations_plugin_impl(*this) )
{
}

custom_operations_plugin::~custom_operations_plugin()
{
}

std::string custom_operations_plugin::plugin_name()const
{
   return "custom_operations";
}
std::string custom_operations_plugin::plugin_description()const
{
   return "custom_operations description";
}

void custom_operations_plugin::plugin_set_program_options(
   boost::program_options::options_description& cli,
   boost::program_options::options_description& cfg
   )
{
   cli.add_options()
         ("custom_operations_option", boost::program_options::value<std::string>(), "custom_operations option")
         ;
   cfg.add(cli);
}

void custom_operations_plugin::plugin_initialize(const boost::program_options::variables_map& options)
{
   database().add_index< primary_index< account_contact_index  > >();
   database().add_index< primary_index< htlc_orderbook_index  > >();

   database().applied_block.connect( [&]( const signed_block& b) {
      my->onBlock(b);
   } );

   if (options.count("custom_operations")) {
      my->_plugin_option = options["custom_operations"].as<std::string>();
   }
}

void custom_operations_plugin::plugin_startup()
{
   ilog("custom_operations: plugin_startup() begin");
}

} }
