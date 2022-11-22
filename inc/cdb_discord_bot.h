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

class CDB_DiscordBot
{
    private:
        CDB_Logger * logger = NULL;

    public:
        CDB_DiscordBot(void);
        void init(std::string);
        void set_logger(CDB_Logger *);
};

#endif /* CDB_DISCORD_BOT__H */
