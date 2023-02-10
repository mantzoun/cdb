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
            this->logger->debug("Discord Bot: " + event.message);
            break;
        case dpp::ll_info:
            this->logger->info("Discord Bot: " +  event.message);
            break;
        case dpp::ll_warning:
            this->logger->warn("Discord Bot: " + event.message);
            break;
        case dpp::ll_error:
        case dpp::ll_critical:
        default:
            this->logger->error("Discord Bot: " + event.message);
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
        cdb::callback_msg msg = {CDB_MSG_MAX, "N/A"};

        this->logger->debug("parse message  " + event.custom_id);

        strncpy(dev_id, event.custom_id.c_str(), sizeof(dev_id));
        tmp = strtok(dev_id, "#");

        while (tmp != NULL)
        {
            switch(index++){
                case 0:
                    msg.content = tmp;
                    this->logger->debug("message is " + msg.content);
                    break;
                case 1:
                    if (strcmp(tmp, "ON") == 0) {
                        msg.type = CDB_MSG_MQTT_HANDLER_TURN_DEVICE_ON;
                    } else if (strcmp(tmp, "OFF") == 0) {
                        msg.type = CDB_MSG_MQTT_HANDLER_TURN_DEVICE_OFF;
                    }

                    this->logger->debug("type is " + std::to_string(msg.type));
                    break;
                default:
                    this->logger->error("Unexpected number of arguments : " + event.custom_id);
                    break;
            }

            tmp = strtok (NULL, "#");
        }

        this->m_handler->message_cb(&msg);
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

void cdb::DiscordBot::set_mqtt_handler(cdb::CallbackClass * m_handler)
{
    this->m_handler = m_handler;
}

void cdb::DiscordBot::set_logger(cdb::Logger * l)
{
    this->logger = l;
}

std::string cdb::DiscordBot::bot_id(void)
{
    return this->_bot_id;
}
