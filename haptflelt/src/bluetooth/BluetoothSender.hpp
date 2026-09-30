/**
 * @file BluetoothSender.hpp
 * @brief BluetoothSender class definition
 * @author Franklin
 */
#ifndef BLUETOOTH_SENDER_HPP
#define BLUETOOTH_SENDER_HPP

#include "extern.hpp"

namespace Bluetooth {

    /**
     * @enum sent_command_t
     * @brief Represents the commands sent to the PC.
     */
    enum class sent_command_t {
        NONE, // No command.

        TOUCH_LEFT, TOUCH_RIGHT, TOUCH_FRONT_RIGHT, TOUCH_FRONT_LEFT, // Single touches.

        TOUCH_L_R, TOUCH_L_FR, TOUCH_R_FL, TOUCH_FR_FL, // Combined touches.

        MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, MOVE_FRONT, MOVE_BACK // Movement types.
    };

    /**
     * @class BluetoothSender
     * @brief Sends commands to the PC.
     */
    class BluetoothSender {

        private:
            BluetoothSerial** serial; ///< Bluetooth serial.

            /**
             * @brief Converts a command into the text sent to the PC.
             * @param _command Command to be converted.
             * @return Respective text.
             */
            String commandToText(sent_command_t _command);

        public:

            /**
             * @brief Constructor with parameters.
             * @param _serial Address of bluetooth serial.
             */
            BluetoothSender(BluetoothSerial** _serial) : serial(_serial) {}
            
            /**
             * @brief Sends a command to the PC.
             * @param _command Command to be sent.
             */
            void send_message(sent_command_t _command);

    };

}

#endif