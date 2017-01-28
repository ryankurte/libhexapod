
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

    // Output data
    float indicies[NUM_SLICES_MAX];
    float targets[3][NUM_SLICES_MAX];
    float angles[3][NUM_SLICES_MAX];
    float actuals[3][NUM_SLICES_MAX];

    // Calculate position of every slice
    for (int i = 0; i < config.slices; i++) {
        float phase = i / (((float)config.slices - 1) / 2) - 1.0;
        indicies[i] = phase;

        // Calculate leg position for a given gait
        struct hpod_vector3_s position;
        HPOD_gait_calc(&hexy, &config.gait, &config.movement, phase, &position);

        // Save leg positions
        targets[0][i] = position.x;
        targets[1][i] = position.y;
        targets[2][i] = position.z;

        // Calculate servo control
        HPOD_leg_ik3(&hexy, position.x, position.y, position.z,
                     &angles[0][i], &angles[1][i], &angles[2][i]);

        HPOD_leg_fk3(&hexy, angles[0][i], angles[1][i], angles[2][i],
                     &actuals[0][i], &actuals[1][i], &actuals[2][i]);

    }

    // Write outputs
    write_file("output.csv", config.slices, indicies, targets, angles, actuals);

    return 0;
}

