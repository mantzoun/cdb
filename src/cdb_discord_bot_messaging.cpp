/*
 * cdb_discord_bot_messaging.c
 *
 * implementation of CDB_DiscordBot methods
 */

#include "dpp/restresults.h"
#include "dpp/guild.h"

#include <stdio.h>
#include <map>

#include "cdb_discord_bot.h"

dpp::command_completion_event_t generic_callback(dpp::confirmation_callback_t result);

void cdb::DiscordBot::post_message(std::string content, dpp::snowflake guild_id, std::string channel,
                         dpp::command_completion_event_t callback(dpp::confirmation_callback_t result))
{
    dpp::message m;

    channel_map_t c_map = this->channel_map[guild_id];

    m.channel_id = c_map[channel];
    m.content    = content;

    message_create(m, *callback);
}

void cdb::DiscordBot::message_cb(cdb::intra_msg_t * msg)
{
    dpp::message m, s;
    dpp::component c;
    dpp::component ar;

    cdb::discord_bot_msg_t * bot_msg = msg->content.bot;

    log(dpp::ll_debug, "received message " + std::to_string(bot_msg->type));

    for (auto& it: this->message_map) {
        dpp::snowflake g_id = it.first;
        message_map_t device_map = it.second;
        channel_map_t channel_map = this->channel_map[g_id];

        std::string device = bot_msg->device;

        switch(bot_msg->type){
            case CDB_INTRA_DISC_BOT_DEVICE_ADD:
                if (device_map.find(device) == device_map.end()){
                    log(dpp::ll_debug, "message not found for " + device);
                    m.channel_id = channel_map["devices"];
                    m.content    = device;

                    ar.set_type(dpp::cot_action_row);

                    c.set_style(dpp::cos_success);
                    c.set_label("ON");
                    c.set_id(device + "#ON");
                    c.set_type(dpp::cot_button);

                    ar.add_component(c);

                    c.set_style(dpp::cos_danger);
                    c.set_label("OFF");
                    c.set_id(device + "#OFF");
                    c.set_type(dpp::cot_button);

                    ar.add_component(c);

                    m.add_component(ar);

                    message_create(m, &generic_callback);
                } else {
                    log(dpp::ll_debug, "message found for " + device + ". Skipping");
                }
                break;
            case CDB_INTRA_DISC_BOT_DEVICE_SET:
                if (device_map.find(device) == device_map.end()){
                    log(dpp::ll_warning, "status message not found for " + device);
                    return;
                }

                m = message_get_sync(device_map[device], channel_map["devices"]);
                if (bot_msg->content == "ON"){
                    m.content = device + " is ON";
                } else if (bot_msg->content == "OFF"){
                    m.content = device + " is OFF";
                }

                message_edit(m, &generic_callback);
                post_message(m.content, m.guild_id, "syslog", &generic_callback);
                break;
            case CDB_INTRA_DISC_BOT_POST_FILE:
                m.channel_id = channel_map[bot_msg->channel];
                m.add_file(bot_msg->content.substr(bot_msg->content.find_last_of("\\/"), bot_msg->content.size()),
                           dpp::utility::read_file(bot_msg->content));

                message_create(m, &generic_callback);
                break;
            case CDB_INTRA_DISC_BOT_POST_TEXT:
                post_message(bot_msg->content, g_id, bot_msg->channel, &generic_callback);
                break;
            default:
                break;
        }
    }
}
