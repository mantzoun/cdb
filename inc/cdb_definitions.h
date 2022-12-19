#ifndef CDB_DEFINITIONS__H
#define CDB_DEFINITIONS__H

#define CDB_OK                              0
#define CDB_NOK                             1

/* CONFIGURATION */
#define CDB_CONF_DISCORD_TYPE               0
#define CDB_CONF_DISCORD_TOKEN              1
#define CDB_CONF_DISCORD_PARAMS             2

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

/* MESSAGES -> DISCORD BOT */
#define CDB_DISC_MSG_MQTT_DEV_ADD           0
#define CDB_DISC_MSG_MQTT_DEV_STATUS_ON     1
#define CDB_DISC_MSG_MQTT_DEV_STATUS_OFF    2

/* MESSAGES -> MQTT HANDLER */
#define CDB_MQTT_HANDLER_TURN_DEVICE_ON     0
#define CDB_MQTT_HANDLER_TURN_DEVICE_OFF    1
#define CDB_MQTT_HANDLER_POLL_DEVICE_STATUS 2

#endif /* CDB_DEFINITIONS__H */
