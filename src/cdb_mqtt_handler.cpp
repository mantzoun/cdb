#include <map>
#include "string"
#include "cstring"
#include "iostream"

#include "cdb_mqtt_handler.h"

CDB_Logger * CDB_MQTT_Handler::logger;
CDB_Callback_Class * my_bot;

static std::map<std::string, std::string> stat_topic_map;
static std::map<std::string, std::string> cmnd_topic_map;

static void on_connect(mosquitto * m, void *obj, int res)
{
    //printf("mqtt on connect res %d\n", res);
}

static void on_disconnect(mosquitto * m, void *obj, int res)
{
    //printf("mqtt on disconnect res %d\n", res);
}

static void on_message(mosquitto * m, void *obj, const mosquitto_message *message)
{
    cdb_msg_t type = CDB_MSG_MAX;
    printf("mqtt message %s\n", (char*) message->payload);
    if (stat_topic_map.find(message->topic) != stat_topic_map.end()){
        if (strcmp((char*) message->payload, "ON") == 0){
            type = CDB_MSG_DISC_MQTT_DEV_STATUS_ON;
        } else if (strcmp((char*) message->payload, "OFF") == 0){
            type = CDB_MSG_DISC_MQTT_DEV_STATUS_OFF;
        }

        std::cout << "device " + stat_topic_map[message->topic] + "\n";
        cdb_callback_msg  msg = {type, stat_topic_map[message->topic]};
        my_bot->message_cb(&msg);
    }
}

CDB_MQTT_Handler::CDB_MQTT_Handler(void)
{
}

void CDB_MQTT_Handler::set_logger(CDB_Logger * l)
{
    logger = l;
}

void CDB_MQTT_Handler::set_discord_bot(CDB_Callback_Class * disc_bot)
{
    this->bot = disc_bot;
    my_bot = disc_bot;
    std::cout << "bot is now " << my_bot << "\n";
}

void CDB_MQTT_Handler::message_cb(cdb_callback_msg * msg)
{
    const char * cmd_on   = "1";
    const char * cmd_off  = "0";
    const char * cmd_poll = "";

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
            this->logger->warn("Unexpected message type: " + std::to_string(msg->type));
    }
}

void CDB_MQTT_Handler::mosq_logger(mosquitto *mosq, void *obj, int level, const char *str)
{
    std::string msg = str;
    logger->debug("MQTT Handler: " + msg);
}

void CDB_MQTT_Handler::init(CDB_Configurator * conf)
{
    CDB_MQTT_Server * broker = conf->inventory.mqtt_server();

    if (broker == NULL) {
        logger->info("No MQTT broker configured, skipping MQTT setup");
        return;
    }

    std::list<CDB_MQTT_Device *> * l = conf->inventory.mqtt_dev_list();

    if (l == NULL) {
        logger->info("No MQTT devices configured, skipping MQTT setup");
    }

//    if (CDB_MQTT_OK != res){
//        logger.error("Connection to broker failed, error " + std::to_string(res));
//    } else {
//        logger.info("Connected to broker.");
//    }

    mosquitto_lib_init();

    this->mosq = mosquitto_new(broker->client_id().c_str(), true, NULL);
    mosquitto_username_pw_set(this->mosq, broker->username().c_str(),
                                          broker->password().c_str());

    mosquitto_connect_callback_set(this->mosq, &on_connect);
    mosquitto_disconnect_callback_set(this->mosq, &on_disconnect);
    mosquitto_message_callback_set(this->mosq, &on_message);

    mosquitto_log_callback_set(this->mosq, &CDB_MQTT_Handler::mosq_logger);

    this->connect(broker);

    mosquitto_loop_start(this->mosq);

    std::list<CDB_MQTT_Device *>::iterator it;
    for (it = l->begin(); it != l->end(); ++it){
        logger->info("Adding device " + (*it)->name());
        this->add_device(*it);
        cdb_callback_msg msg = { CDB_MSG_DISC_MQTT_DEV_ADD,
                                 (*it)->name()};
        stat_topic_map[(*it)->stat()] = (*it)->name();
        cmnd_topic_map[(*it)->name()] = (*it)->cmnd();
        this->bot->message_cb(&msg);
    }
}

int CDB_MQTT_Handler::connect(CDB_MQTT_Server * conf)
{
    int res = mosquitto_connect(this->mosq, conf->addr().c_str(), conf->port(), 30);
    if (MOSQ_ERR_SUCCESS != res){
        logger->error("Connection to MQTT broker failed, error " + std::to_string(res));
        return CDB_NOK;
    }

    logger->info("Connected to MQTT broker.");
    return CDB_OK;
}

void CDB_MQTT_Handler::add_device(CDB_MQTT_Device * device)
{
    mosquitto_subscribe(this->mosq, NULL, device->stat().c_str(), 1);
}

void CDB_MQTT_Handler::publish(const char * topic, int payload_len, const void * payload)
{
    mosquitto_publish(this->mosq, NULL, topic, payload_len, payload, 1, false);
}
