/*
 * cdb_logger.cpp
 *
 * implementation of CDB_Logger class methods
 */

#include <iostream>
#include <ctime>
#include <cstdio>

#include "cdb_logger.h"

 std::string cdb_log_lvl_str[CDB_LOG_ERROR + 1] =
                             {
                                "DEBUG  ",
                                "INFO   ",
                                "WARNING",
                                "ERROR  "
                             };

CDB_Logger::CDB_Logger(cdb_log_lvl lvl)
{
    this->set_level(lvl);
}

/*
 * set_level
 *
 * Set the log level
 */
void CDB_Logger::set_level(cdb_log_lvl lvl)
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
        char res[30];
        sprintf(res, "%02d-%02d-%02d %02d:%02d:%02d %s", ltm->tm_year, ltm->tm_mon, ltm->tm_mday,
                                                         ltm->tm_hour, ltm->tm_min, ltm->tm_sec,
                                                         cdb_log_lvl_str[lvl].c_str());
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
 * info
 *
 * wrapper that calls the log() function with INFO severity
 */
void CDB_Logger::info(std::string s)
{
    this->log(CDB_LOG_INFO, s);
}

/*
 * warn
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
