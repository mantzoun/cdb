#ifndef CDB_INVENTORY__H
#define CDB_INVENTORY__H

#include <string>
#include <list>

#include "cdb_mqtt_device.h"
#include "cdb_mqtt_server.h"

/**
 * CDB_Inventory Class holds the all entity objects created
 * by the application, such as servers devices or sensors
 */
class CDB_Inventory {
    private:
        std::list<CDB_MQTT_Device *> * mqtt_devices = NULL;
        CDB_MQTT_Server * _mqtt_server = NULL;
    public:
        /**
         * Add an MQTT Broker object. Only one instance is
         * currently supported
         */
        void mqtt_server_add(CDB_MQTT_Server * s) {
            this->_mqtt_server = s;
        }

        /**
         * Getter function for the MQQT broker object
         */
        CDB_MQTT_Server * mqtt_server(void) {
            return this->_mqtt_server;
        }

        /**
         * Add an MQQT device object. All devices are currently
         * associated with the single MQTT broker instance.
         */
        void mqtt_device_add(CDB_MQTT_Device * d) {
            if (this->mqtt_devices == NULL){
                this->mqtt_devices = new std::list<CDB_MQTT_Device *>();
            }

            this->mqtt_devices->push_back(d);
        }

        /**
         * Getter function for the list of MQQT device objects
         */
        std::list<CDB_MQTT_Device *> * mqtt_dev_list(void) {
            return this->mqtt_devices;
        }
};
#endif /* CDB_INVENTORY__H */
