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
rawdata = numpy.genfromtxt(args.filename, delimiter=", ", dtype=None, unpack=True)

lines = int(len(rawdata) - 1)
cols = int(lines / 3)
rows = int(lines / cols)

print("Loaded {} lines for {} rows and {} columns".format(lines, rows, cols))

# Setup figure
fig1 = plt.figure()
fig1.canvas.set_window_title('Hexapod Library Output Test')
plt.subplots_adjust(hspace=.5, bottom=0.05)

# Create a plot instance
def add_plot(x, y, i, scale, name, data):
    ax1 = fig1.add_subplot(x, y, i)
    ax1.plot(scale, data)
    ax1.title.set_text(name)

# Render all lines
for i in range(0, lines):
    add_plot(rows, cols, i+1, rawdata[0][1:], rawdata[i+1][0].decode('UTF-8'), rawdata[i+1][1:])

# Render plot
plt.show();
