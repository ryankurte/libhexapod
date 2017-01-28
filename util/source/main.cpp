
#include <stdio.h>
#include <stdint.h>

#include "hexapod/hexapod.h"


#define NUM_SLICES      100

void write_line(FILE* fp, char* name, int count, float* data)
{
    //fprintf(fp, "%s, ", name);
    for (int i=0; i < count; i++) {
        if (i < (count - 1)) {
            fprintf(fp, "%.2f, ", data[i]);
        } else {
            fprintf(fp, "%.2f", data[i]);
        }
        
    }
    fprintf(fp, "\n");
}

int main(int argc, char **argv)
{

    printf("Hexapod util\r\n");

    // Create hexapod control instance
    struct hexapod_s hexy;
    HPOD_init(&hexy, 100, 100, 20, 100, 120);

    // Create a gait instance
    struct hpod_gait_s gait;
    gait.movement.x = 50.0;
    gait.movement.y = 10.0;
    gait.movement.z = 50.0;
    gait.offset.x = 100.0;
    gait.offset.y = 0.0;
    gait.height_scale = 0.05;

    // Create movement control struct
    struct hpod_vector3_s movement;
    movement.x = 0;
    movement.y = 1;
    movement.z = 0;

    float indicies[NUM_SLICES];
    float targets[3][NUM_SLICES];
    float angles[3][NUM_SLICES];
    float actuals[3][NUM_SLICES];

    // Calculate position of every slice
    for (int i = 0; i < NUM_SLICES; i++) {
        float phase = i / (((float)NUM_SLICES - 1) / 2) - 1.0;
        indicies[i] = phase;

        // Calculate leg position for a given gait
        struct hpod_vector3_s position;
        HPOD_gait_calc(&hexy, &gait, &movement, phase, &position);

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

    // Open output file
    FILE *fp = fopen("output.csv", "w");

    // Write outputs
    write_line(fp, "I", NUM_SLICES, indicies);
    write_line(fp, "X_TARGETS", NUM_SLICES, targets[0]);
    write_line(fp, "Y_TARGETS", NUM_SLICES, targets[1]);
    write_line(fp, "Z_TARGETS", NUM_SLICES, targets[2]);
    write_line(fp, "LEG_ALPHA", NUM_SLICES, angles[0]);
    write_line(fp, "LEG_BETA",  NUM_SLICES, angles[1]);
    write_line(fp, "LEG_THETA", NUM_SLICES, angles[2]);
    write_line(fp, "X_ACTUAL",  NUM_SLICES, actuals[0]);
    write_line(fp, "Y_ACUAL",   NUM_SLICES, actuals[1]);
    write_line(fp, "Z_ACTUAL",  NUM_SLICES, actuals[2]);

    // Close file
    fclose(fp);
}

