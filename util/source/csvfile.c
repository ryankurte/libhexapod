
#include "csvfile.h"

#include <stdio.h>

#include "util.h"


void write_line(FILE* fp, char* name, int count, float* data)
{
    //fprintf(fp, "%s, ", name);
    for (int i = 0; i < count; i++) {
        if (i < (count - 1)) {
            fprintf(fp, "%.2f, ", data[i]);
        } else {
            fprintf(fp, "%.2f", data[i]);
        }

    }
    fprintf(fp, "\n");
}

void write_file(const char* name, int count, float* indicies, 
    float targets[][NUM_SLICES_MAX], float angles[][NUM_SLICES_MAX], float actuals[][NUM_SLICES_MAX])
{
    // Open output file
    FILE *fp = fopen(name, "w");

    // Write outputs
    write_line(fp, "I", count, indicies);
    write_line(fp, "X_TARGETS", count, targets[0]);
    write_line(fp, "Y_TARGETS", count, targets[1]);
    write_line(fp, "Z_TARGETS", count, targets[2]);
    write_line(fp, "LEG_ALPHA", count, angles[0]);
    write_line(fp, "LEG_BETA",  count, angles[1]);
    write_line(fp, "LEG_THETA", count, angles[2]);
    write_line(fp, "X_ACTUAL",  count, actuals[0]);
    write_line(fp, "Y_ACUAL",   count, actuals[1]);
    write_line(fp, "Z_ACTUAL",  count, actuals[2]);

    // Close file
    fclose(fp);
}
