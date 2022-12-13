/*
 * cdb_io.cpp
 *
 * implementation of CDB_IO class methods
 */

#include <fstream>
#include <unistd.h>

#include "cdb_io.h"


CDB_IO::CDB_IO(void)
{
}

/*
 * fifo_read
 *
 * read from the defined named fifo
 */
void CDB_IO::fifo_read(void)
{
     this->logger->debug("starting fifo");

    std::ifstream fifo{this->fifo_path};
    std::string line;

    while (!this->fifo_stop && !fifo.fail()){
        this->logger->debug("entering fifo io loop");

        while (std::getline(fifo, line)){
//            std::cout << line << '\n';
        }
        if (fifo.eof()){
            fifo.clear();
        }
        usleep(500000);
    }

    this->logger->warn("exiting fifo io loop");
}

/*
 * fifo_init
 *
 * Start the thread that will parse input from the fifo
 */
bool CDB_IO::fifo_init(std::string path, void * cb)
{
    if (fifo_t != NULL){
        this->logger->warn("IO thread already running");
        return false;
    }

    this->fifo_path = path;
    this->fifo_cb = cb;
    this->fifo_stop = false;
    this->fifo_t = new std::thread(&CDB_IO::fifo_read, this);

    if (this->fifo_t == NULL){
        return false;
    }

    return true;
}

/*
 * fifo_terminate
 *
 * stop the fifo_read thread
 */
void CDB_IO::fifo_terminate()
{
    if (this->fifo_t == NULL){
         this->logger->warn("IO thread not running");
        return;
    }

    this->fifo_stop = true;
    this->fifo_t->join();
    delete this->fifo_t;
    this->fifo_t = NULL;
}

/*
 * set_logger
 *
 * Set the logger object used for logging
 */
void CDB_IO::set_logger(CDB_Logger * l)
{
    this->logger = l;
}
