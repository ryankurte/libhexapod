/**
 * Libhexapod
 * Hexapod c++ wrapper
 *
 * https://github.com/ryankurte/libhexapod
 * Copyright 2017 Ryan Kurte
 */

#ifndef HEXAPOD_HPP
#define HEXAPOD_HPP

#include "hexapod/hexapod.h"

class Hexapod
{
public:

    void init(struct hexapod_config_s* config)
    {
        HPOD_init(&this->hexapod, config);
    }

    void leg_ik3(float x, float y, float h, float* alpha, float* beta, float* theta)
    {
        HPOD_leg_ik3(&this->hexapod, x, y, h, alpha, beta, theta);
    }

    void leg_fk3(float alpha, float beta, float theta, float* x, float* y, float* h)
    {
        HPOD_leg_fk3(&this->hexapod, alpha, beta, theta, x, y, h);
    }

    void gait_calc(struct hpod_gait_s *gait, struct hpod_vector3_s *movement,
                   float phase, struct hpod_vector3_s* leg_pos)
    {
        HPOD_gait_calc(&this->hexapod, gait, movement, phase, leg_pos);
    }

private:
    struct hexapod_s hexapod;
};

#endif
