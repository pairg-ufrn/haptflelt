#include "TouchButton.hpp"

namespace Touch {

    bool TouchButton::touched() {
        if (touchRead(this->PIN) < this->threshold) return true;
        return false;
    }

    void TouchButton::setPin(unsigned short _PIN) {
        this->PIN = _PIN;
    }

    void TouchButton::setThreshold(float _threshold) {
        this->threshold = _threshold;
    }
}