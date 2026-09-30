/**
 * @file BluetoothReceiver.hpp
 * @brief BluetoothReceiver class definition
 * @author Franklin
 */
#ifndef BLUETOOTH_RECEIVER_HPP
#define BLUETOOTH_RECEIVER_HPP

#include "extern.hpp"
#include "vibration/VibrationSystem.hpp"

using namespace Vibration;

namespace Bluetooth {

    /**
     * @class BluetoothReceiver
     * @brief Reads and interprets the messages received from the PC.
     */
    class BluetoothReceiver {

        private:
            BluetoothSerial** serial; ///< Bluetooth serial.

            /**
             * @brief Converts a received message into a haptic feedback direction.
             * @param _message Received message.
             * @return Respective haptic feedback direction.
             */
            feedback_direction_t messageToCommand(String _message);
        public:

            /**
             * @brief Constructor with parameters.
             * @param _serial Address of bluetooth serial.
             */
            BluetoothReceiver(BluetoothSerial** _serial) : serial(_serial) {}

            /**
             * @brief Reads a message received from the PC.
             * @return Respective haptic feedback direction.
             */
            feedback_direction_t read_message();

    };

}

#endif