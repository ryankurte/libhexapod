
#ifndef HEXAPOD_DEFS_H
#define HEXAPOD_DEFS_H

#include <math.h>

// Helpers for degree <-> radian conversion
#define HPOD_RAD_TO_DEG(a)          (a * 180 / M_PI)
#define HPOD_DEG_TO_RAD(a)          (a * M_PI / 180)

// Helpers for scalar <-> radian conversion
// Scalar ranges from -1 to 1 are used to represent angles to simplify floating point
// methods and reduce error where possible
#define HPOD_RAD_TO_SCL(a)          (a / M_PI)
#define HPOD_SCL_TO_RAD(a)          (a * M_PI)
#define HPOD_WRAP_SCL(a)            ((a >= 0.0) ? fmod(a, 1.0) : -fmod(a, 1.0))

#define HPOD_LIMIT_RANGE(min, max, val)   ((val < min) ? min : (val > max) ? max : val)

struct hpod_gait_s {
    float movement_width;
    float offset_width;
    float movement_height;
    float offset_height;
    float movement_length;
    float height_scale;
};

struct hpod_vector_s {
    float x;
    float y;
    float r;
};

/**
 * Hexapod object for internal use
 * This stores the context required to perform hexapod related calculations
 */
struct hexapod_s {
    float width;    //!< Width between joints
    float offset_a; //!< Distance between planar and vertical shoulder joints
    float len_ab;   //!< Length of upper leg (femur)
    float len_bc;   //!< Length of foreleg
};

#endif

