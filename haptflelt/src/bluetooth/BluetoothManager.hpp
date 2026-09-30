/**
 * @file BluetoothManager.hpp
 * @brief BluetoothManager class definition
 * @author Franklin
 */
#ifndef BLUETOOTH_MANAGER_HPP
#define BLUETOOTH_MANAGER_HPP

#include "extern.hpp"
#include "BluetoothReceiver.hpp"
#include "BluetoothSender.hpp"

namespace Bluetooth {

    /**
     * @class BluetoothManager
     * @brief Manages the Bluetooth connection and the communication with the PC.
     */
    class BluetoothManager {

        private:
            BluetoothSerial* serialBT; ///< Bluetooth serial.
            bool connected; ///< `true` if Bluetooth is connected, `false` otherwise.

            BluetoothReceiver receiver; ///< Receives and interprets messages from the PC.
            BluetoothSender sender; ///< Sends commands to the PC.

            String bt_name; ///< Bluetooth device name.

            /**
             * @brief Starts the Bluetooth serial with the given name.
             * @param bt_name Bluetooth device name.
             */
            void connect(String bt_name);

            /**
             * @brief Ends the Bluetooth serial and releases it.
             */
            void disconnect();

        public:

            /**
             * @brief Constructor with parameters.
             * @param _bt_name Bluetooth device name.
             */
            BluetoothManager(String _bt_name) :
                serialBT(nullptr),
                connected(false),
                receiver(&this->serialBT),
                sender(&this->serialBT),
                bt_name(_bt_name) {
            }

            /**
             * @brief Class destructor.
             */
            ~BluetoothManager() {}

            /**
             * @brief Initializes the Bluetooth system.
             */
            void initSystem();

            /**
             * @brief Shuts down the Bluetooth system.
             */
            void shutdownSystem();

            /**
             * @brief Checks if Bluetooth is connected.
             * @return `true` if Bluetooth is connected, `false` otherwise.
             */
            bool isConnected();

            /**
             * @brief Checks if the communication is ready.
             * @return `true` if the communication is ready, `false` otherwise.
             */
            bool communicationReady();

            /**
             * @brief Checks if there is received data to be read.
             * @return `true` if there is available data, `false` otherwise.
             */
            bool hasAvailableData();

            /**
             * @brief Reads all the received messages and keeps only the most recent one.
             * @return Most recent haptic feedback direction received.
             */
            feedback_direction_t receive();

            /**
             * @brief Sends a command to the PC.
             * @param message Command to be sent.
             */
            void send(sent_command_t message);

            /**
             * @brief Checks if there is a paired device.
             * @return `true` if there is a paired device, `false` otherwise.
             */
            bool hasPairedDevice();

    };

}

#endif