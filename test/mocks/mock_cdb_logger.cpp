/*
 * mock_cdb_logger.cpp
 *
 * mock implementation of CDB_Logger class methods
 */

#include <iostream>
#include <ctime>
#include <cstdio>

#include "cdb_logger.h"

std::string cdb_log_lvl_str[cdb::CDB_LOG_ERROR + 1] =
                             {
                                "DEBUG  ",
                                "INFO   ",
                                "WARNING",
                                "ERROR  "
                             };

cdb::Logger::Logger(cdb::log_lvl lvl)
{
    this->set_level(lvl);
}

void cdb::Logger::set_level(cdb::log_lvl lvl)
{
    this->level = lvl;
}

void cdb::Logger::log(cdb::log_lvl lvl, std::string s)
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

void cdb::Logger::debug(std::string s)
{
    this->log(cdb::CDB_LOG_DEBUG, s);
}

void cdb::Logger::info(std::string s)
{
    this->log(cdb::CDB_LOG_INFO, s);
}

void cdb::Logger::warn(std::string s)
{
    this->log(cdb::CDB_LOG_WARN, s);
}

void cdb::Logger::error(std::string s)
{
    this->log(cdb::CDB_LOG_ERROR, s);
}
