#include "BluetoothSender.hpp"

namespace Bluetooth {

    String BluetoothSender::commandToText(sent_command_t _command) {
        if (_command == sent_command_t::TOUCH_LEFT) return "TOUCH_LEFT";
        else if (_command == sent_command_t::TOUCH_RIGHT) return "TOUCH_RIGHT";
        else if (_command == sent_command_t::TOUCH_FRONT_RIGHT) return "TOUCH_FRONT_RIGHT";
        else if (_command == sent_command_t::TOUCH_FRONT_LEFT) return "TOUCH_FRONT_LEFT";
        else if (_command == sent_command_t::TOUCH_L_R) return "TOUCH_L_R";
        else if (_command == sent_command_t::TOUCH_L_FR) return "TOUCH_L_FR";
        else if (_command == sent_command_t::TOUCH_R_FL) return "TOUCH_R_FL";
        else if (_command == sent_command_t::TOUCH_FR_FL) return "TOUCH_FR_FL";
        else if (_command == sent_command_t::MOVE_UP) return "MOVE_UP";
        else if (_command == sent_command_t::MOVE_DOWN) return "MOVE_DOWN";
        else if (_command == sent_command_t::MOVE_LEFT) return "MOVE_LEFT";
        else if (_command == sent_command_t::MOVE_RIGHT) return "MOVE_RIGHT";
        else if (_command == sent_command_t::MOVE_FRONT) return "MOVE_FRONT";
        else if (_command == sent_command_t::MOVE_BACK) return "MOVE_BACK";
        return "NONE";
    }

    void BluetoothSender::send_message(sent_command_t _command) {
        String message = this->commandToText(_command);
        Serial.println(message);
        (*serial)->println(message);
    }

}