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
     * @brief Error codes used in this module
     */
    enum bot_err
    {
        CDB_BOT_OK,
        CDB_BOT_ERR,
    };

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
            std::map<std::string, dpp::snowflake> channel_map;
            std::map<std::string, dpp::snowflake> device_map;

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

            /**
             * @brief Post a messsage to a channel
             *
             * @param content A srting containing the message
             * @param channel The channel to post to
             * @param callback The callback with the action result
             */
            void post_message(std::string content, std::string channel,
                 dpp::command_completion_event_t callback(dpp::confirmation_callback_t result));

            /**
             * @brief Callback function for the guild map request
             *
             * @param guild_map The guild map
             */
            void guild_map_cb(dpp::guild_map guild_map);

            /**
             * @brief Callback function for the channel map request
             *
             * @param channel_map The channel map
             */
            void channel_map_cb(dpp::channel_map channel_map);

            /**
             * @brief Callback function for the message map request
             *
             * @param message_map The message map
             */
            void message_map_cb(dpp::message_map message_map);

            /**
             * @brief Callback function for the message request
             *
             * @param message The request message
             */
            void dpp_message_cb(dpp::message message);
    };
}

#endif /* CDB_DISCORD_BOT__H */
