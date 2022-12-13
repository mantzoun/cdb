/*
 * cdb_logger.h
 *
 * Implement all logger functionality in one object, shared
 * between threads. need to check thread safety
 */

#ifndef CDB_LOGGER__H
#define CDB_LOGGER__H

#include<string>

enum cdb_log_lvl
{
    CDB_LOG_DEBUG,
    CDB_LOG_INFO,
    CDB_LOG_WARN,
    CDB_LOG_ERROR
};

class CDB_Logger
{
    private:
        cdb_log_lvl level;

    public:
                CDB_Logger(cdb_log_lvl);

        void    set_level(cdb_log_lvl);
        void    log(cdb_log_lvl, std::string);

        void    debug(std::string);
        void    info(std::string);
        void    warn(std::string);
        void    error(std::string);
};

#endif /* CDB_LOGGER__H */
