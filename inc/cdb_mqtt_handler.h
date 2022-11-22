#ifndef CDB_MQTT_HANDLER__H
#define CDB_MQTT_HANDLER__H

#include <mosquitto.h>

#include "cdb_mqtt_device.h"
#include "cdb_mqtt_server.h"
#include "cdb_logger.h"

#define CDB_MQTT_OK     0
#define CDB_MQTT_NOK    1

class CDB_MQTT_Handler {
    private:
        static CDB_Logger * logger;
        mosquitto * mosq;

        void (CDB_MQTT_Handler::*loggerptr)(mosquitto *, void *, int, const char *);

        static void mosq_logger(mosquitto *mosq, void *obj, int level, const char *str);

    public:
        CDB_MQTT_Handler(void);
        void add_device(CDB_MQTT_Device * device);
        void set_logger(CDB_Logger *);
        void init(CDB_MQTT_Server *);
        int connect(CDB_MQTT_Server *);
        void publish(void);
};

#endif /* CDB_MQTT_HANDLER__H */
