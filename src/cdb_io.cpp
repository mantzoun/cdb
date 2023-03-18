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
    LOG_DEBUG(this->logger,"CDB_IO: starting fifo");

    std::ifstream fifo{this->fifo_path};
    std::string line;

    while (!this->fifo_stop && !fifo.fail()){
        while (std::getline(fifo, line)){
        LOG_DEBUG(this->logger,"CDB_IO: fifo input: " + line);
            int i = 0;
            cdb::intra_msg_t msg;

            msg.sender = CDB_IO_HANDLER;
            io_msg_t io_msg;
            msg.content.io = &io_msg;

            for (i = 0; i < CDB_IO_FIFO_FIELD_TOTAL; i++){
                size_t delim = line.find("#");

                if (delim == std::string::npos){
                    break;
                }

                std::string str = line.substr(0, delim);
                line.erase(0, delim + 1);

                switch(i) {
                    case CDB_IO_FIFO_COMMAND:
                        io_msg.command = str;
                        break;
                    case CDB_IO_FIFO_FIELD1:
                        io_msg.field1 = str;
                        break;
                    case CDB_IO_FIFO_FIELD2:
                        io_msg.field2 = str;
                        break;
                    case CDB_IO_FIFO_FIELD3:
                        io_msg.field3 = str;
                        break;
                    default:
                        break;
                }
            }

            if (i == CDB_IO_FIFO_FIELD_TOTAL) {
                this->message_handler->message_cb(&msg);
            }
        }
        if (fifo.eof()){
            fifo.clear();
        }
        usleep(500000);
    }

    LOG_WARN(this->logger,"CDB_IO: exiting fifo io loop");
}

bool cdb::IO::fifo_init(std::string path)
{
    if (fifo_t != NULL){
        LOG_WARN(this->logger,"CDB_IO: IO thread already running");
        return false;
    }

    this->fifo_path = path;
    this->fifo_stop = false;
    this->fifo_t = new std::thread(&cdb::IO::fifo_read, this);

    if (this->fifo_t == NULL){
        return false;
    }

    return true;
}

bool cdb::IO::fifo_terminate()
{
    if (this->fifo_t == NULL){
         LOG_WARN(this->logger,"CDB_IO: IO thread not running");
        return false;
    }

    this->fifo_stop = true;
    this->fifo_t->join();
    delete this->fifo_t;
    this->fifo_t = NULL;

    return true;
}

void cdb::IO::set_logger(cdb::Logger * l)
{
    this->logger = l;
}

void cdb::IO::set_message_handler(cdb::CallbackClass * handler)
{
    this->message_handler = handler;
}

