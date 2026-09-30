#include "TouchSystem.hpp"

namespace Touch {

    touch_t TouchSystem::detectTouch() {
        
        auto R = this->rightButton.touched();
        auto L = this->leftButton.touched();
        auto FR = this->frontRightButton.touched();
        auto FL = this->frontLeftButton.touched();

        auto combination = R + L + FR + FL; 

        if (combination == 0) return touch_t::NONE;

        if (combination == 4) return touch_t::ALL;

        if (combination == 2) {
            if (R && L) return touch_t::L_R;
            else if (R && FL) return touch_t::R_FL;
            else if (L && FR) return touch_t::L_FR;
            else if (FL && FR) return touch_t::FR_FL;
            else return touch_t::NONE;
        } else if (combination == 1) {
            if (R) return touch_t::RIGHT;
            else if (L) return touch_t::LEFT;
            else if (FR) return touch_t::FRONT_RIGHT;
            else if (FL) return touch_t::FRONT_LEFT;
        }

        return touch_t::NONE;
    }

}