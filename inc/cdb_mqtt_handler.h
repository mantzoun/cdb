#ifndef CDB_MQTT_HANDLER__H
#define CDB_MQTT_HANDLER__H

#include <mosquitto.h>

#include "cdb_callback_class.h"
#include "cdb_configurator.h"
#include "cdb_mqtt_device.h"
#include "cdb_mqtt_server.h"
#include "cdb_discord_bot.h"
#include "cdb_logger.h"

class CDB_MQTT_Handler : public CDB_Callback_Class
{
    private:
        static CDB_Logger * logger;
        mosquitto * mosq;
        CDB_Callback_Class * bot;

        void (CDB_MQTT_Handler::*loggerptr)(mosquitto *, void *, int, const char *);

        static void mosq_logger(mosquitto *mosq, void *obj, int level, const char *str);

    public:
        CDB_MQTT_Handler(void);
        void add_device(CDB_MQTT_Device * device);
        void set_logger(CDB_Logger *);
        void set_discord_bot(CDB_Callback_Class *);
        void init(CDB_Configurator *);
        int connect(CDB_MQTT_Server *);
        void publish(const char * topic, int payload_len, const void * payload);
        void send_command();
        void message_cb(cdb_callback_msg *);
};

#endif /* CDB_MQTT_HANDLER__H */
