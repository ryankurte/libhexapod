/**
 * Libhexapod
 * @file
 * @brief Servo functions
 *
 * https://github.com/ryankurte/libhexapod
 * Copyright 2017 Ryan Kurte
 */

#ifndef HEXAPOD_SERVO_H
#define HEXAPOD_SERVO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

/** \defgroup Servo
 * @brief Servo output adaption
 * @{
 */

/**
 * @brief Servo description structure
 * Used to map angle outputs to servo ranges
 */
struct hpod_servo_s {
    float range_rads;   //!< Servo range in radians
    int output_range;   //!< Servo output range (ie. 1024 maximum)
    int output_offset;  //!< Servo output offset (ie. 512 midpoint)
    float scale;        //!< Servo scale factor
};

void HPOD_servo_init(struct hpod_servo_s *servo, float range_rads, int output_range, int output_offset);
int HPOD_servo_scale(struct hpod_servo_s *servo, float angle);
void HPOD_servo_mix(struct hpod_servo_s *servo, float in[6][3], int out[6][3]);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif
