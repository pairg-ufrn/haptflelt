#include "CommandMapper.hpp"

namespace Core {

    const CommandMapper::MovementEntry CommandMapper::movement_table[] = {
        { Movement::movement_t::FRONT, Bluetooth::sent_command_t::MOVE_FRONT },
        { Movement::movement_t::BACK,  Bluetooth::sent_command_t::MOVE_BACK  },
        { Movement::movement_t::UP,    Bluetooth::sent_command_t::MOVE_UP    },
        { Movement::movement_t::DOWN,  Bluetooth::sent_command_t::MOVE_DOWN  },
        { Movement::movement_t::LEFT,  Bluetooth::sent_command_t::MOVE_LEFT  },
        { Movement::movement_t::RIGHT, Bluetooth::sent_command_t::MOVE_RIGHT },
    };

    const size_t CommandMapper::movement_table_count =
        sizeof(CommandMapper::movement_table) / sizeof(CommandMapper::MovementEntry);

    const CommandMapper::TouchEntry CommandMapper::touch_table[] = {
        { Touch::touch_t::RIGHT,       Bluetooth::sent_command_t::TOUCH_RIGHT       },
        { Touch::touch_t::LEFT,        Bluetooth::sent_command_t::TOUCH_LEFT        },
        { Touch::touch_t::FRONT_RIGHT, Bluetooth::sent_command_t::TOUCH_FRONT_RIGHT },
        { Touch::touch_t::FRONT_LEFT,  Bluetooth::sent_command_t::TOUCH_FRONT_LEFT  },
        { Touch::touch_t::L_R,         Bluetooth::sent_command_t::TOUCH_L_R         },
        { Touch::touch_t::L_FR,        Bluetooth::sent_command_t::TOUCH_L_FR        },
        { Touch::touch_t::R_FL,        Bluetooth::sent_command_t::TOUCH_R_FL        },
        { Touch::touch_t::FR_FL,       Bluetooth::sent_command_t::TOUCH_FR_FL       },
    };

    const size_t CommandMapper::touch_table_count =
        sizeof(CommandMapper::touch_table) / sizeof(CommandMapper::TouchEntry);

    Bluetooth::sent_command_t CommandMapper::toCommand(Movement::movement_t movement) {
        for (size_t i = 0; i < movement_table_count; i++) {
            if (movement_table[i].movement == movement) return movement_table[i].command;
        }

        return Bluetooth::sent_command_t::NONE;
    }

    Bluetooth::sent_command_t CommandMapper::toCommand(Touch::touch_t touch) {
        for (size_t i = 0; i < touch_table_count; i++) {
            if (touch_table[i].touch == touch) return touch_table[i].command;
        }

        return Bluetooth::sent_command_t::NONE;
    }

}
