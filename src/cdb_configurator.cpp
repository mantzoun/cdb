#include <fstream>
#include <iostream>
#include <sstream>

#include "cdb_configurator.h"
#include "cdb_mqtt_device.h"

int cdb::Configurator::conf_line_parser(std::string parameter_array[], std::string line, int num_of_arguments)
{
    std::stringstream conf(line);

    int i = 0;

    for (i=0; i<num_of_arguments; i++){
        if (!getline(conf, parameter_array[i], ' ')){
            break;
        }
        if (parameter_array[i] == "") {
            i--;
        }
    }

    if (i!= num_of_arguments) {
        LOG_WARN(this->logger,"invalid config: " + line);
        return CDB_NOK;
    }

    return CDB_OK;
}

void cdb::Configurator::parse_my_url(std::string line)
{
    std::string items[2];

    if (CDB_OK == this->conf_line_parser(items, line, 2)) {
        this->my_url = items[1];
    }
}

void cdb::Configurator::parse_log_level(std::string line)
{
    std::string items[2];

    if (CDB_OK == this->conf_line_parser(items, line, 2)) {
        if (items[1] == "DEBUG") {
            this->log_level = CDB_LOG_DEBUG;
        } else if (items[1] == "INFO") {
            this->log_level = CDB_LOG_INFO;
        } else if (items[1] == "WARN") {
            this->log_level = CDB_LOG_WARN;
        } else if (items[1] == "ERROR") {
            this->log_level = CDB_LOG_ERROR;
        } else {
            return;
        }
        LOG_DEBUG(this->logger,"Parsed log level as " + items[1]);
    }
}

void cdb::Configurator::parse_mqtt_server(std::string line)
{
    std::string items[CDB_CONF_MQTT_SERVER_PARAMS];

    if (CDB_OK == this->conf_line_parser(items, line, CDB_CONF_MQTT_SERVER_PARAMS)) {
        cdb::MqttServer * s = new cdb::MqttServer(items[CDB_CONF_MQTT_SERVER_ADDR],
                                            std::stoi(items[CDB_CONF_MQTT_SERVER_PORT]),
                                            items[CDB_CONF_MQTT_SERVER_USERNAME],
                                            items[CDB_CONF_MQTT_SERVER_PASSWORD],
                                            items[CDB_CONF_MQTT_SERVER_CLIENT_ID]);
        LOG_DEBUG(this->logger,"Parsed MQTT server configuration for " + s->addr() + ":" + std::to_string(s->port()));
        this->inventory.mqtt_server_add(s);
    }
}

void cdb::Configurator::parse_mqtt_device(std::string line)
{
    std::string items[CDB_CONF_MQTT_DEVICE_PARAMS];

    if (CDB_OK == this->conf_line_parser(items, line, CDB_CONF_MQTT_DEVICE_PARAMS)) {
        cdb::MqttDevice * d = new cdb::MqttDevice(items[CDB_CONF_MQTT_DEVICE_NAME],
                                            items[CDB_CONF_MQTT_DEVICE_CMND],
                                            items[CDB_CONF_MQTT_DEVICE_STAT]);
        LOG_DEBUG(this->logger,"Parsed MQTT device configuration for " + d->name());
        this->inventory.mqtt_device_add(d);
    }
}

void cdb::Configurator::parse_discord_config(std::string line)
{
    std::string items[CDB_CONF_DISCORD_PARAMS];

    if (CDB_OK == this->conf_line_parser(items, line, CDB_CONF_DISCORD_PARAMS)) {
        this->set_discord_token(items[CDB_CONF_DISCORD_TOKEN]);
        this->set_discord_bot_id(items[CDB_CONF_DISCORD_BOT_ID]);
        LOG_DEBUG(this->logger,"Parsed discord token configuration");
    }
}

void cdb::Configurator::parse_line(std::string line)
{
    std::string type;
    std::stringstream conf(line);

    char c = line[0];

    if (c == '#' || c == 0) {
        return;
    }

    getline(conf, type, ' ');

    if (type == "mqtt") {
        this->parse_mqtt_device(line);
    } else if (type == "mqtt_server") {
        this->parse_mqtt_server(line);
    } else if (type == "discord") {
        this->parse_discord_config(line);
    } else if (type == "log-level") {
        this->parse_log_level(line);
    } else if (type == "my_url") {
        this->parse_my_url(line);
    } else {
        LOG_WARN(this->logger,"invalid config: " + line);
    }
}

void cdb::Configurator::config_read(std::string file)
{
    std::string line;

    std::ifstream conf_file(file);

    while (std::getline(conf_file, line)){
        this->parse_line(line);
    }
}

cdb::Configurator::Configurator(void)
{
}

std::string cdb::Configurator::discord_token(void)
{
    return this->_discord_token;
}

std::string cdb::Configurator::discord_bot_id(void)
{
    return this->_discord_bot_id;
}

cdb::log_lvl cdb::Configurator::get_log_level(void)
{
    return this->log_level;
}

std::string cdb::Configurator::get_my_url(void)
{
    return this->my_url;
}

void cdb::Configurator::set_discord_token(std::string token)
{
    this->_discord_token = token;
}

void cdb::Configurator::set_discord_bot_id(std::string bot_id)
{
    this->_discord_bot_id = bot_id;
}

void cdb::Configurator::set_logger(cdb::Logger * logger)
{
    this->logger = logger;
}
