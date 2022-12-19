#ifndef CDB_CALLBACK_CLASS__H
#define CDB_CALLBACK_CLASS__H

#include <string>

struct cdb_callback_msg
{
    uint8_t     type;
    std::string msg;
};

class CDB_Callback_Class {
    public:
        virtual void message_cb(cdb_callback_msg *) = 0;
};

#endif /* CDB_CALLBACK_CLASS__H */
