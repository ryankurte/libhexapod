

#ifndef HEXAPOD_H
#define HEXAPOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

#include "hexapod/hexapod_defs.h"

void HPOD_init(struct hexapod_s* hexapod, float width, float length, float offset_a, float len_ab, float len_bc);

void HPOD_leg_ik2(struct hexapod_s* hexapod, float d, float h, float* alpha, float* beta);

void HPOD_leg_ik3(struct hexapod_s* hexapod, float x, float y, float h,
                  float* alpha, float* beta, float* theta);

void HPOD_leg_fk2(struct hexapod_s* hexapod, float alpha, float beta,
                  float* x, float* h);

void HPOD_leg_fk3(struct hexapod_s* hexapod, float alpha, float beta, float theta,
                  float* x, float* y, float* h);

void HPOD_body_transform(struct hexapod_s* hexapod, float alpha, float beta, int offset_x, int offset_y,
                         float x, float y, float z, float* joint_x, float* joint_y, float* joint_z);

void HPOD_world_roll_pitch(struct hexapod_s* hexapod, float angle, int offset,
                         float xy, float z, float* adj_xy, float* adj_z);

void HPOD_gait_calc(struct hexapod_s* hexapod, struct hpod_gait_s *gait, struct hpod_vector_s *movement,
                    float phase, float* x, float* y, float* h);

#ifdef __cplusplus
}
#endif

#endif
