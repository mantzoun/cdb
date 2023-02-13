#include <map>
#include "string"
#include "cstring"
#include "iostream"

#include "cdb_mqtt_handler.h"
#include "cdb_discord_bot.h"

cdb::Logger * cdb::MqttHandler::logger;
cdb::CallbackClass * my_bot;

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
        cdb::callback_msg msg = { CDB_MSG_DISC_MQTT_DEV_ADD,
                                 (*it)->name()};
        stat_topic_map[(*it)->stat()] = (*it)->name();
        cmnd_topic_map[(*it)->name()] = (*it)->cmnd();
        this->bot->message_cb(&msg);
    }
}

void cdb::MqttHandler::on_disconnect_cb(void)
{
    LOG_WARN(logger, "MQTT Handler: On Disconnect");
}

void cdb::MqttHandler::on_message_cb(const mosquitto_message *message)
{
    cdb::msg_t type = cdb::CDB_MSG_MAX;
    std::string msg = (char*) message->payload;
    LOG_INFO(logger, "MQTT Handler: Received message " + msg + " for device " + stat_topic_map[message->topic]);

    if (stat_topic_map.find(message->topic) != stat_topic_map.end()){
        if (strcmp((char*) message->payload, "ON") == 0){
            type = cdb::CDB_MSG_DISC_MQTT_DEV_STATUS_ON;
        } else if (strcmp((char*) message->payload, "OFF") == 0){
            type = cdb::CDB_MSG_DISC_MQTT_DEV_STATUS_OFF;
        } else {
            LOG_WARN(logger, "MQTT Handler: Could not determine message type");
            return;
        }

        cdb::callback_msg  msg = {type, stat_topic_map[message->topic]};
        my_bot->message_cb(&msg);
    }
}

void cdb::MqttHandler::set_logger(cdb::Logger * l)
{
    logger = l;
}

void cdb::MqttHandler::set_discord_bot(cdb::CallbackClass * disc_bot)
{
    this->bot = disc_bot;
    my_bot = disc_bot;
}

void cdb::MqttHandler::message_cb(cdb::callback_msg * msg)
{
    const char * cmd_on   = "1";
    const char * cmd_off  = "0";
    const char * cmd_poll = "";

    LOG_INFO(logger, "MQTT Handler: Received bot msg: " + msg->content);

    switch(msg->type){
        case CDB_MSG_MQTT_HANDLER_TURN_DEVICE_ON:
            this->publish(cmnd_topic_map[msg->content].c_str(), sizeof(cmd_on), cmd_on);
            break;
        case CDB_MSG_MQTT_HANDLER_TURN_DEVICE_OFF:
            this->publish(cmnd_topic_map[msg->content].c_str(), sizeof(cmd_off), cmd_off);
            break;
        case CDB_MSG_MQTT_HANDLER_POLL_DEVICE_STATUS:
            this->publish(cmnd_topic_map[msg->content].c_str(), sizeof(cmd_poll), cmd_poll);
            break;
        default:
            LOG_WARN(logger, "MQTT Handler: Unexpected message type: " + std::to_string(msg->type));
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
