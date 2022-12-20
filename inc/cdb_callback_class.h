#ifndef CDB_CALLBACK_CLASS__H
#define CDB_CALLBACK_CLASS__H

#include <string>

#include "cdb_definitions.h"

/**
 * the message to be sent to the class interface
 */
struct cdb_callback_msg
{
    cdb_msg_t   type;       /*!< Messge type */
    std::string content;    /*!< Messge content */
};

/** Callback abstract class
 *
 * Classes that need a messaging interface
 * are derived from this class.
 */
class CDB_Callback_Class {
    public:
        /**
         * Interface to accept messages from other modules
         */
        virtual void message_cb(cdb_callback_msg *) = 0;
};

#endif /* CDB_CALLBACK_CLASS__H */
