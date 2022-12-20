#ifndef __CDB_IO__H
#define __CDB_IO__H

#include<thread>

#include "cdb_logger.h"

/** Class CDB_IO handles the local input/output for
 * the running process
 *
 * Currently only oneway reading from a named fifo
 * is implemented
 */
class CDB_IO
{
private:
    bool        fifo_stop;
    std::thread * fifo_t = NULL;
    std::string fifo_path;
    void        * fifo_cb;
    CDB_Logger  * logger = NULL;

   /**
    * fifo_read
    *
    * read from the defined named fifo
    */
    void    fifo_read(void);

public:
    /** Default constructor
     */
            CDB_IO(void);
    /** Initialize the named fifo
     *
     * Start the thread that will parse input from the fifo
     */
    bool    fifo_init(std::string, void *);

    /** Terminate the fifo
     *
     * stop the fifo_read thread
     */
    void    fifo_terminate(void);

    /** Set the logger object
     */
    void    set_logger(CDB_Logger *);
};

#endif /* __CDB_IO__H */
