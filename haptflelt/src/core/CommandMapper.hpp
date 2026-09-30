/**
 * @file CommandMapper.hpp
 * @brief CommandMapper class definition
 * @author Franklin
 */
#ifndef COMMANDMAPPER_HPP
#define COMMANDMAPPER_HPP

#include "extern.hpp"
#include "../movement/MovementSystem.hpp"
#include "../touch/TouchSystem.hpp"
#include "../bluetooth/BluetoothSender.hpp"

namespace Core {

    /**
     * @class CommandMapper
     * @brief Maps movements and touches to the commands sent over Bluetooth.
     */
    class CommandMapper {

        private:
            /**
             * @struct MovementEntry
             * @brief Associates a movement with a Bluetooth command.
             */
            struct MovementEntry {
                Movement::movement_t movement; ///< Detected movement.
                Bluetooth::sent_command_t command; ///< Respective Bluetooth command.
            };

            /**
             * @struct TouchEntry
             * @brief Associates a touch combination with a Bluetooth command.
             */
            struct TouchEntry {
                Touch::touch_t touch; ///< Detected touch combination.
                Bluetooth::sent_command_t command; ///< Respective Bluetooth command.
            };

            static const MovementEntry movement_table[]; ///< Movement to command table.
            static const size_t movement_table_count; ///< Number of entries in the movement table.

            static const TouchEntry touch_table[]; ///< Touch to command table.
            static const size_t touch_table_count; ///< Number of entries in the touch table.

        public:
            /**
             * @brief Translates a movement into the respective Bluetooth command.
             * @param movement Detected movement.
             * @return Respective command, or `sent_command_t::NONE` if there is no entry for it.
             */
            static Bluetooth::sent_command_t toCommand(Movement::movement_t movement);

            /**
             * @brief Translates a touch combination into the respective Bluetooth command.
             * @param touch Detected touch combination.
             * @return Respective command, or `sent_command_t::NONE` if there is no entry for it.
             */
            static Bluetooth::sent_command_t toCommand(Touch::touch_t touch);

    };

}

#endif
