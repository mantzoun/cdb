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

#define     CDB_DISC_MSG_MQQT_DEV_ADD   1

struct cdb_disc_msg
{
    uint8_t     type;
    std::string msg;
};

class CDB_DiscordBot
{
    private:
        CDB_Logger * logger = NULL;

    public:
        CDB_DiscordBot(void);
        void init(std::string);
        void set_logger(CDB_Logger *);
        void message_cb(cdb_disc_msg *msg);
};

#endif /* CDB_DISCORD_BOT__H */
