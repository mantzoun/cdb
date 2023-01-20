#ifndef CDB_DEVICE__H
#define CDB_DEVICE__H

#include <string>

namespace cdb {
    /**
     * @class Device
     *
     * @brief Device is the base class for the devices to
     *        be handled by the applications
     */
    class Device
    {
        private:
            std::string _name;
            std::string _type;

        public:
            /**
             * @brief Default constructor
             */
            Device(void)
            {
            }

            /**
             * @brief Constructor for the basic parameters included
             *        in the base class
             */
            Device(std::string name, std::string type)
            {
                this->set_name(name);
                this->set_type(type);
            }

            /**
             * @brief Setter for the device name
             *        The name must be unique for each device
             *
             * @param name The device name
             */
            void set_name(std::string name)
            {
                this->_name = name;
            }

            /**
             * @brief Setter for the device type
             *
             * @param type The device type
             */
            void set_type(std::string type)
            {
                this->_type = type;
            }

            /**
             * @brief Getter for the device name
             *
             * @return The device name
             */
            std::string name(void)
            {
                return this->_name;
            }

            /**
             * @brief Getter for the device type
             *
             * @return The device type
             */
            std::string type(void)
            {
                return this->_type;
            }
    };
}

#endif /* CDB_DEVICE__H */
