/**
 * Libhexapod
 * Hexapod utility definitions
 *
 * https://github.com/ryankurte/libhexapod
 * Copyright 2017 Ryan Kurte
 */

#ifndef HPOD_UTIL_H
#define HPOD_UTIL_H

#include "hexapod/hexapod.h"

#ifdef __cplusplus
extern "C" {
#endif

// Maximum number of slices in output calculation
#define NUM_SLICES_MAX      1000
#define FILE_NAME_MAX       64

// Utility configuration
struct config_s {
    int slices;
    char filename[FILE_NAME_MAX];
    struct hexapod_config_s hexapod;
    struct hpod_gait_s gait;
    struct hpod_vector3_s movement;
};

// Default configuration
#define DEFAULT_CONFIG {100, "output.csv", HPOD_DEFAULT_CONFIG, HPOD_DEFAULT_GAIT, {0.0, 1.0, 0.0}}

void parse_config(int argc, char** argv, struct config_s* config);

#ifdef __cplusplus
}
#endif

#endif
