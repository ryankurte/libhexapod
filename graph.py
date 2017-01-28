#!/usr/bin/env python 

import sys
import csv
import numpy
import matplotlib.pyplot as plt

filename = sys.argv[1]

# Load files
data = numpy.genfromtxt(filename, delimiter=", ")

# Create plots
fig1 = plt.figure()
plt.subplots_adjust(hspace=.5, bottom=0.05)

ax1 = fig1.add_subplot(321)
ax1.plot(data[0], data[1], 'r-')
ax1.title.set_text('Gait X (left/right)')

ax2 = fig1.add_subplot(323)
ax2.plot(data[0], data[2], 'k-')
ax2.title.set_text('Gait Y (forward/back)')

ax3 = fig1.add_subplot(325)
ax3.plot(data[0], data[3], 'b-')
ax3.title.set_text('Gait Z (up/down)')

ax4 = fig1.add_subplot(322)
ax4.plot(data[0], data[4], 'r-')
ax4.title.set_text('Leg X (forward/back)')

ax5 = fig1.add_subplot(324)
ax5.plot(data[0], data[5], 'k-')
ax5.title.set_text('Leg Y (left/right)')

ax6 = fig1.add_subplot(326)
ax6.plot(data[0], data[6], 'b-')
ax6.title.set_text('Leg Z (up/down)')

plt.show();


