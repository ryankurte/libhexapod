#!/usr/bin/env python 

import sys
import csv
import numpy
import matplotlib.pyplot as plt

filename = sys.argv[1]

# Load files
data = numpy.genfromtxt(filename, delimiter=", ")

fig1 = plt.figure()
fig1.canvas.set_window_title('Hexapod Library Output Test')
plt.subplots_adjust(hspace=.5, bottom=0.05)

def add_plot(num, name, scale, data):
    ax1 = fig1.add_subplot(num)
    ax1.plot(scale, data, 'r-')
    ax1.title.set_text(name)

# Create plots
add_plot(331, 'Target X (left/right)', data[0], data[1])
add_plot(334, 'Target Y (forward/back)', data[0], data[2])
add_plot(337, 'Target Z (up/down)', data[0], data[3])

add_plot(332, 'Leg Alpha', data[0], data[4])
add_plot(335, 'Leg Beta', data[0], data[5])
add_plot(338, 'Leg Theta', data[0], data[6])

add_plot(333, 'Actual X (left/right)', data[0], data[7])
add_plot(336, 'Actual Y (forward/back)', data[0], data[8])
add_plot(339, 'Actual Z (up/down)', data[0], data[9])

plt.show();


