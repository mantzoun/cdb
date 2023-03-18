/*
 * main.cpp
 */

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <mosquitto.h>
#include <cstring>

#include "cdb_configurator.h"
#include "cdb_io.h"
#include "cdb_logger.h"
#include "cdb_discord_bot.h"
#include "cdb_mqtt_handler.h"


class Main : public cdb::CallbackClass
{
    private:
        std::string config_file;
        cdb::Logger * logger;
        cdb::IO io;
        cdb::Configurator conf;
        cdb::MqttHandler m_handler;

        void handle_bot_message(cdb::bot_msg_t * msg)
        {
            switch(msg->type){
                case CDB_INTRA_DISC_BOT_COMMAND:
                    break;
                default:
                    break;
            }
        }

        void handle_mqtt_message(cdb::mqtt_msg_t * msg)
        {
        }

        void handle_io_message(cdb::io_msg_t * msg)
        {
            if (msg->command == "POST_FILE_DISCORD") {
            } else if (msg->command == "POST_TEXT_DISCORD") {
            } else if (msg->command == "ADD_MQTT_CONFIG") {
            } else {
                LOG_WARN(logger, "Invalid IO message");
            }
        }

        void message_cb(cdb::intra_msg_t * msg)
        {
            switch(msg->sender){
                case cdb::CDB_IO_HANDLER:
                    handle_io_message(msg->content.io);
                    break;
                case cdb::CDB_MQTT_HANDLER:
                    handle_mqtt_message(msg->content.mqtt);
                    break;
                case cdb::CDB_DISCORD_BOT:
                    handle_bot_message(msg->content.bot);
                    break;
                default:
                    break;
            }
        }

    public:
        Main(int argc, char** argv) {
            if (argc < 2){
                std::cout << "Usage: cdb <config_file>\n";
                exit(1);
            }
            config_file = argv[1];
        }

        int run(void) {
            logger = new cdb::Logger(cdb::CDB_LOG_DEBUG);

            conf.set_logger(logger);
            conf.config_read(config_file);
            logger->set_level(conf.get_log_level());

            m_handler.set_logger(logger);
            io.set_logger(logger);

            LOG_INFO(logger, "Reading config file");

            io.fifo_init("/tmp/mqtt_disc_cpp.fifo");

            LOG_INFO(logger, "Starting Discord Bot");
//            cdb::DiscordBot bot(conf.discord_token());
//            bot.set_logger(logger);
//            bot.init(conf.discord_token(), conf.discord_bot_id());
//            bot.set_message_handler(this);

            m_handler.set_message_handler(this);
            m_handler.init(&conf);

            io.set_message_handler(this);

            LOG_INFO(logger, "Init done, entering loop");

            while(1){
                usleep(1000000);
            }
        }
};

int main(int argc, char** argv)
{
    Main obj(argc, argv);

    return obj.run();
}
