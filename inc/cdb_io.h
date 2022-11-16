/*
 * cdb_io.h
 *
 * handle process I/O with local system.
 * Currently supports input via named fifo
 */

#ifndef __CDB_IO__H
#define __CDB_IO__H

#include<thread>

#include "cdb_logger.h"

class CDB_IO
{
private:
    bool        fifo_stop;
    std::thread * fifo_t = NULL;
    std::string fifo_path;
    void        * fifo_cb;
    CDB_Logger  * logger = NULL;

    void    fifo_read(void);

public:
            CDB_IO(void);
    bool    fifo_init(std::string, void *);
    void    fifo_terminate(void);
    void    set_logger(CDB_Logger *);
};

#endif /* __CDB_IO__H */
