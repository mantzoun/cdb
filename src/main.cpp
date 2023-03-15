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
    public:
        Main(int argc, char** argv) {
            if (argc < 2){
                std::cout << "Usage: cdb <config_file>\n";
                exit(1);
            }
            config_file = argv[1];
        }

        void message_cb(cdb::callback_msg * msg)
        {
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
            cdb::DiscordBot bot(conf.discord_token());
            bot.set_logger(logger);
            bot.init(conf.discord_token(), conf.discord_bot_id());
            bot.set_mqtt_handler(&m_handler);

            m_handler.set_discord_bot(&bot);
            m_handler.init(&conf);

            io.set_discord_bot(&bot);

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
