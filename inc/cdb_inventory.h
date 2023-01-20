#ifndef CDB_INVENTORY__H
#define CDB_INVENTORY__H

#include <string>
#include <list>

#include "cdb_mqtt_device.h"
#include "cdb_mqtt_server.h"

namespace cdb {
    /**
     * @class Inventory
     *
     * @brief The Inventory class holds all the entity objects created
     *        by the application, such as servers devices or sensors
     */
    class Inventory {
        private:
            std::list<cdb::MqttDevice *> * mqtt_devices = NULL;
            cdb::MqttServer * _mqtt_server = NULL;
        public:
            /**
             * @brief Add an MQTT Broker object. Only one instance is
             *        currently supported
             *
             * @param s The object containing the Broker information
             */
            void mqtt_server_add(cdb::MqttServer * s) {
                this->_mqtt_server = s;
            }

            /**
             * @brief Getter function for the MQQT broker object
             *
             * @return The Broker object
             */
            cdb::MqttServer * mqtt_server(void) {
                return this->_mqtt_server;
            }

            /**
             * @brief Add an MQQT device object. All devices are currently
             *        associated with the single MQTT broker instance.
             *
             * @param d The device information
             */
            void mqtt_device_add(cdb::MqttDevice * d) {
                if (this->mqtt_devices == NULL){
                    this->mqtt_devices = new std::list<cdb::MqttDevice *>();
                }

                this->mqtt_devices->push_back(d);
            }

            /**
             * @brief Getter function for the list of MQQT device objects
             *
             * @return the device list
             */
            std::list<cdb::MqttDevice *> * mqtt_dev_list(void) {
                return this->mqtt_devices;
            }
    };
}

#endif /* CDB_INVENTORY__H */
