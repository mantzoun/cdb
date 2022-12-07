/*
 * main.cpp
 *
 * initialize logger object and i/o thread.
 */

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <mosquitto.h>

#include "cdb_config.h"
#include "cdb_io.h"
#include "cdb_logger.h"
#include "cdb_discord_bot.h"

void on_connect(mosquitto * m, void *obj, int res)
{
    printf("mqtt on connect res %d\n", res);
}

void on_disconnect(mosquitto * m, void *obj, int res)
{
    printf("mqtt on disconnect res %d\n", res);
}

void on_message(mosquitto * m, void *obj, const mosquitto_message *message)
{
    printf("mqtt message %s\n", (char*) message->payload);
}

int main(int argc, char** argv)
{

    if (argc < 2){
        std::cout << "Usage: cdb <config_file>\n";
        exit(1);
    }

    std::string config_file = argv[1];

    CDB_IO io;
    CDB_DiscordBot bot;
    CDB_Logger logger = CDB_Logger(CDB_LOG_DEBUG);
    CDB_Configurator conf;
    CDB_MQTT_Handler h;

    h.set_logger(&logger);
    io.set_logger(&logger);
    conf.set_logger(&logger);

    conf.config_read(config_file);

    if (conf.inventory.mqtt_server != NULL) {
        conf.inventory.mqtt_server->set_cb_on_connect(&on_connect);
        conf.inventory.mqtt_server->set_cb_on_disconnect(&on_disconnect);
        conf.inventory.mqtt_server->set_cb_on_message(&on_message);

        h.init(conf.inventory.mqtt_server);

        int res = h.connect(conf.inventory.mqtt_server);

        if (CDB_MQTT_OK != res){
            logger.error("Connection to broker failed, error " + std::to_string(res));
        } else {
            logger.info("Connected to broker.");
        }

        std::list<CDB_MQTT_Device *> * l = conf.mqtt_dev_list();
        if (l != NULL) {
            std::list<CDB_MQTT_Device *>::iterator it;
            for (it = l->begin(); it != l->end(); ++it){
                h.add_device(*it);
            }
        }

    }

    logger.info("start process\n");

    io.fifo_init("/tmp/mqtt_disc_cpp.fifo", NULL);

    bot.init(conf.discord_token());

    logger.info("bot init done\n");

    while(1);
    logger.info("exit 0\n");
}
