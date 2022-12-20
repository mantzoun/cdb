#ifndef CDB_MQTT_HANDLER__H
#define CDB_MQTT_HANDLER__H

#include <mosquitto.h>

#include "cdb_callback_class.h"
#include "cdb_configurator.h"
#include "cdb_mqtt_device.h"
#include "cdb_mqtt_server.h"
#include "cdb_discord_bot.h"
#include "cdb_logger.h"

/** The MQTT handler class
 *
 * This class is responsible for the connection to the MQTT broker.
 * It will send and received messages to and from the broker, as well
 * as to and from other modules that require broker information or
 * interaction
 */
class CDB_MQTT_Handler : public CDB_Callback_Class
{
    private:
        static CDB_Logger * logger;
        mosquitto * mosq;
        CDB_Callback_Class * bot;

        void (CDB_MQTT_Handler::*loggerptr)(mosquitto *, void *, int, const char *);

        static void mosq_logger(mosquitto *mosq, void *obj, int level, const char *str);

    public:
        /** Default constructor
         */
        CDB_MQTT_Handler(void);

        /** Add a device to the list managed by the handler
         *
         * @param device The attributes of the device to be
         *               added. These will include the stat topic,
         *               the command topic and the device ID
         */
        void add_device(CDB_MQTT_Device * device);

        /** Get a pointer to a logger object
         */
        void set_logger(CDB_Logger *);

        /** Get a pointer to the discord bot module
         *
         * The bot is able to receive messages from the
         * handler for any updates from the managed devices
         */
        void set_discord_bot(CDB_Callback_Class *);

        /** Initialize the handler
         *
         * Connect to the broker and subscrive to device topics
         */
        void init(CDB_Configurator *);

        /** connecto to the MQTT broker
         */
        int connect(CDB_MQTT_Server *);

        /** publish a message on a MQTT topic
         */
        void publish(const char * topic, int payload_len, const void * payload);

        /** receive a message from another module
         */
        void message_cb(cdb_callback_msg *);
};

#endif /* CDB_MQTT_HANDLER__H */
