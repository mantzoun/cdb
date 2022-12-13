/*
 * cdb_discord_bot.c
 *
 * implementation of CDB_DiscordBot methods
 */

#include "dpp/dpp.h"
#include "dpp/restresults.h"
#include "dpp/guild.h"

#include <stdio.h>
#include <map>

#include "cdb_discord_bot.h"

static dpp::cluster * bot;

static dpp::snowflake syslog_sf;
static dpp::snowflake devices_sf;

std::map<std::string, dpp::snowflake> device_map;

CDB_DiscordBot::CDB_DiscordBot(void)
{
}

dpp::command_completion_event_t existing_devices(dpp::confirmation_callback_t value)
{
    std::cout << "devices init Callback\n";
    if ( value.is_error() == true ){
        std::cout << "Error\n";
        dpp::error_info err = value.get_error();
        std::cout << err.message;
    }

    dpp::message_map map = std::get<dpp::message_map>(value.value);

    for (auto& it: map) {
        dpp::message m = it.second;
        if (m.channel_id == devices_sf){
            std::cout << "found message for " + m.content + "\n";
            device_map[m.content] = m.message_reference.message_id;
        }
    }

    return NULL;
}

dpp::command_completion_event_t my_message_cb(dpp::confirmation_callback_t value)
{
    std::cout << "message Callback\n";
    if ( value.is_error() == true ){
        std::cout << "Error\n";
        dpp::error_info err = value.get_error();
        std::cout << err.message;
    }

    dpp::message m = std::get<dpp::message>(value.value);

    if (m.channel_id == devices_sf) {
        std::cout << "Got respose for " + m.content + "\n";
        device_map[m.content] = m.message_reference.message_id;
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
            syslog_sf = id;
        } else if (c.name == "devices") {
            devices_sf = id;
            bot->messages_get(devices_sf, 0, 0, 0, 0, &existing_devices);
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

void CDB_DiscordBot::message_cb(cdb_disc_msg * msg)
{
    std::cout << "1";
    dpp::message m;

    switch(msg->type){
        case CDB_DISC_MSG_MQQT_DEV_ADD:
            if (device_map.find(msg->msg) == device_map.end()){
                bot->log(dpp::ll_debug, "message not found for " + msg->msg);
                m.channel_id = devices_sf;
                m.content    = msg->msg;

                bot->message_create(m, &my_message_cb);
            } else {
                bot->log(dpp::ll_debug, "message found for " + msg->msg + ". Skipping");
            }
            break;
        default:
            break;
    }
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

    bot->start(dpp::st_return);
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


