#!/usr/bin/env python3
# Helper to graph outputs from hex-util for diagnostic purposes
#
# https://github.com/ryankurte/libhexapod
# Copyright 2017 Ryan Kurte

import sys
import csv
import numpy
import matplotlib.pyplot as plt
import argparse

# Parse arguments
parser = argparse.ArgumentParser(description='Render csv outputs from hex-util')
parser.add_argument('--filename', default="output.csv",
                    help='input file for graphing')
args = parser.parse_args()

# Load files
data = numpy.genfromtxt(args.filename, delimiter=", ")

# Setup figure
fig1 = plt.figure()
fig1.canvas.set_window_title('Hexapod Library Output Test')
plt.subplots_adjust(hspace=.5, bottom=0.05)

# Create a plot instance
def add_plot(num, name, scale, data):
    ax1 = fig1.add_subplot(num)
    ax1.plot(scale, data, 'r-')
    ax1.title.set_text(name)

# Create individual plots
add_plot(331, 'Target X (left/right)', data[0], data[1])
add_plot(334, 'Target Y (forward/back)', data[0], data[2])
add_plot(337, 'Target Z (up/down)', data[0], data[3])

add_plot(332, 'Leg Alpha', data[0], data[4])
add_plot(335, 'Leg Beta', data[0], data[5])
add_plot(338, 'Leg Theta', data[0], data[6])

add_plot(333, 'Actual X (left/right)', data[0], data[7])
add_plot(336, 'Actual Y (forward/back)', data[0], data[8])
add_plot(339, 'Actual Z (up/down)', data[0], data[9])

# Render plot
plt.show();
