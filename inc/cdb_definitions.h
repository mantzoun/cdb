/**
 * One place to hold definitions and enums
 * for the project
 */
#ifndef CDB_DEFINITIONS__H
#define CDB_DEFINITIONS__H

#define CDB_OK                              0
#define CDB_NOK                             1

/* CONFIGURATION */
#define CDB_CONF_DISCORD_TYPE               0
#define CDB_CONF_DISCORD_BOT_ID             1
#define CDB_CONF_DISCORD_TOKEN              2
#define CDB_CONF_DISCORD_PARAMS             3

#define CDB_CONF_MQTT_DEVICE_TYPE           0
#define CDB_CONF_MQTT_DEVICE_NAME           1
#define CDB_CONF_MQTT_DEVICE_CMND           2
#define CDB_CONF_MQTT_DEVICE_STAT           3
#define CDB_CONF_MQTT_DEVICE_PARAMS         4

#define CDB_CONF_MQTT_SERVER_TYPE           0
#define CDB_CONF_MQTT_SERVER_ADDR           1
#define CDB_CONF_MQTT_SERVER_PORT           2
#define CDB_CONF_MQTT_SERVER_USERNAME       3
#define CDB_CONF_MQTT_SERVER_PASSWORD       4
#define CDB_CONF_MQTT_SERVER_CLIENT_ID      5
#define CDB_CONF_MQTT_SERVER_PARAMS         6

/* FIFO INPUT */
#define CDB_IO_FIFO_COMMAND                 0
#define CDB_IO_FIFO_FIELD1                  1
#define CDB_IO_FIFO_FIELD2                  2
#define CDB_IO_FIFO_FIELD3                  3
#define CDB_IO_FIFO_FIELD_TOTAL             4

#define LOG(__logger, __log_level, __message) \
    { \
        std::string __msg =  __message + \
                            std::string(" (") + \
                            std::string(__FILE__) + \
                            std::string(":") + \
                            std::to_string(__LINE__) + \
                            std::string(")"); \
        (__logger)->log(__log_level, __msg); \
    }

#define LOG_DEBUG(__logger, __message)   LOG(__logger, cdb::CDB_LOG_DEBUG, __message)
#define LOG_INFO(__logger, __message)    LOG(__logger, cdb::CDB_LOG_INFO, __message)
#define LOG_WARN(__logger, __message)    LOG(__logger, cdb::CDB_LOG_WARN, __message)
#define LOG_ERROR(__logger, __message)   LOG(__logger, cdb::CDB_LOG_ERROR, __message)

namespace cdb {
    /**
      * @brief Enum of all cdb message types
      *
      *        used for messages sent to objects derived
      *        from the Callback_Class
      */
    enum cdb_intra_msg_mqtt_t {
        CDB_INTRA_MQTT_TOPIC_SUB,
        CDB_INTRA_MQTT_TOPIC_UPDATE,
        CDB_INTRA_MQTT_TOPIC_PUBLISH,
    };

    enum cdb_intra_msg_bot_t {
        CDB_INTRA_DISC_BOT_COMMAND,
        CDB_INTRA_DISC_BOT_POST_TEXT,
        CDB_INTRA_DISC_BOT_POST_FILE,
        CDB_INTRA_DISC_BOT_DEVICE_ADD,
        CDB_INTRA_DISC_BOT_DEVICE_SET,
    };
/*    enum msg_t {
        // MESSAGES -> DISCORD BOT
        CDB_MSG_DISC_MQTT_DEV_ADD,
        CDB_MSG_DISC_MQTT_DEV_STATUS_ON,
        CDB_MSG_DISC_MQTT_DEV_STATUS_OFF,

        CDB_MSG_DISC_POST_MESSAGE,
        CDB_MSG_DISC_POST_FILE,

        // MESSAGES -> MQTT HANDLER
        CDB_MSG_MQTT_HANDLER_TURN_DEVICE_ON,
        CDB_MSG_MQTT_HANDLER_TURN_DEVICE_OFF,
        CDB_MSG_MQTT_HANDLER_POLL_DEVICE_STATUS,

        CDB_MSG_MAX
    };*/
}

#endif /* CDB_DEFINITIONS__H */
