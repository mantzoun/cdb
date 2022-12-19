#ifndef CDB_CONFIGURATOR__H
#define CDB_CONFIGURATOR__H

#include <string>

#include "cdb_definitions.h"
#include "cdb_inventory.h"
#include "cdb_logger.h"

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

        std::string discord_token(void);
        void        set_discord_token(std::string token);
};

void cdb_config_read(std::string file, CDB_Inventory * inventory);

#endif /* CDB_CONFIGURATOR__H */
