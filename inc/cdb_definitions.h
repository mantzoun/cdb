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

#define CDB_IO_FIELD_TYPE                   0
#define CDB_IO_FIELD_CHANNEL                1
#define CDB_IO_FIELD_CONTENT                2
#define CDB_IO_FIELD_TOTAL                  3

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
    enum msg_t {
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
    };
}

#endif /* CDB_DEFINITIONS__H */
