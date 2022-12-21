#ifndef CDB_CONFIGURATOR__H
#define CDB_CONFIGURATOR__H

#include <string>

#include "cdb_definitions.h"
#include "cdb_inventory.h"
#include "cdb_logger.h"

/**
 * The CDB_Configurator Class will parse the config file,
 * instantiate the necessary objects and store data to
 * a CDB_Inventory instance
 */
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
        /**
         * An instance of the CDB_Inventory class, thas holds
         * all configured objects
         */
        CDB_Inventory inventory;

        /**
         * Default construtor
         */
        CDB_Configurator(void);

        /**
         * Set the pointer to a logger object
         */
        void set_logger(CDB_Logger * logger);

        /**
         * Read the configuration from the provided file
         *
         * @param file the configuration file
         */
        void config_read(std::string file);

        /**
         * Getter function for the discord bot API token
         */
        std::string discord_token(void);

        /**
         * Setter function for the discord bot API token
         */
        void        set_discord_token(std::string token);
};

#endif /* CDB_CONFIGURATOR__H */
