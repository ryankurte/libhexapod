
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

    // Create position control struct
    struct hpod_vector3_s position;
    position.x = 1;
    position.y = 0;
    position.z = 0;

    float indicies[NUM_SLICES];

    float positions[3][NUM_SLICES];
    float angles[3][NUM_SLICES];

    // Calculate position of every slice
    for (int i = 0; i < NUM_SLICES; i++) {
        float phase = i / (((float)NUM_SLICES - 1) / 2);
        indicies[i] = phase;

        // Calculate leg position for a given gait
        HPOD_gait_calc(&hexy, &gait, &movement, phase, &position);

        // Save leg positions
        positions[0][i] = position.x;
        positions[1][i] = position.y;
        positions[2][i] = position.z;

        // Calculate servo control
        HPOD_leg_ik3(&hexy, position.x, position.y, position.z,
            &angles[0][i], &angles[1][i], &angles[2][i]);
    }

    // Open output file
    FILE *fp = fopen("output.csv", "w");

    // Write outputs
    write_line(fp, "I", NUM_SLICES, indicies);
    write_line(fp, "X", NUM_SLICES, positions[0]);
    write_line(fp, "Y", NUM_SLICES, positions[1]);
    write_line(fp, "Z", NUM_SLICES, positions[2]);
    write_line(fp, "ALPHA", NUM_SLICES, angles[0]);
    write_line(fp, "BETA", NUM_SLICES, angles[1]);
    write_line(fp, "THETA", NUM_SLICES, angles[2]);

    // Close file
    fclose(fp);
}

