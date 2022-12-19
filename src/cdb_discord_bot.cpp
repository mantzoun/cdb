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

static std::map<std::string, dpp::snowflake> device_map;

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
        std::string device_name;
        dpp::message m = it.second;

        if (m.channel_id == devices_sf){
            size_t fs = m.content.find(" ");

            if (fs == std::string::npos){
                device_name = m.content;
            } else {
                device_name = m.content.substr(0, fs);
            }

            std::cout << "found message for " + device_name + "\n";
            device_map[device_name] = m.id;
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
        device_map[m.content] = m.id;
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

void CDB_DiscordBot::message_cb(cdb_callback_msg * msg)
{
    dpp::message m, s;
    dpp::component c;
    dpp::component ar;

    bot->log(dpp::ll_debug, "received message");
    bot->log(dpp::ll_debug, "received message " + msg->msg);
    switch(msg->type){
        case CDB_DISC_MSG_MQTT_DEV_ADD:
            if (device_map.find(msg->msg) == device_map.end()){
                bot->log(dpp::ll_debug, "message not found for " + msg->msg);
                m.channel_id = devices_sf;
                m.content    = msg->msg;

                ar.set_type(dpp::cot_action_row);

                c.set_style(dpp::cos_success);
                c.set_label("ON");
                c.set_id(msg->msg + "#ON");
                c.set_type(dpp::cot_button);

                ar.add_component(c);

                c.set_style(dpp::cos_danger);
                c.set_label("OFF");
                c.set_id(msg->msg + "#OFF");
                c.set_type(dpp::cot_button);

                ar.add_component(c);

                m.add_component(ar);

                bot->message_create(m, &my_message_cb);
            } else {
                bot->log(dpp::ll_debug, "message found for " + msg->msg + ". Skipping");
            }
            break;
        case CDB_DISC_MSG_MQTT_DEV_STATUS_ON:
        case CDB_DISC_MSG_MQTT_DEV_STATUS_OFF:
            if (device_map.find(msg->msg) == device_map.end()){
                bot->log(dpp::ll_warning, "status message not found for " + msg->msg);
                return;
            }

            m = bot->message_get_sync(device_map[msg->msg], devices_sf);
            if (msg->type == CDB_DISC_MSG_MQTT_DEV_STATUS_ON){
                m.content    = msg->msg + " is ON";
            } else if (msg->type == CDB_DISC_MSG_MQTT_DEV_STATUS_OFF){
                m.content    = msg->msg + " is OFF";
            }

            s.channel_id = syslog_sf;
            s.content    = m.content;

            bot->message_edit(m, &my_message_cb);
            bot->message_create(s, &my_message_cb);
            break;
        default:
            break;
    }
}

void CDB_DiscordBot::init(std::string token)
{
    bot = new dpp::cluster(token);

    bot->on_log(dpp::utility::cout_logger());

    bot->on_button_click([this](const dpp::button_click_t & event) {
        /* Button clicks are still interactions, and must be replied to in some form to
         * prevent the "this interaction has failed" message from Discord to the user.
         */
        event.reply();

        char dev_id[50];
        char * tmp;
        int index = 0;
        cdb_callback_msg msg = {255, "N/A"};

        this->logger->debug("parse message  " + event.custom_id);

        strncpy(dev_id, event.custom_id.c_str(), sizeof(dev_id));
        tmp = strtok(dev_id, "#");

        while (tmp != NULL)
        {
            switch(index++){
                case 0:
                    msg.msg=tmp;
                    this->logger->debug("message is " + msg.msg);
                    break;
                case 1:
                    if (strcmp(tmp, "ON") == 0) {
                        msg.type = CDB_MQTT_HANDLER_TURN_DEVICE_ON;
                    } else if (strcmp(tmp, "OFF") == 0) {
                        msg.type = CDB_MQTT_HANDLER_TURN_DEVICE_OFF;
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

        std::cout << "API call\n";
        bot->current_user_get_guilds(&callback);
    });

    bot->start(dpp::st_return);
}

void CDB_DiscordBot::set_mqtt_handler(CDB_Callback_Class * m_handler)
{
    this->m_handler = m_handler;
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


