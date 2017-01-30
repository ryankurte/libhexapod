#!/usr/bin/env python3
# Hexapod walking algorithm test and animation
# Displays output of walking algorithm, this should be reimplemented using libhexapod c bindings
#
# https://github.com/ryankurte/libhexapod
# Copyright 2017 Ryan Kurte

import math
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation

import hexapod

# Configuration variables

hexy = hexapod.Hexapod(300, 150, 50, 80, 150)

gait=(100.0, 200.0, 40.0)
offset=(150.0, 0.0, -50.0)
lift=0.1

hexy.set_gait(gait, offset, lift)


# Internal variables
phase = np.arange(-2, 2, 0.01)

mx = 0.0
my = 1.0
mr = 0.0

freq = 1.0

desired_pos = [[0] * len(phase)] * 3

x = [0] * len(phase)
y = [0] * len(phase)
h = [0] * len(phase)

x1 = [0] * len(phase)
y1 = [0] * len(phase)
h1 = [0] * len(phase)

a = [0] * len(phase)
b = [0] * len(phase)
o = [0] * len(phase)

a1 = [0] * len(phase)
b1 = [0] * len(phase)
o1 = [0] * len(phase)

ep_x = [0] * len(phase)
ep_y = [0] * len(phase)
ep_h = [0] * len(phase)

# Gait control functions
def fx(m, p):
    return np.sin(p * np.pi) * gait[0] / 2 * m[0] + offset[0]

def fy(m, p):
    return np.sin(p * np.pi) * gait[1] / 2 * m[1]

def fh(m, p):
    p = math.fmod(p + 3.0, 2.0) - 1.0
    if(np.abs(p) > (0.5 + lift / 2)):
        return -gait[2] / 2 + offset[2]
    elif(np.abs(p) < (0.5 - lift / 2)):
        return gait[2] / 2 + offset[2]
    elif(p < 0):
        return np.cos((p + lift/2) / lift * np.pi) * gait[2] / 2 + offset[2]
    else:
        return np.cos((p - lift/2) / lift * np.pi) * gait[2] / 2 + offset[2]
        
def gait_calc(m, p):
    return (fx(m, p), fy(m, p), fh(m, p))

def ik2(d, h):
    # Calculate length between A & C
    len_ac = np.sqrt(math.pow(d, 2) + math.pow(h, 2))

    # Calculate rotational offset from zero frame (h = 0)
    angle_dh = math.atan(h / d)

    # Calculate angles in local frame
    angle_a = math.acos((math.pow(len_ac, 2) + math.pow(hexy.len_ab, 2) - math.pow(hexy.len_bc, 2)) / (2 * len_ac * hexy.len_ab))
    angle_b = math.acos((math.pow(hexy.len_ab, 2) + math.pow(hexy.len_bc, 2) - math.pow(len_ac, 2)) / (2 * hexy.len_ab * hexy.len_bc))

    # Convert back into world frame
    alpha = angle_a + angle_dh
    beta = angle_b

    return (alpha, beta)

def ik3(x, y, h):
    # Calculate distance and angle from origin to point (x, y)
    len_xy = np.sqrt(math.pow(x, 2) + math.pow(y, 2))
    angle_xy = math.atan(y / x)

    # Process ik2 equation with total distance (less offset between joints at A)
    alpha, beta = ik2(len_xy - hexy.offset_a, h)

    # Output all angles
    theta = angle_xy
    return (alpha, beta, theta)

def calculate_fk3(a, b, o):
    pos_a = (np.cos(o) * hexy.offset_a, np.sin(o) * hexy.offset_a, 0)
    hexy.len_ab_xy = np.cos(a) * hexy.len_ab
    pos_b = (pos_a[0] + np.cos(o) * hexy.len_ab_xy, pos_a[1] + np.sin(o) * hexy.len_ab_xy, pos_a[2] + np.sin(a) * hexy.len_ab)
    world_beta = a + b - np.pi
    hexy.len_bc_xy = np.cos(world_beta) * hexy.len_bc
    pos_c = (pos_b[0] + np.cos(o) * hexy.len_bc_xy, pos_b[1] + np.sin(o) * hexy.len_bc_xy, pos_b[2] + np.sin(world_beta) * hexy.len_bc)

    return (pos_a, pos_b, pos_c)

def calculate_fk3_ep(a, b, o):
    pa, pb, pc = calculate_fk3(a, b, o)
    return pc

def gen_subplt(title, index, range, data, alt, error):
    subpolt = plt.subplot2grid((3, 4), index)
    subpolt.set_title(title)
    subpolt.set_ylim(-range, range)
    dataplot, = subpolt.plot(phase, data, 'b')
    altplot, = subpolt.plot(phase, alt, 'g')
    errorplot, = subpolt.plot(phase, error, 'r')
    return (subpolt, dataplot, altplot, errorplot)

# Create plots
fig1 = plt.figure()
plt.subplots_adjust(hspace=.5, bottom=0.25)

# Desired and output gait plots
xsubplt, xplot, xaltplot, xerrplot = gen_subplt('Leg Offset X', (0, 0), offset[0] + gait[0] / 2, x, x1, ep_x)
ysubplt, yplot, yaltplot, yerrplot = gen_subplt('Leg Offset Y', (1, 0), gait[1] / 2, y, y1, ep_y)
hsubplt, hplot, haltplot, herrplot = gen_subplt('Leg Offset H', (2, 0), np.abs(offset[2]) + gait[2], h, h1, ep_h)

# Inverse Kinematic plots
asubplt = plt.subplot2grid((3, 4), (0, 1))
plt.title('Alpha')
asubplt.set_ylim(-np.pi, np.pi)
aplot, = plt.plot(phase, a, 'b')
a1plot, = plt.plot(phase, a1, 'r')

bsubplt = plt.subplot2grid((3, 4), (1, 1))
plt.title('Beta')
bsubplt.set_ylim(-np.pi, np.pi)
bplot, = plt.plot(phase, b, 'b')
b1plot, = plt.plot(phase, b1, 'r')

osubplt = plt.subplot2grid((3, 4), (2, 1))
plt.title('theta')
osubplt.set_ylim(-np.pi, np.pi)
oplot, = plt.plot(phase, o, 'b')
o1plot, = plt.plot(phase, o1, 'r')

# 3d Animation
animsubplt = plt.subplot2grid((3, 4), (0, 2), colspan=2, rowspan=3, projection='3d')
xyhplot, = animsubplt.plot(x, y, zs=h)
animplot, = animsubplt.plot([0, 1], [0, 1], zs=[0, 1])
legplot, = animsubplt.plot([0], [0], zs=[0])

animscale = max(gait[0] + offset[0], max(gait[1], gait[2] + abs(offset[2])))

animsubplt.set_xlim3d(-0, animscale)
animsubplt.set_ylim3d(-animscale / 2, animscale / 2)
animsubplt.set_zlim3d(-animscale / 2, animscale / 2)
animsubplt.set_xlabel('X')
animsubplt.set_ylabel('Y')
animsubplt.set_zlabel('H')

# Create sliders
axcolor = 'lightgoldenrodyellow'
xmove = plt.axes([0.25, 0.15, 0.5, 0.03], axisbg=axcolor)
ymove = plt.axes([0.25, 0.1, 0.5, 0.03], axisbg=axcolor)
#fmove = plt.axes([0.25, 0.05, 0.5, 0.03], axisbg=axcolor)

sxmove = Slider(xmove, 'Movement X', -1.0, 1.0, valinit=mx)
symove = Slider(ymove, 'Movement Y', -1.0, 1.0, valinit=my)
#sfmove = Slider(fmove, 'Movement F', 0, 2.0, valinit=freq)

# Update data function
def update(val):
    mx = sxmove.val
    my = symove.val
    
    m = (mx, my, mr)

    # Generate new data
    for i in range(0, len(phase)):
        x[i], y[i], h[i] = gait_calc(m, phase[i])
        x1[i], y1[i], h1[i] = hexy.gait_calc(m, phase[i])
        a[i], b[i], o[i] = ik3(x[i], y[i], h[i])
        a1[i], b1[i], o1[i] = hexy.leg_ik3(x1[i], y1[i], h1[i])
        ep_x[i], ep_y[i], ep_h[i] = calculate_fk3_ep(a1[i], b1[i], o1[i])

    # Attach to plots
    xplot.set_data(phase, x)
    yplot.set_data(phase, y)
    hplot.set_data(phase, h)
    xerrplot.set_data(phase, ep_x)
    yerrplot.set_data(phase, ep_y)
    herrplot.set_data(phase, ep_h)
    xaltplot.set_data(phase, x1)
    yaltplot.set_data(phase, y1)
    haltplot.set_data(phase, h1)
    
    aplot.set_data(phase, a)
    bplot.set_data(phase, b)
    oplot.set_data(phase, o)

    a1plot.set_data(phase, a1)
    b1plot.set_data(phase, b1)
    o1plot.set_data(phase, o1)

    xyhplot.set_data(x, y)
    xyhplot.set_3d_properties(h)

# Bind update to changes in sliders
sxmove.on_changed(update)
symove.on_changed(update)

def build_leg(frame, offset):

    pos_a, pos_b, pos_c = calculate_fk3(a[frame], b[frame], o[frame])

    leg_x = [0 + offset[0], pos_a[0] + offset[0], pos_b[0] + offset[0], pos_c[0] + offset[0]]
    leg_y = [0 + offset[1], pos_a[1] + offset[1], pos_b[1] + offset[1], pos_c[1] + offset[1]]
    leg_h = [0 + offset[2], pos_a[2] + offset[2], pos_b[2] + offset[2], pos_c[2] + offset[2]]

    return leg_x, leg_y, leg_h

# Animation update function
def update_anim(frame):
    animplot.set_data([0, x[frame]], [0, y[frame]])
    animplot.set_3d_properties([0, h[frame]])

    pos_a, pos_b, pos_c = calculate_fk3(a1[frame], b1[frame], o1[frame])

    leg_x, leg_y, leg_h = build_leg(frame, (0, 0, 0))

    legplot.set_data(leg_x, leg_y)
    legplot.set_3d_properties(leg_h)

# Enable animation
line_ani = animation.FuncAnimation(fig1, update_anim, len(phase),
                                   interval=10, blit=False)

update(0)

plt.show()


