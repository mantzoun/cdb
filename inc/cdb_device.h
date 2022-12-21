#ifndef CDB_DEVICE__H
#define CDB_DEVICE__H

#include <string>

/**
 * CDB_Device is the base class for the devices to
 * be handled by the applications
 */
class CDB_Device
{
    private:
        std::string _name;
        std::string _type;

    public:
        /**
         * Default constructor
         */
        CDB_Device(void)
        {
        }

        /**
         * Constructor for the basic parameters included
         * in the case class
         */
        CDB_Device(std::string name, std::string type)
        {
            this->set_name(name);
            this->set_type(type);
        }

        /**
         * Setter for the device name
         * The name must be unique for each device
         */
        void set_name(std::string name)
        {
            this->_name = name;
        }

        /**
         * Setter for the device type
         */
        void set_type(std::string type)
        {
            this->_type = type;
        }

        /**
         * Getter for the device name
         */
        std::string name(void)
        {
            return this->_name;
        }

        /**
         * Getter for the device type
         */
        std::string type(void)
        {
            return this->_type;
        }
};
#endif /* CDB_DEVICE__H */
