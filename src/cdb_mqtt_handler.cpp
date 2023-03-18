#include <map>
#include "string"
#include "cstring"
#include "iostream"

#include "cdb_mqtt_handler.h"

cdb::Logger * cdb::MqttHandler::logger;
cdb::CallbackClass * my_message_handler;

static std::map<std::string, std::string> stat_topic_map;
static std::map<std::string, std::string> cmnd_topic_map;

static void on_connect(mosquitto * m, void *obj, int res)
{
    cdb::MqttHandler * handler = (cdb::MqttHandler *) obj;
    handler->on_connect_cb();
}

static void on_disconnect(mosquitto * m, void *obj, int res)
{
    cdb::MqttHandler * handler = (cdb::MqttHandler *) obj;
    handler->on_disconnect_cb();
}

static void on_message(mosquitto * m, void *obj, const mosquitto_message *message)
{
    cdb::MqttHandler * handler = (cdb::MqttHandler *) obj;
    handler->on_message_cb(message);
}

cdb::MqttHandler::MqttHandler(void)
{
}

void cdb::MqttHandler::on_connect_cb(void)
{
    cdb::Configurator *conf = this->conf;
    std::list<cdb::MqttDevice *> * l = conf->inventory.mqtt_dev_list();
    LOG_INFO(logger, "MQTT Handler: On Connect");

    std::list<cdb::MqttDevice *>::iterator it;
    for (it = l->begin(); it != l->end(); ++it){
        LOG_INFO(logger, "MQTT Handler: Adding device " + (*it)->name());
        this->add_device(*it);

        stat_topic_map[(*it)->stat()] = (*it)->name();
        cmnd_topic_map[(*it)->name()] = (*it)->cmnd();

        cdb::intra_msg_t msg;
        cdb::mqtt_handler_msg_t mqtt = { CDB_INTRA_MQTT_TOPIC_SUB,
                                         (*it)->name(),
                                         ""
                                       };
        msg.sender       = CDB_MQTT_HANDLER;
        msg.content.mqtt = &mqtt;
        this->message_handler->message_cb(&msg);
    }
}

void cdb::MqttHandler::on_disconnect_cb(void)
{
    LOG_WARN(logger, "MQTT Handler: On Disconnect");
}

void cdb::MqttHandler::on_message_cb(const mosquitto_message *message)
{
    std::string msg = (char*) message->payload;
    LOG_INFO(logger, "MQTT Handler: Received message " + msg + " for device " + stat_topic_map[message->topic]);

    if (stat_topic_map.find(message->topic) != stat_topic_map.end()){
        if ( strcmp((char*) message->payload, "ON") != 0 &&
             strcmp((char*) message->payload, "OFF") != 0 ) {
            LOG_WARN(logger, "MQTT Handler: Could not determine message type");
            return;
        }

        cdb::intra_msg_t msg;
        std::string update = (char*) message->payload;
        cdb::mqtt_handler_msg_t mqtt = { CDB_INTRA_MQTT_TOPIC_UPDATE,
                                         stat_topic_map[message->topic],
                                         update
                                       };
        msg.sender       = CDB_MQTT_HANDLER;
        msg.content.mqtt = &mqtt;
        my_message_handler->message_cb(&msg);
    }
}

void cdb::MqttHandler::set_logger(cdb::Logger * l)
{
    logger = l;
}

void cdb::MqttHandler::set_message_handler(cdb::CallbackClass * handler)
{
    this->message_handler = handler;
    my_message_handler = handler;
}

void cdb::MqttHandler::message_cb(cdb::intra_msg_t * msg)
{
    const char * cmd_on     = "1";
    const char * cmd_off    = "0";
    const char * cmd_poll   = "";

    cdb::mqtt_handler_msg_t * mqtt = msg->content.mqtt;

    LOG_INFO(logger, "MQTT Handler: Received intra msg: " + mqtt->content + " for device " + mqtt->device);

    switch(mqtt->type){
        case CDB_INTRA_MQTT_TOPIC_PUBLISH:
            if (mqtt->content == "ON") {
                this->publish(cmnd_topic_map[mqtt->device].c_str(), sizeof(cmd_on), cmd_on);
            } else if (mqtt->content == "OFF") {
                this->publish(cmnd_topic_map[mqtt->device].c_str(), sizeof(cmd_on), cmd_off);
            } else if (mqtt->content == "POLL") {
                this->publish(cmnd_topic_map[mqtt->device].c_str(), sizeof(cmd_poll), cmd_poll);
            } else {
                LOG_WARN(logger, "MQTT Handler: Unexpected publish message: " + mqtt->content);
            }
            break;
        default:
            LOG_WARN(logger, "MQTT Handler: Unexpected message type: " + std::to_string(mqtt->type));
    }
}

void cdb::MqttHandler::mosq_logger(mosquitto *mosq, void *obj, int level, const char *str)
{
    std::string msg = "MQTT Handler: ";
    msg += str;

    switch(level){
        case MOSQ_LOG_DEBUG:
            LOG_DEBUG(logger, msg);
            break;
        case MOSQ_LOG_INFO:
        case MOSQ_LOG_NOTICE:
            LOG_INFO(logger, msg);
            break;
        case MOSQ_LOG_WARNING:
            LOG_WARN(logger, msg);
            break;
        case MOSQ_LOG_ERR:
        default:
            LOG_ERROR(logger, msg);
            break;
    }
}

void cdb::MqttHandler::init(cdb::Configurator * conf)
{
    this->conf = conf;
    cdb::MqttServer * broker = conf->inventory.mqtt_server();
    std::list<cdb::MqttDevice *> * l = conf->inventory.mqtt_dev_list();

    if (broker == NULL) {
        LOG_INFO(logger, "MQTT Handler: No MQTT broker configured, skipping MQTT setup");
        return;
    }

    if (l == NULL) {
        LOG_INFO(logger, "MQTT Handler: No MQTT devices configured, skipping MQTT setup");
        return;
    }

    mosquitto_lib_init();

    this->mosq = mosquitto_new(broker->client_id().c_str(), true, this);
    mosquitto_username_pw_set(this->mosq, broker->username().c_str(),
                                          broker->password().c_str());

    mosquitto_connect_callback_set(this->mosq, &on_connect);
    mosquitto_disconnect_callback_set(this->mosq, &on_disconnect);
    mosquitto_message_callback_set(this->mosq, &on_message);

    mosquitto_log_callback_set(this->mosq, &cdb::MqttHandler::mosq_logger);

    this->connect(broker);

    mosquitto_loop_start(this->mosq);
}

int cdb::MqttHandler::connect(cdb::MqttServer * conf)
{
    int res = mosquitto_connect(this->mosq, conf->addr().c_str(), conf->port(), 30);
    if (MOSQ_ERR_SUCCESS != res){
        LOG_ERROR(logger, "MQTT Handler: Connection to MQTT broker failed, error " + std::to_string(res));
        return CDB_NOK;
    }

    LOG_INFO(logger, "MQTT Handler: Connected to MQTT broker.");
    return CDB_OK;
}

void cdb::MqttHandler::add_device(cdb::MqttDevice * device)
{
    mosquitto_subscribe(this->mosq, NULL, device->stat().c_str(), 2);
}

void cdb::MqttHandler::publish(const char * topic, int payload_len, const void * payload)
{
    std::string msg = (char*) payload;
    LOG_INFO(logger, "MQTT Handler: Publish message " + msg);
    mosquitto_publish(this->mosq, NULL, topic, payload_len, payload, 2, false);
}
