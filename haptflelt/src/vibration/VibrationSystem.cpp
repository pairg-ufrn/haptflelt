#include "VibrationSystem.hpp"

namespace Vibration {

    std::vector<unsigned short> VibrationSystem::directionToIndex(feedback_direction_t _direction) {
        std::vector<unsigned short> indexes;
        
        if (_direction == feedback_direction_t::FRONT) indexes.push_back(0);
        else if (_direction == feedback_direction_t::RIGHT) indexes.push_back(1);
        else if (_direction == feedback_direction_t::BACK) indexes.push_back(2);
        else if (_direction == feedback_direction_t::LEFT) indexes.push_back(3);

        else if (_direction == feedback_direction_t::FRONT_RIGHT) {
            indexes.push_back(0);
            indexes.push_back(1);
        }
        else if (_direction == feedback_direction_t::BACK_RIGHT) {
            indexes.push_back(1);
            indexes.push_back(2);
        }
        else if (_direction == feedback_direction_t::BACK_LEFT) {
            indexes.push_back(2);
            indexes.push_back(3);
        }
        else if (_direction == feedback_direction_t::FRONT_LEFT) {
            indexes.push_back(3);
            indexes.push_back(0);
        }

        return indexes;
    }

    void VibrationSystem::hapticFeedback(feedback_direction_t _direction) {
        auto indexes = this->directionToIndex(_direction);

        for (auto i : indexes) this->vibracalls->turnOn(i);
    }

    void VibrationSystem::stopHapticFeedback(feedback_direction_t _direction) {
        auto indexes = this->directionToIndex(_direction);

        for (auto i : indexes) this->vibracalls->turnOff(i);
    }

    void VibrationSystem::hapticFeedback(feedback_direction_t _direction, uint32_t milliseconds) {
        auto indexes = this->directionToIndex(_direction);
        
        for (auto i : indexes) this->vibracalls->turnOn(i);
        delay(milliseconds);
        for (auto i : indexes) this->vibracalls->turnOff(i);
    }

    void VibrationSystem::stopAll() {
        unsigned short indexes[] = {0, 1, 2, 3};
        this->vibracalls->turnOff(indexes, 4);
    }

}