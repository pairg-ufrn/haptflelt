/**
 * @file SecretCommandSystem.hpp
 * @brief SecretCommandSystem class definition
 * @author Franklin
 */
#ifndef SECRETCOMMANDSYSTEM_HPP
#define SECRETCOMMANDSYSTEM_HPP

#include "extern.hpp"
#include "../touch/TouchSystem.hpp"

namespace Core {

    /**
     * @enum secret_command_t
     * @brief Represents the device actions that are not sent to the PC.
     */
    enum class secret_command_t {
        NONE, // No command.

        CALIBRATE_ZERO // Recalibrates the resting position.
    };

    /**
     * @class SecretCommandSystem
     * @brief Maps touch combinations to secret commands.
     */
    class SecretCommandSystem {

        private:
            /**
             * @struct Entry
             * @brief Associates a touch combination with a secret command.
             */
            struct Entry {
                Touch::touch_t trigger; ///< Touch combination that triggers the command.
                secret_command_t command; ///< Respective secret command.
            };

            static const Entry triggers[]; ///< Touch combination to secret command table.
            static const size_t triggers_count; ///< Number of entries in the table.

        public:

            /**
             * @brief Default constructor.
             */
            SecretCommandSystem() {}

            /**
             * @brief Checks if a touch combination triggers a secret command.
             * @param touch Detected touch combination.
             * @return Respective secret command, or `secret_command_t::NONE`.
             */
            secret_command_t detect(Touch::touch_t touch);

    };

}

#endif
