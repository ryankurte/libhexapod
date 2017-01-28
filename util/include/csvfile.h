/**
 * Libhexapod
 * Hexapod Utility CSV writer
 *
 * https://github.com/ryankurte/libhexapod
 * Copyright 2017 Ryan Kurte
 */

#ifndef HEXAPOD_UTIL_FILE_H
#define HEXAPOD_UTIL_FILE_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "util.h"

void write_file(const char* name, int count, float* indicies, 
    float targets[][NUM_SLICES_MAX], float angles[][NUM_SLICES_MAX], float actuals[][NUM_SLICES_MAX]);

#ifdef __cplusplus
}
#endif

#endif
