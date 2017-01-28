/**
 * Libhexapod
 * Hexapod utility helpers
 *
 * https://github.com/ryankurte/libhexapod
 * Copyright 2017 Ryan Kurte
 */

#include "util.h"

#include <getopt.h>
#include <string.h>
#include <stdio.h>

void print_help()
{
    struct config_s config = DEFAULT_CONFIG;

    printf("\r\nLibhexapod Simulation Utility");
    printf("\r\n");
    printf("\r\nThis runs hexapod algorithms and outputs data as a CSV for analysis\r\n");
    printf("Arguments:\r\n");
    printf("--file filename, file to write output to (default: %s)\r\n", config.filename);
    printf("--slices N, number of slices for computation (default: %d)\r\n", config.slices);
    printf("--movement-x N, X (left/right) movement (default: %.2f)\r\n", config.movement.x);
    printf("--movement-y N, Y (forward/reverse) movement (default: %.2f)\r\n", config.movement.y);
    printf("--movement-z N, Z rotational movement (default: %.2f)\r\n", config.movement.z);
    printf("\r\n");
}

void parse_config(int argc, char** argv, struct config_s* config)
{

    int c;
    int option_index = 0;

    // Setup options
    static struct option long_options[] = {
        {"help", no_argument,               0, 'h'},
        {"file", required_argument,         0, 'f'},
        {"slices", required_argument,       0, 's'},
        {"movement-x", required_argument,   0, 'x'},
        {"movement-y", required_argument,   0, 'y'},
        {"movement-z", required_argument,   0, 'z'},
        {0, 0, 0, 0}
    };

    while (1) {
        c = getopt_long (argc, argv, "h", long_options, &option_index);
        if (c == -1) {
            break;
        }

        switch (c) {
        case 'h':
            print_help();
            exit(0);
            break;
        case 'f':
            strncpy(config->filename, optarg, FILE_NAME_MAX);
            break;
        case 's':
            config->slices = atof(optarg);
            break;
        case 'x':
            config->movement.x = atof(optarg);
        case 'y':
            config->movement.y = atof(optarg);
        case 'z':
            config->movement.z = atof(optarg);
            break;
        default:
            printf("Unrecognized option %s\r\n", long_options[option_index].name);
            break;
        }
    }
}
