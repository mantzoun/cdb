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

void cdb::DiscordBot::message_cb(cdb::callback_msg * msg)
{
    dpp::message m, s;
    dpp::component c;
    dpp::component ar;

    std::string content = msg->content;
    std::string channel = msg->channel;

    log(dpp::ll_debug, "received message " + std::to_string(msg->type) + " : " + content + " : " + channel);
    for (auto& it: this->message_map) {
        dpp::snowflake g_id = it.first;
        message_map_t device_map = it.second;
        channel_map_t channel_map = this->channel_map[g_id];

        switch(msg->type){
            case CDB_MSG_DISC_MQTT_DEV_ADD:
                if (device_map.find(content) == device_map.end()){
                    log(dpp::ll_debug, "message not found for " + content);
                    m.channel_id = channel_map["devices"];
                    m.content    = content;

                    ar.set_type(dpp::cot_action_row);

                    c.set_style(dpp::cos_success);
                    c.set_label("ON");
                    c.set_id(content + "#ON");
                    c.set_type(dpp::cot_button);

                    ar.add_component(c);

                    c.set_style(dpp::cos_danger);
                    c.set_label("OFF");
                    c.set_id(content + "#OFF");
                    c.set_type(dpp::cot_button);

                    ar.add_component(c);

                    m.add_component(ar);

                    message_create(m, &generic_callback);
                } else {
                    log(dpp::ll_debug, "message found for " + content + ". Skipping");
                }
                break;
            case CDB_MSG_DISC_MQTT_DEV_STATUS_ON:
            case CDB_MSG_DISC_MQTT_DEV_STATUS_OFF:
                if (device_map.find(content) == device_map.end()){
                    log(dpp::ll_warning, "status message not found for " + content);
                    return;
                }

                m = message_get_sync(device_map[content], channel_map["devices"]);
                if (msg->type == CDB_MSG_DISC_MQTT_DEV_STATUS_ON){
                    m.content    = content + " is ON";
                } else if (msg->type == CDB_MSG_DISC_MQTT_DEV_STATUS_OFF){
                    m.content    = content + " is OFF";
                }

                message_edit(m, &generic_callback);
                post_message(m.content, m.guild_id, "syslog", &generic_callback);
                break;
            case CDB_MSG_DISC_POST_FILE:
                m.channel_id = channel_map[channel];
                m.add_file(content.substr(content.find_last_of("\\/"), content.size()),
                           dpp::utility::read_file(content));

                message_create(m, &generic_callback);
                break;
            case CDB_MSG_DISC_POST_MESSAGE:
                post_message(content, g_id, channel, &generic_callback);
                break;
            default:
                break;
        }
    }
}
