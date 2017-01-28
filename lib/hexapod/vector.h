/**
 * Libhexapod
 * @file
 * @brief Vector definitions for hexapod library use
 *
 * https://github.com/ryankurte/libhexapod
 * Copyright 2017 Ryan Kurte
 */

#ifndef HEXAPOD_VECTOR_H
#define HEXAPOD_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

/** \defgroup Vector
 * @brief Vector helpers for use in libhexapod
 * @{
 */

struct hpod_vector2_s {
    float x;
    float y;
};
typedef struct hpod_vector2_s hpod_vector2_t;

struct hpod_vector3_s {
    float x;
    float y;
    float z;
};
typedef struct hpod_vector3_s hpod_vector3_t;


hpod_vector2_t hpod_vector2_mul(hpod_vector2_t *a, hpod_vector2_t *b);

hpod_vector3_t hpod_vector3_mul(hpod_vector3_t *a, hpod_vector3_t *b);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif
