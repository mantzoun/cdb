/*
 * main.cpp
 *
 * initialize logger object and i/o thread.
 */

#include<stdio.h>

#include "cdb_io.h"
#include "cdb_logger.h"

int main()
{
    CDB_IO io;
    CDB_Logger logger = CDB_Logger(CDB_LOG_DEBUG);

    io.set_logger(&logger);

    logger.info("start process\n");

    io.fifo_init("/tmp/mqtt_disc_cpp.fifo", NULL);

    while(1);
    logger.info("exit 0\n");
}
