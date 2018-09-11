/*
 * Copyright (c) 2018 oxarbitrage, and contributors.
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

#include <graphene/app/plugin.hpp>
#include <graphene/chain/database.hpp>

namespace graphene { namespace lua {

using namespace chain;

#ifndef LUA_SPACE_ID
#define LUA_SPACE_ID 9
#endif

namespace detail
{
    class lua_plugin_impl;
}

class lua_plugin : public graphene::app::plugin
{
   public:
      lua_plugin();
      virtual ~lua_plugin();

      std::string plugin_name()const override;
      std::string plugin_description()const override;
      virtual void plugin_set_program_options(
         boost::program_options::options_description& cli,
         boost::program_options::options_description& cfg) override;
      virtual void plugin_initialize(const boost::program_options::variables_map& options) override;
      virtual void plugin_startup() override;

      friend class detail::lua_plugin_impl;
      std::unique_ptr<detail::lua_plugin_impl> my;
};

struct smart_contract_object : public abstract_object<smart_contract_object>
{
    static const uint8_t space_id = LUA_SPACE_ID;
    static const uint8_t type_id  = 0;

    //fc::time_point_sec time;
    account_id_type owner;
    //private_key_type private_key;
    private_key_type private_key;
    std::string script;
    std::string output;
    bool status;
};

struct by_id;
struct by_status;
typedef multi_index_container<
        smart_contract_object,
        indexed_by<
           ordered_non_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
           ordered_non_unique< tag<by_status>, member< smart_contract_object, bool, &smart_contract_object::status > >

        >
> smart_contract_multi_index_type;

typedef generic_index<smart_contract_object, smart_contract_multi_index_type> smart_contract_index;


} } //graphene::lua

FC_REFLECT_DERIVED( graphene::lua::smart_contract_object, (graphene::db::object),
                    (owner)(private_key)(script)(output)(status))