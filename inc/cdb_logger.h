/*
 * cdb_logger.h
 *
 * Implement all logger functionality in one object, shared
 * between threads. need to check thread safety
 */

#ifndef CDB_LOGGER__H
#define CDB_LOGGER__H

#include <string>

namespace cdb {
    /**
     * @brief Supported logger log severity levels
     */
    enum log_lvl
    {
        CDB_LOG_DEBUG,
        CDB_LOG_INFO,
        CDB_LOG_WARN,
        CDB_LOG_ERROR
    };

    /**
     * @class Logger
     *
     * @brief The logger object will handle logging for all
     *        application modules.
     */
    class Logger
    {
        private:
            cdb::log_lvl level;

        public:
            /**
             * @brief Custom logger constructor
             * @param lvl The log severity level
             */
            Logger(cdb::log_lvl lvl);

            /**
             * @brief Set the log level
             *
             * @param lvl The log level
             */
            void set_level(cdb::log_lvl lvl);

            /**
             * @brief Print a message to stdout, if it exceeds the configured severity
             *
             * @param lvl The message severity
             * @param msg The log message
             */
            void    log(cdb::log_lvl lvl, std::string msg);

            /**
             * @brief wrapper that calls the log() function with DEBUG severity
             *
             * @param msg The log message
             */
            void    debug(std::string msg);

            /**
             * @brief wrapper that calls the log() function with INFO severity
             *
             * @param msg The log message
             */
            void    info(std::string msg);

            /**
             * @brief wrapper that calls the log() function with WANR severity
             *
             * @param msg The log message
             */
            void    warn(std::string msg);

            /**
             * @brief wrapper that calls the log() function with ERRROR severity
             *
             * @param msg The log message
             */
            void    error(std::string msg);
    };
}

#endif /* CDB_LOGGER__H */
