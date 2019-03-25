#include "include/bitshares.hpp"

Bitshares::Bitshares()
{

}

void Bitshares::Connect(std::string server)
{
   graphene::wallet::wallet_data wallet_data;

   wallet_data.ws_server = "wss://api.bitshares-kibana.info/ws";
   wallet_data.ws_user = "";
   wallet_data.ws_password = "";
   websocket_connection  = websocket_client.connect( server );

   api_connection = std::make_shared<fc::rpc::websocket_api_connection>(*websocket_connection,
           GRAPHENE_MAX_NESTED_OBJECTS);

   remote_login_api = api_connection->get_remote_api< graphene::app::login_api >(1);
   database_api = api_connection->get_remote_api< graphene::app::database_api >(0);

   FC_ASSERT(remote_login_api->login( wallet_data.ws_user, wallet_data.ws_password ), "Failed to log in to API server");

   wallet_data.chain_id = database_api->get_chain_id();

   wallet_api_ptr = std::make_shared<graphene::wallet::wallet_api>(wallet_data, remote_login_api);

   wallet_filename = "wallet.json";
   wallet_api_ptr->set_wallet_filename(wallet_filename);

   wallet_api = fc::api<graphene::wallet::wallet_api>(wallet_api_ptr);

   wallet_api_ptr->set_wallet_filename( "wallet.json" );
   wallet_api_ptr->load_wallet_file();

   wallet_cli = std::make_shared<fc::rpc::cli>(GRAPHENE_MAX_NESTED_OBJECTS);

   for( auto& name_formatter : wallet_api_ptr->get_result_formatters() )
      wallet_cli->format_result( name_formatter.first, name_formatter.second );

   boost::signals2::scoped_connection closed_connection(websocket_connection->closed.connect([=]{
       cerr << "Server has disconnected us.\n";
       wallet_cli->stop();
   }));


// logging
/*
   //fc::path data_dir;
   fc::logging_config cfg;
   fc::path log_dir = "logs";

   fc::file_appender::config ac;
   ac.filename             = log_dir / "rpc" / "rpc.log";
   ac.flush                = true;
   ac.rotate               = true;
   ac.rotation_interval    = fc::hours( 1 );
   ac.rotation_limit       = fc::days( 1 );

   std::cout << "Logging RPC to file: " << (ac.filename).preferred_string() << "\n";

   cfg.appenders.push_back(fc::appender_config( "default", "console", fc::variant(fc::console_appender::config(), 20)));
   cfg.appenders.push_back(fc::appender_config( "rpc", "file", fc::variant(ac, 5)));

   cfg.loggers = { fc::logger_config("default"), fc::logger_config( "rpc") };
   cfg.loggers.front().level = fc::log_level::info;
   cfg.loggers.front().appenders = {"default"};
   cfg.loggers.back().level = fc::log_level::debug;
   cfg.loggers.back().appenders = {"rpc"};
*/
}

void Bitshares::Disconnect()
{
   websocket_connection->close(0, "user disconnecting");
   //websocket_connection->closed.set_value();

   websocket_connection = nullptr;
   api_connection = nullptr;
   //remote_login_api = nullptr;
   //database_api =  nullptr;
   wallet_api_ptr = nullptr;
}
