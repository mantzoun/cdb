/*
 * cdb_discord_bot_callbacks.c
 *
 * implementation of CDB_DiscordBot methods
 */

#include "dpp/restresults.h"
#include "dpp/guild.h"

#include "cdb_discord_bot.h"

dpp::command_completion_event_t generic_callback(dpp::confirmation_callback_t result)
{
    cdb::DiscordBot * my_bot = (cdb::DiscordBot *) result.bot;

    my_bot->log(dpp::ll_debug, "Generic Callback");

    if ( result.is_error() == true ){
        dpp::error_info err = result.get_error();
        my_bot->log(dpp::ll_error, "Error " + err.message);
        return NULL;
    }

    if (std::holds_alternative<dpp::guild_map>(result.value)) {
        // On init, get the guilds the bot is a member of
        my_bot->guild_map_cb(std::get<dpp::guild_map>(result.value));
    } else if (std::holds_alternative<dpp::channel_map>(result.value)) {
        // On init, get the channels of a guild
        my_bot->channel_map_cb(std::get<dpp::channel_map>(result.value));
    } else if (std::holds_alternative<dpp::message_map>(result.value)) {
        // On init, get the messages in a channel of interest (i.e. devices)
        my_bot->message_map_cb(std::get<dpp::message_map>(result.value));
    } else if (std::holds_alternative<dpp::message>(result.value)) {
        // Callback after a message post/edit
        my_bot->dpp_message_cb(std::get<dpp::message>(result.value));
    }

    return NULL;
}

void cdb::DiscordBot::message_map_cb(dpp::message_map message_map)
{
    log(dpp::ll_debug, "devices init Callback");

    dpp::snowflake g_id = message_map.begin()->second.guild_id;
    cdb::channel_map_t chan_map = this->channel_map[g_id];
    cdb::message_map_t temp_map;

    for (auto& it: message_map) {
        std::string device_name;
        dpp::message m = it.second;

        if (m.channel_id == chan_map["devices"]){
            size_t fs = m.content.find(" ");

            if (fs == std::string::npos){
                device_name = m.content;
            } else {
                device_name = m.content.substr(0, fs);
            }

            log(dpp::ll_debug, "found message for " + device_name);
            temp_map[device_name] = m.id;
        }
    }

    this->message_map[g_id] = temp_map;
}

void cdb::DiscordBot::dpp_message_cb(dpp::message message)
{
    dpp::snowflake g_id = message.guild_id;

    channel_map_t c_map = this->channel_map[g_id];
    message_map_t m_map = this->message_map[g_id];

    log(dpp::ll_debug, "message Callback, gid " + std::to_string(g_id));

    if (message.channel_id == c_map["devices"]) {
        log(dpp::ll_debug, "Got respose for " + message.content);
        m_map[message.content] = message.id;
    }
}

void cdb::DiscordBot::channel_map_cb(dpp::channel_map channels_map)
{
    dpp::snowflake id;
    dpp::channel c;

    cdb::channel_map_t temp_map;

    log(dpp::ll_debug, "channel Callback");

    for (auto& it: channels_map) {
        id = it.first;
        c = it.second;

        log(dpp::ll_debug, c.name);
        temp_map[c.name] = id;

        if (c.name == "devices") {
            messages_get(id, 0, 0, 0, 0, &generic_callback);
        }
    }

    dpp::snowflake g_id = channels_map.begin()->second.guild_id;
    log(dpp::ll_debug, "channel Callback, gid " + std::to_string(g_id));

    this->channel_map[g_id] = temp_map;

    post_message("Bot conneced, bot id " + this->bot_id(), g_id, "syslog", &generic_callback);
}

void cdb::DiscordBot::guild_map_cb(dpp::guild_map guild_map)
{
    log(dpp::ll_debug, "Guild map Callback");

    dpp::snowflake id;
    dpp::guild g;

    for (auto& it: guild_map) {
        // Do stuff
        id = it.first;
        g = it.second;

        channels_get(id, &generic_callback);
    }
}
