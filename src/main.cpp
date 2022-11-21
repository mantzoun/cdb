/*
 * main.cpp
 *
 * initialize logger object and i/o thread.
 */

#include<stdio.h>

#include "cdb_io.h"
#include "cdb_logger.h"
#include "cdb_discord_bot.h"

int main()
{
    CDB_IO io;
    CDB_DiscordBot bot;
    CDB_Logger logger = CDB_Logger(CDB_LOG_DEBUG);

    io.set_logger(&logger);

    logger.info("start process\n");

    io.fifo_init("/tmp/mqtt_disc_cpp.fifo", NULL);

    bot.init();

    logger.info("bot init done\n");

    while(1);
    logger.info("exit 0\n");
}
