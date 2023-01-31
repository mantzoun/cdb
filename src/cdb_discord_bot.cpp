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

static std::map<std::string, dpp::snowflake> channel_map;
static std::map<std::string, dpp::snowflake> device_map;

cdb::DiscordBot::DiscordBot(void)
{
}

dpp::command_completion_event_t existing_devices(dpp::confirmation_callback_t value)
{
    bot->log(dpp::ll_debug, "devices init Callback");
    if ( value.is_error() == true ){
        dpp::error_info err = value.get_error();
        bot->log(dpp::ll_error, "Error " + err.message);
    }

    dpp::message_map map = std::get<dpp::message_map>(value.value);

    for (auto& it: map) {
        std::string device_name;
        dpp::message m = it.second;

        if (m.channel_id == channel_map["devices"]){
            size_t fs = m.content.find(" ");

            if (fs == std::string::npos){
                device_name = m.content;
            } else {
                device_name = m.content.substr(0, fs);
            }

            bot->log(dpp::ll_debug, "found message for " + device_name);
            device_map[device_name] = m.id;
        }
    }

    return NULL;
}

dpp::command_completion_event_t my_message_cb(dpp::confirmation_callback_t value)
{
    bot->log(dpp::ll_debug, "message Callback");
    if ( value.is_error() == true ){
        dpp::error_info err = value.get_error();
        bot->log(dpp::ll_error, "Error " + err.message);
    }

    dpp::message m = std::get<dpp::message>(value.value);

    if (m.channel_id == channel_map["devices"]) {
        bot->log(dpp::ll_debug, "Got respose for " + m.content);
        device_map[m.content] = m.id;
    }

    return NULL;
}

dpp::command_completion_event_t  channels_cb(dpp::confirmation_callback_t value)
{
    bot->log(dpp::ll_debug, "channel Callback");
    if ( value.is_error() == true ){
        dpp::error_info err = value.get_error();
        bot->log(dpp::ll_error, "Error " + err.message);
    }

    dpp::channel_map channelmap = std::get<dpp::channel_map>(value.value);

    dpp::snowflake id;
    dpp::channel c;

    for (auto& it: channelmap) {
        id = it.first;
        c = it.second;

        bot->log(dpp::ll_debug, c.name);
        channel_map[c.name] = id;

        if (c.name == "devices") {
            bot->messages_get(id, 0, 0, 0, 0, &existing_devices);
        }
    }

    return NULL;
}

dpp::command_completion_event_t  callback(dpp::confirmation_callback_t value)
{
    bot->log(dpp::ll_debug, "Callback");
    if ( value.is_error() == true ){
        dpp::error_info err = value.get_error();
        bot->log(dpp::ll_error, "Error " + err.message);
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

void cdb::DiscordBot::message_cb(cdb::callback_msg * msg)
{
    dpp::message m, s;
    dpp::component c;
    dpp::component ar;

    std::string content = msg->content;
    std::string channel = msg->channel;

    bot->log(dpp::ll_debug, "received message " + std::to_string(msg->type) + " : " + content + " : " + channel);
    switch(msg->type){
        case CDB_MSG_DISC_MQTT_DEV_ADD:
            if (device_map.find(content) == device_map.end()){
                bot->log(dpp::ll_debug, "message not found for " + content);
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

                bot->message_create(m, &my_message_cb);
            } else {
                bot->log(dpp::ll_debug, "message found for " + content + ". Skipping");
            }
            break;
        case CDB_MSG_DISC_MQTT_DEV_STATUS_ON:
        case CDB_MSG_DISC_MQTT_DEV_STATUS_OFF:
            if (device_map.find(content) == device_map.end()){
                bot->log(dpp::ll_warning, "status message not found for " + content);
                return;
            }

            m = bot->message_get_sync(device_map[content], channel_map["devices"]);
            if (msg->type == CDB_MSG_DISC_MQTT_DEV_STATUS_ON){
                m.content    = content + " is ON";
            } else if (msg->type == CDB_MSG_DISC_MQTT_DEV_STATUS_OFF){
                m.content    = content + " is OFF";
            }

            s.channel_id = channel_map["syslog"];
            s.content    = m.content;

            bot->message_edit(m, &my_message_cb);
            bot->message_create(s, &my_message_cb);
            break;
        case CDB_MSG_DISC_POST_FILE:
            m.channel_id = channel_map[channel];
            m.add_file(content.substr(content.find_last_of("\\/"), content.size()),
                       dpp::utility::read_file(content));

            bot->message_create(m, &my_message_cb);
            break;
        case CDB_MSG_DISC_POST_MESSAGE:
            m.channel_id = channel_map[channel];
            m.content    = content;

            bot->message_create(m, &my_message_cb);
            break;
        default:
            break;
    }
}

void cdb::DiscordBot::init(std::string token)
{
    bot = new dpp::cluster(token);

    // Use our own logger for output consistency
    bot->on_log([this](const dpp::log_t & event) {
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


    bot->on_button_click([this](const dpp::button_click_t & event) {
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

    bot->on_ready([](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot->global_command_create(
                dpp::slashcommand("ping", "Ping pong!", bot->me.id)
            );
        }

        bot->log(dpp::ll_debug, "API call");
        bot->current_user_get_guilds(&callback);
    });

    bot->start(dpp::st_return);
}

void cdb::DiscordBot::set_mqtt_handler(cdb::CallbackClass * m_handler)
{
    this->m_handler = m_handler;
}

/*
 * set_logger
 *
 * Set the logger object used for logging
 */
void cdb::DiscordBot::set_logger(cdb::Logger * l)
{
    this->logger = l;
}


