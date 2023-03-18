#ifndef CDB_INTRA_MESSAGING__H
#define CDB_INTRA_MESSAGING__H

namespace cdb {
    /**
     * @brief Enum of all cdb intra message senders
     *
     */
    enum sender_t
    {
        CDB_MAIN,
        CDB_DISCORD_BOT,
        CDB_MQTT_HANDLER,
        CDB_IO_HANDLER,
    };

    /**
     * @class discord_bot_msg_t
     *
     * @brief the message type to be used for communication with the
     *        discord bot module
     */
    struct discord_bot_msg_t
    {
        cdb_intra_msg_bot_t     type;       /*!< Discord bot message type */
        std::string             device;     /*!< Device related to message */
        std::string             channel;    /*!< Channel related to message */
        std::string             guild;      /*!< Guild related to message */
        std::string             content;    /*!< Additional information */
    };

    /**
     * @class mqtt_handler_msg_t
     *
     * @brief the message type to be used for communication with the
     *        MQTT handler module
     */
    struct mqtt_handler_msg_t
    {
        cdb_intra_msg_mqtt_t    type;       /*!< MQTT message type */
        std::string             device;     /*!< MQTT device related to topic */
        std::string             content;    /*!< Additional information */
    };

    /**
     * @class io_msg_t
     *
     * @brief the message type to be used for communication with the
     *        I/O module
     *
     *        command           | field1        | field2           | field3
     *        ------------------+---------------+------------------+--------------
     *        POST_FILE_DISCORD | path to file  | discord channel  | guild ID
     *        POST_TEXT_DISCORD | text message  | discord channel  | guild ID
     *        ADD_MQTT_CONFIG   | config string |                  |
     */
    struct io_msg_t
    {
        std::string command;
        std::string field1;
        std::string field2;
        std::string field3;
    };

    /**
     * @class intra_msg_content_t
     *
     * @brief the message content, dependign on the module it is being sent to/from
     */
    union intra_msg_content_t
    {
        discord_bot_msg_t   * bot;      /*!< pointer to Discord bot message */
        mqtt_handler_msg_t  * mqtt;     /*!< pointer to MQTT message */
        io_msg_t            * io;       /*!< pointer to IO message */
    };

    /**
     * @class callback_msg
     *
     * @brief the message type to be user for inter-module communication
     */
    struct intra_msg_t
    {
        cdb::sender_t           sender;         /*!< Message sender */
        intra_msg_content_t     content;
    };
}
#endif /* CDB_INTRA_MESSAGING__H */
