#ifndef __CDB_IO__H
#define __CDB_IO__H

#include<thread>

#include "cdb_logger.h"

namespace cdb {
    /**
     * @class IO
     *
     * @brief Class CDB_IO handles the local input/output for
     *        the running process
     *
     *        Currently only oneway reading from a named fifo
     *        is implemented
     */
    class IO
    {
    private:
        bool        fifo_stop;
        std::thread * fifo_t = NULL;
        std::string fifo_path;
        void        * fifo_cb;
        cdb::Logger * logger = NULL;

       /**
        * fifo_read
        *
        * read from the defined named fifo
        */
        void fifo_read(void);

    public:
        /**
         * @brief Default constructor
         */
        IO(void);

        /**
         * @brief Initialize the named fifo
         *
         *        Start the thread that will parse input from the fifo
         *
         * @return true if in case os success
         */
        bool fifo_init(std::string, void *);

        /**
         * @brief Terminate the fifo
         *
         *        stop the fifo_read thread
         */
        void fifo_terminate(void);

        /**
         * @brief Set the logger object
         *
         * @param logger The logger object
         */
        void set_logger(cdb::Logger * logger);
    };
}

#endif /* __CDB_IO__H */
