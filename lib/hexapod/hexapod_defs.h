
#ifndef HEXAPOD_DEFS_H
#define HEXAPOD_DEFS_H

/**
 * Hexapod object for internal use
 * This stores the context required to perform hexapod related calculations
 */
struct hexapod_s {
    float offset_a; //!< Distance between planar and vertical shoulder joints
    float len_ab;   //!< Length of upper leg (femur)
    float len_bc;   //!< Length of foreleg
};

#endif

