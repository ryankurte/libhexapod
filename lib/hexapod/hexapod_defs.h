/**
 * Libhexapod
 * @file
 * @brief Hexapod definitions
 *
 * https://github.com/ryankurte/libhexapod
 * Copyright 2017 Ryan Kurte
 */

#ifndef HEXAPOD_DEFS_H
#define HEXAPOD_DEFS_H

#include <math.h>

#include "hexapod/vector.h"

#ifdef __cplusplus
extern "C" {
#endif


/** \defgroup Definitions
 * @brief Definitions used in libhexapod
 * @{
 */

// Helpers for degree <-> radian conversion
#define HPOD_RAD_TO_DEG(a)          (a * 180 / M_PI)
#define HPOD_DEG_TO_RAD(a)          (a * M_PI / 180)

// Helpers for scalar <-> radian conversion
// Scalar ranges from -1 to 1 are used to represent angles to simplify floating point
// methods and reduce error where possible
#define HPOD_RAD_TO_SCL(a)          (a / M_PI)
#define HPOD_SCL_TO_RAD(a)          (a * M_PI)
#define HPOD_WRAP_SCL(a)            fmod(a, 1.0)
#define HPOD_LIMIT_RANGE(min, max, val)   ((val < min) ? min : (val > max) ? max : val)

/**
 * Hexapod config object
 * This is used to configure the hexapod dimensions
 */
struct hexapod_config_s {
    float length;   //!< Hexapod length (between front and rear joints)
    float width;    //!< Hexapod width between joints
    float offset_a; //!< Distance between planar and vertical shoulder joints
    float len_ab;   //!< Length of upper leg (femur)
    float len_bc;   //!< Length of foreleg
};

// Default hexapod config for testing / convenience purposes
#define HPOD_DEFAULT_CONFIG {200, 100, 45, 80, 150}

/**
 * Hexapod object for internal use
 * This stores the context required to perform hexapod related calculations
 */
struct hexapod_s {
    struct hexapod_config_s config;
};

/**
 * Gait control object
 */
struct hpod_gait_s {
    struct hpod_vector3_s movement;     //!< Defines dimensions of leg movement box
    struct hpod_vector3_s offset;       //!< Defines offset of movement box
    float height_scale;                 //!< Defines leg lift offset as a factor of movement height
};

// Default gait for testing / convenience purposes
#define HPOD_DEFAULT_MOVEMENT {100.0, 200.0, 20.0}
#define HPOD_DEFAULT_OFFSET {150.0, 0.0, -70.0}
#define HPOD_DEFAULT_LIFT_SCALE 0.1
#define HPOD_DEFAULT_GAIT {HPOD_DEFAULT_MOVEMENT, HPOD_DEFAULT_OFFSET, HPOD_DEFAULT_LIFT_SCALE}

/**
 * Leg offset structure
 * Used for computation of leg offsets in output calculation
 */
struct leg_offset_s {
    int phase;
    int x;
    int y;
};

// Leg location multipliers
// Phase modifier, X offset, Y offset
extern struct leg_offset_s leg_offsets[6];

#ifdef __cplusplus
}
#endif


/** @}*/

#endif

