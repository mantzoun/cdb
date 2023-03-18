/*
 * cdb_discord_bot_main.c
 *
 * implementation of CDB_DiscordBot methods
 */

#include "dpp/restresults.h"
#include "dpp/guild.h"

#include <stdio.h>
#include <map>

#include "cdb_discord_bot.h"

dpp::command_completion_event_t generic_callback(dpp::confirmation_callback_t result);

void cdb::DiscordBot::init(std::string token, std::string bot_id)
{
    this->_bot_id = bot_id;

    // Use our own logger for output consistency
    on_log([this](const dpp::log_t & event) {
    switch (event.severity) {
        case dpp::ll_trace:
        case dpp::ll_debug:
            LOG_DEBUG(logger, "Discord Bot: " + event.message);
            break;
        case dpp::ll_info:
            LOG_INFO(logger, "Discord Bot: " +  event.message);
            break;
        case dpp::ll_warning:
            LOG_WARN(logger, "Discord Bot: " + event.message);
            break;
        case dpp::ll_error:
        case dpp::ll_critical:
        default:
            LOG_ERROR(logger, "Discord Bot: " + event.message);
            break;
        }
    });


    on_button_click([this](const dpp::button_click_t & event) {
        /* Button clicks are still interactions, and must be replied to in some form to
         * prevent the "this interaction has failed" message from Discord to the user.
         */
        event.reply();

        char dev_id[50];
        char * tmp;
        int index = 0;
        cdb::intra_msg_t msg;
        cdb::discord_bot_msg_t bot;
        msg.sender = CDB_DISCORD_BOT;
        msg.content.bot = &bot;

        LOG_DEBUG(logger, "parse message " + event.custom_id);

        strncpy(dev_id, event.custom_id.c_str(), sizeof(dev_id));
        tmp = strtok(dev_id, "#");

        while (tmp != NULL)
        {
            std::string tmp_s = tmp;

            switch(index++){
                case 0:
                    bot.device = tmp_s;
                    LOG_DEBUG(logger, "message is " + bot.device);
                    break;
                case 1:
                    if (tmp_s == "ON" || tmp_s == "OFF") {
                        bot.type = CDB_INTRA_DISC_BOT_COMMAND;

                        LOG_DEBUG(logger, "type is " + std::to_string(bot.type));
                    } else {
                        LOG_DEBUG(logger, "invalid message content " + tmp_s);
                    }
                    break;
                default:
                    LOG_ERROR(logger, "Unexpected number of arguments : " + event.custom_id);
                    break;
            }

            tmp = strtok (NULL, "#");
        }

        this->message_handler->message_cb(&msg);
    });

    on_ready([this](const dpp::ready_t& event) {
        //if (dpp::run_once<struct register_bot_commands>()) {
        //    bot->global_command_create(
        //        dpp::slashcommand("ping", "Ping pong!", bot->me.id)
        //    );

        this->current_user_get_guilds(&generic_callback);

        this->register_commands();
    });

    on_slashcommand([this](const dpp::slashcommand_t & event) {
        this->handle_slashcommand(event);
    });

    start(dpp::st_return);
}

void cdb::DiscordBot::set_message_handler(cdb::CallbackClass * handler)
{
    this->message_handler = handler;
}

void cdb::DiscordBot::set_logger(cdb::Logger * l)
{
    this->logger = l;
}

std::string cdb::DiscordBot::bot_id(void)
{
    return this->_bot_id;
}
