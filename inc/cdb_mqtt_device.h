#ifndef CDB_MQTT_DEVICE__H
#define CDB_MQTT_DEVICE__H

#include <string>

#include "cdb_device.h"

/**
 * CDB_MQTT_Device Class
 *
 * Derived from the CDB_Device class
 * with the addition of setters and getters
 * for mqtt status and command topics
 */
class CDB_MQTT_Device : public CDB_Device
{
    private:
        std::string _cmnd;
        std::string _stat;

    public:
        /**
         * Default constructor
         *
         * Set the device to mqtt by default
         */
        CDB_MQTT_Device(void)
        {
            this->set_type("mqtt");
        }

        /**
         * Constructor that accepts the additional mqqt parameters
         */
        CDB_MQTT_Device(std::string name, std::string cmnd, std::string stat)
        {
            this->set_name(name);
            this->set_cmnd(cmnd);
            this->set_stat(stat);
            this->set_type("mqtt");
        }

        /**
         * Setter for the mqtt command topic of the device
         */
        void set_cmnd(std::string cmnd)
        {
            this->_cmnd = cmnd;
        }

        /**
         * Setter for the mqtt status topic of the device
         */
        void set_stat(std::string stat)
        {
            this->_stat = stat;
        }

        /**
         * Getter for the mqtt command topic of the device
         */
        std::string cmnd(void)
        {
            return this->_cmnd;
        }

        /**
         * Getter for the mqtt status topic of the device
         */
        std::string stat(void)
        {
            return this->_stat;
        }
};
#endif /* CDB_MQTT_DEVICE__H */
