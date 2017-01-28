/**
 * Libhexapod
 * Hexapod mixer
 *
 * https://github.com/ryankurte/libhexapod
 * Copyright 2017 Ryan Kurte
 */

#include "hexapod/servo.h"

#include <stdint.h>
#include <math.h>

#define SERVO_LIMIT_RANGE(min, max, val)   ((val < min) ? min : (val > max) ? max : val)

/**
 * @brief Initialise servo adaptor
 */
void HPOD_servo_init(struct hpod_servo_s *servo, float range_rads, int output_range, int output_offset)
{
    servo->range_rads = range_rads;
    servo->output_range = output_range;
    servo->output_offset = output_offset;

    servo->scale = range_rads / servo->output_range;
}

/**
 * @brief Mix servo outputs from an input angle
 */
int HPOD_servo_mix(struct hpod_servo_s *servo, float angle)
{
    float limited_angle = SERVO_LIMIT_RANGE(-servo->range_rads, servo->range_rads, angle);

    int output = (int) (limited_angle / servo->scale + servo->output_offset);

    return output;
}

