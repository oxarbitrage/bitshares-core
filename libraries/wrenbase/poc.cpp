#include <wrenbase/Int.hpp>
#include <fc/time.hpp>
#include <fc/log/logger.hpp>
#include <graphene/chain/database.hpp>

class Wren {
   public:
      static void        wrenBind();
      static std::string wrenScript();
};

int main( int argc, char** argv ) {


       Wren::wrenBind();


   wrenpp::VM vm;


   auto script = wrenScript();

   vm.executeString( script )


}


void wren_plugin_impl::wrenBind() {
   wrenpp::beginModule( "main" )
           .bindClass<wren_plugin_impl,wren_plugin&>( "Data" )
           .bindFunction< decltype( &wren_plugin_impl::getBalance), &wren_plugin_impl::getBalance >( true, "getBalance(_,_)")
           .endClass()
           .endModule();
}

std::string wren_plugin_impl::wrenScript() {
   return R"(
      foreign class Data {
         foreign static getBalance(account, asset)
      }
      System.print( Data )
      System.print(Data.getBalance("90742","0"))

      )";  // Wren UInt256
}
