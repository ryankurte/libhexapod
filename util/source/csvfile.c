
#include "csvfile.h"

#include <stdio.h>

#include "util.h"


void write_line(FILE* fp, int count, float* data)
{
    for (int i = 0; i < count; i++) {
        if (i < (count - 1)) {
            fprintf(fp, "%.2f, ", data[i]);
        } else {
            fprintf(fp, "%.2f", data[i]);
        }

    }
    fprintf(fp, "\n");
}

void write_file(const char* name, int count, float data[NUM_SLICES_MAX][10])
{
    // Open output file
    FILE *fp = fopen(name, "w");

    // Write outputs
    fprintf(fp, "Phase, Target X (left/right), Target Y (forward/back), Target Z (up/down), "
            "Leg Angle Alpha, Leg Angle Beta, Leg Angle Theta, "
            "Actual X (left/right), Actual Y (forward/back), Actual Z (up/down)\n");

    for (int i = 0; i < count; i++) {
        write_line(fp, 10, data[i]);
    }

    // Close file
    fclose(fp);
}
