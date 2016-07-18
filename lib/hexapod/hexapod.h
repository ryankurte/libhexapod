

#ifndef HEXAPOD_H
#define HEXAPOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

void HEX_leg_ik2(float d, float h, float* alpha, float* beta);
void HEX_leg_ik3(float x, float y, float h, float* alpha, float* beta, float* omega);

#ifdef __cplusplus
}
#endif

#endif
