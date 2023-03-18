#ifndef CDB_CALLBACK_CLASS__H
#define CDB_CALLBACK_CLASS__H

#include <string>

#include "cdb_definitions.h"
#include "cdb_intra_messaging.h"

namespace cdb {
    /**
     * @class CallbackClass
     *
     * @brief Callback abstract class
     *
     *        Classes that need a messaging interface
     *        are derived from this class.
     */
    class CallbackClass {
        public:
            /**
             * @brief Interface to accept messages from other modules
             *
             * @param msg the message information
             */
            virtual void message_cb(cdb::intra_msg_t * msg) = 0;
    };
}
#endif /* CDB_CALLBACK_CLASS__H */
