#ifndef CDB_CONFIG__H
#define CDB_CONFIG__H

#include <string>
#include <list>

#include "cdb_mqtt_device.h"
#include "cdb_mqtt_server.h"
#include "cdb_mqtt_handler.h"
#include "cdb_logger.h"

#define CDB_CONF_OK         0
#define CDB_CONF_NOK        1

#define CDB_DISCORD_TYPE    1
#define CDB_DISCORD_TOKEN   1
#define CDB_DISCORD_PARAMS  2

class CDB_Inventory {
    public:
        std::list<CDB_MQTT_Device *> * mqtt_devices = NULL ;
        CDB_MQTT_Server * mqtt_server = NULL;

        void mqtt_server_add(CDB_MQTT_Server * s) {
            this->mqtt_server = s;
        }

        void mqtt_device_add(CDB_MQTT_Device * d) {
            if (this->mqtt_devices == NULL){
                this->mqtt_devices = new std::list<CDB_MQTT_Device *>();
            }

            this->mqtt_devices->push_back(d);
        }
};

class CDB_Configurator {
    private:
        CDB_Logger * logger;
        std::string _discord_token;

        int  conf_line_parser(std::string * parameter_array, std::string line, int num_of_arguments);
        void parse_discord_token(std::string line);
        void parse_mqtt_device(std::string line);
        void parse_mqtt_server(std::string line);
        void parse_line(std::string line);

    public:
        CDB_Inventory inventory;

        CDB_Configurator(void);

        void set_logger(CDB_Logger * logger);
        void config_read(std::string file);
        std::list<CDB_MQTT_Device *> * mqtt_dev_list(void);

        std::string discord_token(void);
        void        set_discord_token(std::string token);
};

void cdb_config_read(std::string file, CDB_Inventory * inventory);

#endif /* CDB_CONFIG__H */
