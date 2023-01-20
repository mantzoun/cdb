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


static cdb::DiscordBot bot;

int main(int argc, char** argv)
{
    if (argc < 2){
        std::cout << "Usage: cdb <config_file>\n";
        exit(1);
    }

    std::string config_file = argv[1];

    cdb::Logger logger = cdb::Logger(cdb::CDB_LOG_DEBUG);

    cdb::IO io;
    cdb::Configurator conf;
    cdb::MqttHandler m_handler;

    m_handler.set_logger(&logger);
    io.set_logger(&logger);
    conf.set_logger(&logger);

    logger.info("Reading config file");
    conf.config_read(config_file);

    io.fifo_init("/tmp/mqtt_disc_cpp.fifo", NULL);

    logger.info("Starting Discord Bot\n");
    bot.set_logger(&logger);
    bot.init(conf.discord_token());
    bot.set_mqtt_handler(&m_handler);

    m_handler.set_discord_bot(&bot);
    m_handler.init(&conf);

    logger.info("Init done, entering loop");

    while(1){
        usleep(1000000);
    }
}
