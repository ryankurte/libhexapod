

#ifndef HEXAPOD_H
#define HEXAPOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

#include "hexapod/hexapod_defs.h"

void HPOD_init(struct hexapod_s* hexapod, float width, float offset_a, float len_ab, float len_bc);

void HPOD_leg_ik2(struct hexapod_s* hexapod, float d, float h, float* alpha, float* beta);

void HPOD_leg_ik3(struct hexapod_s* hexapod, float x, float y, float h,
                 float* alpha, float* beta, float* omega);

void HPOD_gait_calc(struct hexapod_s* hexapod, struct hpod_gait_s *gait, struct hpod_vector_s *movement,
                    float phase, float* x, float* y, float* h);

#ifdef __cplusplus
}
#endif

#endif
