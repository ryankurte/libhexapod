/**
 * Libhexapod
 * Hexapod utility
 * This runs the hexapod library and writes outputs to a csv for analysis
 *
 * https://github.com/ryankurte/libhexapod
 * Copyright 2017 Ryan Kurte
 */

#include <stdio.h>
#include <stdint.h>

#include "hexapod/hexapod.h"

#include "util.h"
#include "csvfile.h"


int main(int argc, char **argv)
{
    struct config_s config = DEFAULT_CONFIG;

    parse_config(argc, argv, &config);

    // Create hexapod control instance
    struct hexapod_s hexy;
    HPOD_init(&hexy, &config.hexapod);

    int res = HPOD_gait_valid(&hexy, &config.gait);
    if (res < 0) {
        printf("WARNING: Hexapod and gait combination could not be solved (error: %d)\r\n", res);
        return -1;
    }

    // Output data
    float data[NUM_SLICES_MAX][10];
    // Calculate position of every slice
    for (int i = 0; i < config.slices; i++) {
        float phase = i / (((float)config.slices - 1) / 2) - 1;
        data[i][0] = phase;

        // Calculate leg position for a given gait
        struct hpod_vector3_s position;
        HPOD_gait_calc(&hexy, &config.gait, &config.movement, phase, &position);

        // Save leg positions
        data[i][1] = position.x;
        data[i][2] = position.y;
        data[i][3] = position.z;

        // Calculate servo control
        float alpha, beta, theta;
        HPOD_leg_ik3(&hexy, &position, &alpha, &beta, &theta);

        data[i][4] = alpha;
        data[i][5] = beta;
        data[i][6] = theta;

        // Calculate control inverse
        struct hpod_vector3_s actual;
        HPOD_leg_fk3(&hexy, alpha, beta, theta, &actual);
        data[i][7] = actual.x;
        data[i][8] = actual.y;
        data[i][9] = actual.z;
    }

    // Write outputs
    write_file("output.csv", config.slices, data);

    return 0;
}

