

#ifndef HEXAPOD_H
#define HEXAPOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

#include "hexapod/hexapod_defs.h"

void HPOD_init(struct hexapod_s* hexapod, float offset_a, float len_ab, float len_bc);

void HPOD_leg_ik2(struct hexapod_s* hexapod, float d, float h, float* alpha, float* beta);

void HPOD_leg_ik3(struct hexapod_s* hexapod, float x, float y, float h,
                 float* alpha, float* beta, float* omega);

#ifdef __cplusplus
}
#endif

#endif
