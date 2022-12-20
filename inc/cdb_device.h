#ifndef CDB_DEVICE__H
#define CDB_DEVICE__H

#include <string>

class CDB_Device
{
    private:
        std::string _name;
        std::string _type;

    public:
        CDB_Device(void)
        {
        }

        CDB_Device(std::string name, std::string type)
        {
            this->set_name(name);
            this->set_type(type);
        }

        void set_name(std::string name)
        {
            this->_name = name;
        }

        void set_type(std::string type)
        {
            this->_type = type;
        }

        std::string name(void)
        {
            return this->_name;
        }

        std::string type(void)
        {
            return this->_type;
        }
};
#endif /* CDB_DEVICE__H */
