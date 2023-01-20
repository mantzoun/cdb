/*
 * cdb_io.cpp
 *
 * Implementation of CDB_IO class methods
 */

#include <fstream>
#include <unistd.h>

#include "cdb_io.h"

cdb::IO::IO(void)
{
}

void cdb::IO::fifo_read(void)
{
     this->logger->debug("starting fifo");

    std::ifstream fifo{this->fifo_path};
    std::string line;

    while (!this->fifo_stop && !fifo.fail()){
        this->logger->debug("entering fifo io loop");

        while (std::getline(fifo, line)){
        }
        if (fifo.eof()){
            fifo.clear();
        }
        usleep(500000);
    }

    this->logger->warn("exiting fifo io loop");
}

bool cdb::IO::fifo_init(std::string path, void * cb)
{
    if (fifo_t != NULL){
        this->logger->warn("IO thread already running");
        return false;
    }

    this->fifo_path = path;
    this->fifo_cb = cb;
    this->fifo_stop = false;
    this->fifo_t = new std::thread(&cdb::IO::fifo_read, this);

    if (this->fifo_t == NULL){
        return false;
    }

    return true;
}

void cdb::IO::fifo_terminate()
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

void cdb::IO::set_logger(cdb::Logger * l)
{
    this->logger = l;
}
