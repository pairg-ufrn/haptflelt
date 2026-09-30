#include "BluetoothReceiver.hpp"

namespace Bluetooth {

    feedback_direction_t BluetoothReceiver::messageToCommand(String _message) {            
        if (_message == "up") return feedback_direction_t::FRONT;
        else if (_message == "right") return feedback_direction_t::RIGHT;
        else if (_message == "down") return feedback_direction_t::BACK;
        else if (_message == "left") return feedback_direction_t::LEFT;
        else return feedback_direction_t::NONE;
    }

    feedback_direction_t BluetoothReceiver::read_message() {
        // Messages from the PC are terminated with '\n'.
        String message = (*serial)->readStringUntil('\n');

        message.trim();

        // Handshake used by the PC to confirm the connected port is the HaptFlelt.
        if (message == "PING") {
            (*serial)->println("PONG");
            return feedback_direction_t::NONE;
        }

        return this->messageToCommand(message);
    }

}