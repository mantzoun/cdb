#include "cdb_mqtt_handler.h"

CDB_Logger * CDB_MQTT_Handler::logger;

CDB_MQTT_Handler::CDB_MQTT_Handler(void)
{
}

void CDB_MQTT_Handler::set_logger(CDB_Logger * l)
{
    logger = l;
}

void CDB_MQTT_Handler::mosq_logger(mosquitto *mosq, void *obj, int level, const char *str)
{
    std::string msg = str;
    logger->debug("MQTT Handler: " + msg);
}

void CDB_MQTT_Handler::init(CDB_MQTT_Server * conf)
{
    mosquitto_lib_init();

    this->mosq = mosquitto_new(conf->client_id().c_str(), true, NULL);
    mosquitto_username_pw_set(this->mosq, conf->username().c_str(), conf->password().c_str());

    mosquitto_connect_callback_set(this->mosq, conf->on_connect());
    mosquitto_disconnect_callback_set(this->mosq, conf->on_disconnect());
    mosquitto_message_callback_set(this->mosq, conf->on_message());

    mosquitto_log_callback_set(this->mosq, &CDB_MQTT_Handler::mosq_logger);

    mosquitto_loop_start(this->mosq);
}

int CDB_MQTT_Handler::connect(CDB_MQTT_Server * conf)
{
    int res = mosquitto_connect(this->mosq, conf->addr().c_str(), conf->port(), 30);
    if (MOSQ_ERR_SUCCESS != res){
        logger->error("Connection to MQTT broker failed, error " + std::to_string(res));
        return CDB_MQTT_NOK;
    }

    logger->info("Connected to MQTT broker.");
    return CDB_MQTT_OK;
}

void CDB_MQTT_Handler::add_device(CDB_MQTT_Device * device)
{
    mosquitto_subscribe(this->mosq, NULL, device->stat().c_str(), 1);
}

void CDB_MQTT_Handler::publish(void)
{
//    mosquitto_publish(m, NULL, topic, 0, NULL, 1, false);
}
