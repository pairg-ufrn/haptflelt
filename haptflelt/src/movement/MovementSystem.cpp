#include "MovementSystem.hpp"

using namespace std;

namespace Movement {

    bool MovementSystem::initSystem() {
        if (!this->mpu.begin()) return false;

        this->calibrate();
        return true;
    }

    void MovementSystem::calibrate(uint16_t samples) {
        double sum_x = 0.0, sum_y = 0.0, sum_z = 0.0;

        for (uint16_t i = 0; i < samples; i++) {
            this->mpu.update();

            sum_x += this->mpu.getAccelerometerValuesX();
            sum_y += this->mpu.getAccelerometerValuesY();
            sum_z += this->mpu.getAccelerometerValuesZ();

            delay(5);
        }

        this->baseline_x = sum_x / samples;
        this->baseline_y = sum_y / samples;
        this->baseline_z = sum_z / samples;
    }

    void MovementSystem::recalibrate(uint16_t samples) {
        this->calibrate(samples);
        this->current_movement = movement_t::NONE;
        this->movement_start_time = 0;
    }

    movement_t MovementSystem::oppositeOf(movement_t movement) {
        switch (movement) {
            case movement_t::UP:    return movement_t::DOWN;
            case movement_t::DOWN:  return movement_t::UP;
            case movement_t::LEFT:  return movement_t::RIGHT;
            case movement_t::RIGHT: return movement_t::LEFT;
            case movement_t::FRONT: return movement_t::BACK;
            case movement_t::BACK:  return movement_t::FRONT;
            default: return movement_t::NONE;
        }
    }

    void MovementSystem::endCurrentMovement(unsigned long now) {
        this->last_ended_movement = this->current_movement;
        this->cooldown_until = now + this->opposite_cooldown_ms;
        this->current_movement = movement_t::NONE;
        this->movement_start_time = 0;
    }

    float MovementSystem::deltaX() { return this->mpu.getAccelerometerValuesX() - this->baseline_x; }
    float MovementSystem::deltaY() { return this->mpu.getAccelerometerValuesY() - this->baseline_y; }
    float MovementSystem::deltaZ() { return this->mpu.getAccelerometerValuesZ() - this->baseline_z; }

    movement_t MovementSystem::detectMovement() {
        this->mpu.update();

        auto dx = this->deltaX();
        auto dy = this->deltaY();
        auto dz = this->deltaZ();

        auto ax = abs(dx), ay = abs(dy), az = abs(dz);

        // Candidate is the axis with the largest deviation from the resting position.
        // The runner-up is kept so the candidate must clearly stand out from it.
        movement_t candidate = movement_t::NONE;
        float top1 = 0.0f, top2 = 0.0f;

        if (ax >= ay && ax >= az) {
            top1 = ax; top2 = (ay >= az) ? ay : az;
            candidate = (dx < 0) ? movement_t::UP : movement_t::DOWN;
        } else if (ay >= ax && ay >= az) {
            top1 = ay; top2 = (ax >= az) ? ax : az;
            candidate = (dy < 0) ? movement_t::LEFT : movement_t::RIGHT;
        } else {
            top1 = az; top2 = (ax >= ay) ? ax : ay;
            candidate = (dz < 0) ? movement_t::FRONT : movement_t::BACK;
        }

        bool is_expressive = (top1 - top2) >= this->dominance_margin;
        unsigned long now = millis();

        if (this->current_movement == movement_t::NONE) {
            // The opposite of a movement that just ended is the deceleration rebound, not a new gesture.
            bool is_rebound = (now < this->cooldown_until) &&
                (candidate == MovementSystem::oppositeOf(this->last_ended_movement));

            if (!is_rebound && is_expressive && top1 >= this->threshold_enter) {
                this->current_movement = candidate;
                this->movement_start_time = now;
            }
            return this->current_movement;
        }

        // A movement longer than the maximum duration means the belt shifted
        // position, so the resting position is recalibrated.
        if (now - this->movement_start_time >= this->max_movement_duration_ms) {
            this->recalibrate();
            return this->current_movement;
        }

        // Another axis took over dominance, so the movement is corrected.
        if (candidate != this->current_movement && is_expressive && top1 >= this->threshold_enter) {
            this->endCurrentMovement(now);
            this->current_movement = candidate;
            this->movement_start_time = now;
            return this->current_movement;
        }

        // Holds the movement for a minimum duration, since translations only
        // produce a brief acceleration pulse.
        if (now - this->movement_start_time < this->min_movement_duration_ms) {
            return this->current_movement;
        }

        float active_axis_value = 0.0f;

        switch (this->current_movement) {
            case movement_t::UP:    active_axis_value = -dx; break;
            case movement_t::DOWN:  active_axis_value =  dx; break;
            case movement_t::LEFT:  active_axis_value = -dy; break;
            case movement_t::RIGHT: active_axis_value =  dy; break;
            case movement_t::FRONT: active_axis_value = -dz; break;
            case movement_t::BACK:  active_axis_value =  dz; break;
            default: break;
        }

        if (active_axis_value < this->threshold_exit) {
            this->endCurrentMovement(now);
        }

        return this->current_movement;
    }

    void MovementSystem::debugPrint() {
        this->mpu.update();

        auto dx = this->deltaX();
        auto dy = this->deltaY();
        auto dz = this->deltaZ();

        Serial.printf("dx=%.2f dy=%.2f dz=%.2f | enter=%.2f exit=%.2f margin=%.2f\n",
            dx, dy, dz, this->threshold_enter, this->threshold_exit, this->dominance_margin);
    }

}
