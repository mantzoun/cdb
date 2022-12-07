/*
 * cdb_discord_bot.c
 *
 * implementation of CDB_DiscordBot methods
 */

#include "dpp/dpp.h"
#include "dpp/restresults.h"
#include "dpp/guild.h"

#include <stdio.h>

#include "cdb_discord_bot.h"

dpp::cluster * bot;

CDB_DiscordBot::CDB_DiscordBot(void)
{
}

dpp::command_completion_event_t message_cb(dpp::confirmation_callback_t value)
{

    std::cout << "message Callback\n";
    if ( value.is_error() == true ){
        std::cout << "Error\n";
        dpp::error_info err = value.get_error();
        std::cout << err.message;
    }

    return NULL;
}

dpp::command_completion_event_t  channels_cb(dpp::confirmation_callback_t value)
{
    std::cout << "channel Callback\n";
    if ( value.is_error() == true ){
        std::cout << "Error\n";
        dpp::error_info err = value.get_error();
        std::cout << err.message;
    }

    dpp::channel_map channelmap = std::get<dpp::channel_map>(value.value);

    dpp::snowflake id;
    dpp::channel c;

    for (auto& it: channelmap) {
        id = it.first;
        c = it.second;

        std::cout << c.name + "\n";

        if (c.name == "syslog") {
            dpp::message m(id, "TEST");
            bot->message_create(m, &message_cb);
        }
    }

    return NULL;
}

dpp::command_completion_event_t  callback(dpp::confirmation_callback_t value)
{
    std::cout << "Callback\n";
    if ( value.is_error() == true ){
        std::cout << "Error\n";
        dpp::error_info err = value.get_error();
        std::cout << err.message;
    }

    dpp::guild_map guildmap = std::get<dpp::guild_map>(value.value);

    dpp::snowflake id;
    dpp::guild g;

    for (auto& it: guildmap) {
        // Do stuff
        id = it.first;
        g = it.second;

        bot->channels_get(id, &channels_cb);
    }
    return NULL;
}

void CDB_DiscordBot::init(std::string token)
{
    bot = new dpp::cluster(token);

    bot->on_log(dpp::utility::cout_logger());

    bot->on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
    });

    bot->on_ready([](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot->global_command_create(
                dpp::slashcommand("ping", "Ping pong!", bot->me.id)
            );
        }

        std::cout << "API call\n";
        bot->current_user_get_guilds(&callback);
    });

    bot->start(dpp::st_wait);
}

/*
 * set_logger
 *
 * Set the logger object used for logging
 */
void CDB_DiscordBot::set_logger(CDB_Logger * l)
{
    this->logger = l;
}


