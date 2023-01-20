#ifndef CDB_CONFIGURATOR__H
#define CDB_CONFIGURATOR__H

#include <string>

#include "cdb_definitions.h"
#include "cdb_inventory.h"
#include "cdb_logger.h"

namespace cdb {
    /**
     * @class Configurator
     *
     * @brief Configuration handler
     *
     *        The CDB_Configurator Class will parse the config file,
     *        instantiate the necessary objects and store data to
     *        a CDB_Inventory instance
     */
    class Configurator {
        private:
            cdb::Logger * logger;
            std::string _discord_token;

            int  conf_line_parser(std::string * parameter_array, std::string line, int num_of_arguments);
            void parse_discord_token(std::string line);
            void parse_mqtt_device(std::string line);
            void parse_mqtt_server(std::string line);
            void parse_line(std::string line);

        public:
            /**
             * @brief An instance of the CDB_Inventory class, thas holds
             *        all configured objects
             */
            cdb::Inventory inventory;

            /**
             * @brief Default construtor
             */
            Configurator(void);

            /**
             * @brief Set the pointer to a logger object
             *
             * @param logger The logger object
             */
            void set_logger(cdb::Logger * logger);

            /**
             * @brief Read the configuration from the provided file
             *
             * @param file the configuration file
             */
            void config_read(std::string file);

            /**
             * @brief Getter function for the discord bot API token
             *
             * @return THe discord token
             *
             */
            std::string discord_token(void);

            /**
             * @brief Setter function for the discord bot API token
             *
             * @param token The Discord token
             */
            void set_discord_token(std::string token);
    };
}

#endif /* CDB_CONFIGURATOR__H */
