/*
 * cdb_discord_bot.h
 *
 * handle connection with the Discord servers
 * uses the DPP library
 * https://dpp.dev/
 */

#ifndef CDB_DISCORD_BOT__H
#define CDB_DISCORD_BOT__H

#include "dpp/dpp.h"
#include "cdb_callback_class.h"
#include "cdb_logger.h"

namespace cdb {
    /*
     * Map of channels
     * key      Channel name
     * value    Channel info
     */
    typedef std::map<std::string, dpp::snowflake> channel_map_t;

    /*
     * Map of messages
     * key      Message content
     * value    Message info
     */
    typedef std::map<std::string, dpp::snowflake> message_map_t;

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
    class DiscordBot : public cdb::CallbackClass, public dpp::cluster
    {
        private:
            cdb::Logger * logger = NULL;
            cdb::CallbackClass * message_handler = NULL;
            std::string _bot_id;
            /*
             * Map of channels of interest
             * key      Guild snowflake
             * value    Channels map
             */
            std::map<dpp::snowflake, channel_map_t> channel_map;

            /*
             * Map of messages of interest
             * key      Guild snowflake
             * value    Message snowflake
             */
            std::map<dpp::snowflake, message_map_t> message_map;

            /**
             * @brief Post a messsage to a channel
             *
             * @param content A srting containing the message
             * @param channel The channel to post to
             * @param callback The callback with the action result
             */
            void post_message(std::string content, dpp::snowflake guild_id, std::string channel,
                 dpp::command_completion_event_t callback(dpp::confirmation_callback_t result));

            /**
             * @brief Register discord slash commands
             */
            void register_commands(void);

            /**
             * @brief Handler for received slash command
             *
             * @param event Command information
             */
            void handle_slashcommand(const dpp::slashcommand_t & event);

        public:
            using dpp::cluster::cluster;

            /**
             * @brief Initiaze the bot
             *
             * @param token The discord authentication token
             * @param bot_id The ID of the bot instance
             */
            void init(std::string token, std::string bot_id);

            /**
             * @brief set the logger object
             *
             * @param logger The logger object
             */
            void set_logger(cdb::Logger * logger);

            /**
             * @brief Set a pointer to the module that will handle
             *        messages coming from this object
             *
             * @param message_handler The message handling object
             */
            void set_message_handler(cdb::CallbackClass * handler);

            /**
             * @brief Post a message to discord
             *
             * @param msg The message information
             */
            void message_cb(cdb::intra_msg_t *msg);

            /**
             * @brief Getter function for the bot id
             *
             * @return The bot id
             */
            std::string bot_id(void);

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
