#ifndef CDB_CALLBACK_CLASS__H
#define CDB_CALLBACK_CLASS__H

#include <string>

#include "cdb_definitions.h"

namespace cdb {
    /**
     * @class callback_msg
     *
     * @brief the message to be sent to the class interface
     */
    struct callback_msg
    {
        cdb::msg_t  type;       /*!< Messge type */
        std::string content;    /*!< Messge content */
    };

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
            virtual void message_cb(cdb::callback_msg * msg) = 0;
    };
}
#endif /* CDB_CALLBACK_CLASS__H */
