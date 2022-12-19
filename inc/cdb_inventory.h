#ifndef CDB_INVENTORY__H
#define CDB_INVENTORY__H

#include <string>
#include <list>

#include "cdb_mqtt_device.h"
#include "cdb_mqtt_server.h"

class CDB_Inventory {
    public:
        std::list<CDB_MQTT_Device *> * mqtt_devices = NULL ;
        CDB_MQTT_Server * mqtt_server = NULL;

        void mqtt_server_add(CDB_MQTT_Server * s) {
            this->mqtt_server = s;
        }

        void mqtt_device_add(CDB_MQTT_Device * d) {
            if (this->mqtt_devices == NULL){
                this->mqtt_devices = new std::list<CDB_MQTT_Device *>();
            }

            this->mqtt_devices->push_back(d);
        }

        std::list<CDB_MQTT_Device *> * mqtt_dev_list(void) {
            return this->mqtt_devices;
        }
};
#endif /* CDB_INVENTORY__H */
