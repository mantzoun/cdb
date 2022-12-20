/*
 * cdb_discord_bot.h
 *
 * handle connection with the Discord servers
 * uses the DPP library
 * https://dpp.dev/
 */

#ifndef CDB_DISCORD_BOT__H
#define CDB_DISCORD_BOT__H

#include "cdb_logger.h"
#include "cdb_mqtt_handler.h"

class CDB_DiscordBot : public CDB_Callback_Class
{
    private:
        CDB_Logger * logger = NULL;
        CDB_Callback_Class * m_handler = NULL;

    public:
        CDB_DiscordBot(void);
        void init(std::string);
        void set_logger(CDB_Logger *);
        void set_mqtt_handler(CDB_Callback_Class *);
        void message_cb(cdb_callback_msg *msg);
};

#endif /* CDB_DISCORD_BOT__H */
