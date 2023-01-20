#ifndef CDB_MQTT_HANDLER__H
#define CDB_MQTT_HANDLER__H

#include <mosquitto.h>

#include "cdb_callback_class.h"
#include "cdb_configurator.h"
#include "cdb_mqtt_device.h"
#include "cdb_mqtt_server.h"
#include "cdb_discord_bot.h"
#include "cdb_logger.h"

namespace cdb {
    /**
     * @class MqttHandler
     *
     * @brief This class is responsible for the connection to the MQTT broker.
     *        It will send and received messages to and from the broker, as well
     *        as to and from other modules that require broker information or
     *        interaction
     */
    class MqttHandler : public cdb::CallbackClass
    {
        private:
            static cdb::Logger * logger;
            mosquitto * mosq;
            cdb::CallbackClass * bot;

            void (cdb::MqttHandler::*loggerptr)(mosquitto *, void *, int, const char *);

            static void mosq_logger(mosquitto *mosq, void *obj, int level, const char *str);

        public:
            /**
             * @brief Default constructor
             */
            MqttHandler(void);

            /**
             * @brief Add a device to the list managed by the handler
             *
             * @param device The attributes of the device to be
             *               added. These will include the stat topic,
             *               the command topic and the device ID
             */
            void add_device(cdb::MqttDevice * device);

            /**
             * @brief Set a pointer to a logger object
             *
             * @param logger The logger object
             */
            void set_logger(cdb::Logger * logger);

            /**
             * @brief Set a pointer to the discord bot module
             *
             *        The bot is able to receive messages from the
             *        handler for any updates from the managed devices
             *
             * @param bot The discord bot object
             */
            void set_discord_bot(cdb::CallbackClass * bot);

            /**
             * @brief Initialize the handler
             *
             *        Connect to the broker and subscrive to device topics
             *
             * @param conf The configurator
             */
            void init(cdb::Configurator * conf);

            /**
             * @brief connecto to the MQTT broker
             *
             * @param server The MQTT Broker
             */
            int connect(cdb::MqttServer * server);

            /**
             * @brief publish a message on a MQTT topic
             *
             * @param topic        The topic to post to
             * @param payload_len  The message payload length
             * @param payload      The message payload
             */
            void publish(const char * topic, int payload_len, const void * payload);

            /**
             * @brief Receive a message from another module
             *
             * @param msg The message information
             */
            void message_cb(cdb::callback_msg * msg);
    };
}

#endif /* CDB_MQTT_HANDLER__H */
