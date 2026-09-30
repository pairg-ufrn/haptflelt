#include "SecretCommandSystem.hpp"

namespace Core {

    const SecretCommandSystem::Entry SecretCommandSystem::triggers[] = {
        { Touch::touch_t::ALL, secret_command_t::CALIBRATE_ZERO },
    };

    const size_t SecretCommandSystem::triggers_count =
        sizeof(SecretCommandSystem::triggers) / sizeof(SecretCommandSystem::Entry);

    secret_command_t SecretCommandSystem::detect(Touch::touch_t touch) {
        for (size_t i = 0; i < triggers_count; i++) {
            if (triggers[i].trigger == touch) return triggers[i].command;
        }

        return secret_command_t::NONE;
    }

}
