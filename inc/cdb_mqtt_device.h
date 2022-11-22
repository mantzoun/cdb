#ifndef CDB_MQTT_DEVICE__H
#define CDB_MQTT_DEVICE__H

#include <string>

#include "cdb_device.h"

#define CDB_MQTT_DEVICE_TYPE   0
#define CDB_MQTT_DEVICE_NAME   1
#define CDB_MQTT_DEVICE_CMND   2
#define CDB_MQTT_DEVICE_STAT   3
#define CDB_MQTT_DEVICE_PARAMS 4

class CDB_MQTT_Device : public CDB_Device
{
    private:
        std::string _cmnd;
        std::string _stat;

    public:
        CDB_MQTT_Device(void)
        {
        }

        CDB_MQTT_Device(std::string name, std::string cmnd, std::string stat)
        {
            this->set_name(name);
            this->set_cmnd(cmnd);
            this->set_stat(stat);
            this->set_type("mqtt");
        }

        void set_cmnd(std::string cmnd)
        {
            this->_cmnd = cmnd;
        }

        void set_stat(std::string stat)
        {
            this->_stat = stat;
        }

        std::string cmnd(void)
        {
            return this->_cmnd;
        }

        std::string stat(void)
        {
            return this->_stat;
        }
};
#endif /* CDB_MQTT_DEVICE__H */
