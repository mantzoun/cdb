#include <fstream>
#include <iostream>
#include <sstream>

#include "cdb_config.h"
#include "cdb_mqtt_device.h"

int CDB_Configurator::conf_line_parser(std::string parameter_array[], std::string line, int num_of_arguments)
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
        this->logger->warn("invalid config: " + line);
        return CDB_CONF_NOK;
    }

    return CDB_CONF_OK;
}

void CDB_Configurator::parse_mqtt_server(std::string line)
{
    std::string items[CDB_MQTT_SERVER_PARAMS];

    if (CDB_CONF_OK == this->conf_line_parser(items, line, sizeof(items)/sizeof(items[0]))) {
        CDB_MQTT_Server * s = new CDB_MQTT_Server(items[CDB_MQTT_SERVER_ADDR],
                                            std::stoi(items[CDB_MQTT_SERVER_PORT]),
                                            items[CDB_MQTT_SERVER_USERNAME],
                                            items[CDB_MQTT_SERVER_PASSWORD],
                                            items[CDB_MQTT_SERVER_CLIENT_ID]);
        this->logger->debug("Parsed MQTT server configuration for " + s->addr() + ":" + std::to_string(s->port()));
        this->inventory.mqtt_server_add(s);
    }
}

void CDB_Configurator::parse_mqtt_device(std::string line)
{
    std::string items[CDB_MQTT_DEVICE_PARAMS];

    if (CDB_CONF_OK == this->conf_line_parser(items, line, sizeof(items)/sizeof(items[0]))) {
        CDB_MQTT_Device * d = new CDB_MQTT_Device(items[CDB_MQTT_DEVICE_NAME],
                                            items[CDB_MQTT_DEVICE_CMND],
                                            items[CDB_MQTT_DEVICE_STAT]);
        this->logger->debug("Parsed MQTT device configuration for " + d->name());
        this->inventory.mqtt_device_add(d);
    }
}

void CDB_Configurator::parse_discord_token(std::string line)
{
    std::string items[CDB_DISCORD_PARAMS];

    if (CDB_CONF_OK == this->conf_line_parser(items, line, sizeof(items)/sizeof(items[0]))) {
        this->set_discord_token(items[CDB_DISCORD_TOKEN]);
        this->logger->debug("Parsed discord token configuration");
    }
}

void CDB_Configurator::parse_line(std::string line)
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
    } else if (type == "discord_token") {
        this->parse_discord_token(line);
    } else {
        this->logger->warn("invalid config: " + line);
    }
}

void CDB_Configurator::config_read(std::string file)
{
    std::string line;

    std::ifstream conf_file(file);

    while (std::getline(conf_file, line)){
        this->parse_line(line);
    }
}

std::list<CDB_MQTT_Device *> * CDB_Configurator::mqtt_dev_list(void)
{
    return this->inventory.mqtt_devices;
}

CDB_Configurator::CDB_Configurator(void)
{
}

std::string CDB_Configurator::discord_token(void)
{
    return this->_discord_token;
}

void CDB_Configurator::set_discord_token(std::string token)
{
    this->_discord_token = token;
}

void CDB_Configurator::set_logger(CDB_Logger * logger)
{
    this->logger = logger;
}
