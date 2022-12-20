#ifndef CDB_MQTT_SERVER__H
#define CDB_MQTT_SERVER__H

#include <string>
#include <mosquitto.h>

/** Class to hold the connection information for
 * one MQTT broker
 */
class CDB_MQTT_Server
{
    private:
        std::string _addr;
        int _port;
        std::string _username;
        std::string _password;
        std::string _client_id;

    public:
        /** Default constructor
         */
        CDB_MQTT_Server(void)
        {
        }

        /** Constructor that provides all connection information
         *
         * @param   addr    The MQTT broker address
         * @param   port    The MQTT broker listening port
         * @param   uname   The username used for client authentication
         * @param   pass    The password used for client authentication
         * @param   id      The identifier the MQQT broker will use for this client
         */
        CDB_MQTT_Server(std::string addr, int port, std::string uname, std::string pass, std::string id)
        {
            this->_addr = addr;
            this->_port = port;
            this->_username = uname;
            this->_password = pass;
            this->_client_id = id;
        }

        /** Getter function for the MQTT broker address
         */
        std::string addr(void)
        {
            return this->_addr;
        }

        /** Getter function for the MQTT broker listening port
         */
        int port(void)
        {
            return this->_port;
        }

        /** Getter function for the client username
         */
        std::string username(void)
        {
            return this->_username;
        }

        /** Getter function for the client password
         */
        std::string password(void)
        {
            return this->_password;
        }

        /** Getter function for the client ID
         */
        std::string client_id(void)
        {
            return this->_client_id;
        }
};
#endif /* CDB_MQTT_SERVER__H */
