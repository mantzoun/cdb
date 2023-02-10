/*
 * cdb_discord_bot_commands.cpp
 *
 * implementation of CDB_DiscordBot methods
 */

#include "cdb_discord_bot.h"

void register_mqtt_device_info(cdb::DiscordBot * bot)
{
    dpp::slashcommand newcommand("mqtt_device_info", "Print MQTT device information", bot->me.id);
    newcommand.add_option(
        dpp::command_option(dpp::co_string, "device_name", "The device name", true)
    );

    bot->global_command_create(newcommand);
}

void handle_mqtt_device_info(const dpp::slashcommand_t & event)
{
    std::string device = std::get<std::string>(event.get_parameter("device_name"));

    event.reply(std::string("//TODO device info: ") + device);
}

void register_mqtt_device_list(cdb::DiscordBot * bot)
{
    dpp::slashcommand newcommand("mqtt_device_list", "List MQTT devices", bot->me.id);

    bot->global_command_create(newcommand);
}

void handle_mqtt_device_list(const dpp::slashcommand_t & event)
{
    event.reply(std::string("//TODO device list"));
}

void cdb::DiscordBot::register_commands(void)
{
    if (dpp::run_once<struct register_bot_commands>()) {
        register_mqtt_device_info(this);
        register_mqtt_device_list(this);
    }
}

void cdb::DiscordBot::handle_slashcommand(const dpp::slashcommand_t & event){
    if (event.command.get_command_name() == "mqtt_device_list") {
        handle_mqtt_device_list(event);
    } else if (event.command.get_command_name() == "mqtt_device_info") {
        handle_mqtt_device_info(event);
    }
}
