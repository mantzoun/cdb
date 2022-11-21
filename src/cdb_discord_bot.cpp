/*
 * cdb_discord_bot.c
 *
 * implementation of CDB_DiscordBot methods
 */

#include "dpp/dpp.h"

#include "cdb_discord_bot.h"
#include "cdb_discord_conf.h"

CDB_DiscordBot::CDB_DiscordBot(void)
{
}

void CDB_DiscordBot::init(void)
{
    dpp::cluster bot(BOT_TOKEN);

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


