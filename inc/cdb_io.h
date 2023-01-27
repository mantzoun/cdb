#ifndef __CDB_IO__H
#define __CDB_IO__H

#include<thread>

#include "cdb_logger.h"
#include "cdb_discord_bot.h"

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
        cdb::CallbackClass * bot;

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
         * @param fifo The path of the fifo
         *
         * @return true if in case os success
         */
        bool fifo_init(std::string fifo);

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

        /**
         * @brief Set a pointer to the discord bot module
         *
         *        The bot is able to receive messages from the
         *        IO.
         *
         * @param bot The discord bot object
         */
        void set_discord_bot(cdb::CallbackClass * bot);
    };
}

#endif /* __CDB_IO__H */
