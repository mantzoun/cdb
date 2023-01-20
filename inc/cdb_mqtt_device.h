#ifndef CDB_MQTT_DEVICE__H
#define CDB_MQTT_DEVICE__H

#include <string>

#include "cdb_device.h"

namespace cdb {
    /**
     * @class MqttDevice
     *
     * @brief Derived from the Device class
     *        with the addition of setters and getters
     *        for mqtt status and command topics
     */
    class MqttDevice : public cdb::Device
    {
        private:
            std::string _cmnd;
            std::string _stat;

        public:
            /**
             * @brief Default constructor
             */
            MqttDevice(void)
            {
                this->set_type("mqtt");
            }

            /**
             * @brief Constructor that accepts the additional MQTT parameters
             *
             * @param name The device name
             * @param cmnd The command topic
             * @param stat The status topic
             */
            MqttDevice(std::string name, std::string cmnd, std::string stat)
            {
                this->set_name(name);
                this->set_cmnd(cmnd);
                this->set_stat(stat);
                this->set_type("mqtt");
            }

            /**
             * @brief Setter for the mqtt command topic of the device
             *
             * @param cmnd The command topic
             */
            void set_cmnd(std::string cmnd)
            {
                this->_cmnd = cmnd;
            }

            /**
             * @brief Setter for the mqtt status topic of the device
             *
             * @param stat The status topic
             */
            void set_stat(std::string stat)
            {
                this->_stat = stat;
            }

            /**
             * @brief Getter for the mqtt command topic of the device
             *
             * @return The command topic
             */
            std::string cmnd(void)
            {
                return this->_cmnd;
            }

            /**
             * @brief Getter for the mqtt status topic of the device
             *
             * @return The status topic
             */
            std::string stat(void)
            {
                return this->_stat;
            }
    };
}

#endif /* CDB_MQTT_DEVICE__H */
