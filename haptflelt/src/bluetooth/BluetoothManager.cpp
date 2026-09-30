#include "BluetoothManager.hpp"

namespace Bluetooth {

    void BluetoothManager::initSystem() {
        this->connect(this->bt_name);
    }

    void BluetoothManager::disconnect() {
        if (this->isConnected()) {
            this->serialBT->end();
            this->connected = false;
        }

        delete this->serialBT;
    }

    void BluetoothManager::shutdownSystem() {
        this->disconnect();
    }

    void BluetoothManager::connect(String bt_name) {
        if (this->isConnected()) this->disconnect();
        
        this->serialBT = new BluetoothSerial();
        this->connected = this->serialBT->begin(bt_name);

        // Avoids waiting the default 1s timeout if a message arrives without '\n'.
        this->serialBT->setTimeout(50);
    }

    bool BluetoothManager::isConnected() {
        return this->connected;
    }

    bool BluetoothManager::hasAvailableData() {
        return this->serialBT->available();
    }

    feedback_direction_t BluetoothManager::receive() {
        // Drains the whole queue and keeps only the most recent command, since
        // queued messages are already obsolete and would extend the vibration.
        feedback_direction_t latest = feedback_direction_t::NONE;

        while (this->hasAvailableData()) {
            latest = this->receiver.read_message();
        }

        return latest;
    }

    void BluetoothManager::send(sent_command_t message) {
        this->sender.send_message(message);
    }

    bool BluetoothManager::hasPairedDevice() {
        return this->serialBT->hasClient();
    }

}