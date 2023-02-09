/*
 * cdb_discord_bot.h
 *
 * handle connection with the Discord servers
 * uses the DPP library
 * https://dpp.dev/
 */

#ifndef CDB_DISCORD_BOT__H
#define CDB_DISCORD_BOT__H

#include "cdb_logger.h"
#include "cdb_mqtt_handler.h"

namespace cdb {
    /**
     * @class DiscordBot
     *
     * @brief The bot will run on the DPP library to connect to
     *        discord, and offer a callback function for message
     *        posting
     */
    class DiscordBot : public cdb::CallbackClass
    {
        private:
            cdb::Logger * logger = NULL;
            cdb::CallbackClass * m_handler = NULL;
            std::string _bot_id;

        public:
            DiscordBot(void);

            /**
             * @brief Initiaze the bot
             *
             * @param token Thi discord authentication token
             */
            void init(std::string token, std::string bot_id);

            /**
             * @brief set the logger object
             *
             * @param logger The logger object
             */
            void set_logger(cdb::Logger * logger);

            /**
             * @brief set the MQTT handler
             *
             * @param m_handler the MQTT handler object
             */
            void set_mqtt_handler(cdb::CallbackClass * m_handler);

            /**
             * @brief Post a message to discord
             *
             * @param msg The message information
             */
            void message_cb(cdb::callback_msg *msg);

            /**
             * @brief Getter function for the bot id
             *
             * @return The bot id
             */
            std::string bot_id(void);
    };
}

#endif /* CDB_DISCORD_BOT__H */
