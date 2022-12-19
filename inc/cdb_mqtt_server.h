#ifndef CDB_MQTT_SERVER__H
#define CDB_MQTT_SERVER__H

#include <string>
#include <mosquitto.h>

class CDB_MQTT_Server
{
    private:
        std::string _addr;
        int _port;
        std::string _username;
        std::string _password;
        std::string _client_id;


        void (* _cb_on_connect)(mosquitto*, void*, int);
        void (* _cb_on_disconnect)(mosquitto*, void*, int);
        void (* _cb_on_message)(mosquitto*, void*, const mosquitto_message*);

    public:
        CDB_MQTT_Server(void)
        {
        }

        CDB_MQTT_Server(std::string addr, int port, std::string uname, std::string pass, std::string id)
        {
            this->_addr = addr;
            this->_port = port;
            this->_username = uname;
            this->_password = pass;
            this->_client_id = id;
        }

        std::string addr(void)
        {
            return this->_addr;
        }

        int port(void)
        {
            return this->_port;
        }

        std::string username(void)
        {
            return this->_username;
        }

        std::string password(void)
        {
            return this->_password;
        }

        std::string client_id(void)
        {
            return this->_client_id;
        }

        void set_cb_on_connect(void cb (mosquitto*, void*, int))
        {
            _cb_on_connect = cb;
        }

        void set_cb_on_disconnect(void cb (mosquitto*, void*, int))
        {
            _cb_on_disconnect = cb;
        }

        void set_cb_on_message(void cb (mosquitto*, void*, const mosquitto_message*))
        {
            _cb_on_message = cb;
        }

        void (* on_connect())(mosquitto*, void*, int)
        {
            return this->_cb_on_connect;
        }

        void (* on_disconnect())(mosquitto*, void*, int)
        {
            return this->_cb_on_disconnect;
        }

        void (* on_message())(mosquitto*, void*, const mosquitto_message*)
        {
            return this->_cb_on_message;
        }
};
#endif /* CDB_MQTT_SERVER__H */
