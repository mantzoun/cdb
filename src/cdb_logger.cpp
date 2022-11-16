/*
 * cdb_logger.cpp
 *
 * implementation of CDB_Logger class methods
 */

#include <iostream>
#include <ctime>
#include <cstdio>

#include "cdb_logger.h"

CDB_Logger::CDB_Logger(cdb_log_lvl lvl)
{
    this->level = lvl;
}

/*
 * log
 *
 * Print a message to stdout, if it exceeds the configured severity
 */
void CDB_Logger::log(cdb_log_lvl lvl, std::string s)
{
    if (lvl >= this->level){
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char res[20];
        sprintf(res, "%2d-%2d-%2d %2d:%2d:%2d", ltm->tm_year, ltm->tm_mon, ltm->tm_mday,
                                                ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
        std::string tstamp = res;
        std::cout << tstamp + " " + s + "\n";
        std::cout.flush();
    }
}

/*
 * debug
 *
 * wrapper that calls the log() function with DEBUG severity
 */
void CDB_Logger::debug(std::string s)
{
    this->log(CDB_LOG_DEBUG, s);
}

/*
 * debug
 *
 * wrapper that calls the log() function with INFO severity
 */
void CDB_Logger::info(std::string s)
{
    this->log(CDB_LOG_INFO, s);
}

/*
 * debug
 *
 * wrapper that calls the log() function with WANR severity
 */
void CDB_Logger::warn(std::string s)
{
    this->log(CDB_LOG_WARN, s);
}

/*
 * error
 *
 * wrapper that calls the log() function with ERRROR severity
 */
void CDB_Logger::error(std::string s)
{
    this->log(CDB_LOG_ERROR, s);
}
