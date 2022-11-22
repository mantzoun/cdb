/*
 * cdb_discord_bot.c
 *
 * implementation of CDB_DiscordBot methods
 */

#include "dpp/dpp.h"
#include <dpp/restresults.h>

#include <stdio.h>

#include "cdb_discord_bot.h"

CDB_DiscordBot::CDB_DiscordBot(void)
{
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

    for (auto& it: guildmap) {
        // Do stuff
        std::cout << it.second.name;
    }
    return NULL;
}

void CDB_DiscordBot::init(std::string token)
{
    dpp::cluster bot(token);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(
                dpp::slashcommand("ping", "Ping pong!", bot.me.id)
            );
        }

        std::cout << "API call\n";
        bot.current_user_get_guilds(&callback);
    });

    bot.start(dpp::st_wait);
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


