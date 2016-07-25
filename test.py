import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation

# Configuration variables

offset_a = 4
len_ab = 10
len_bc = 10

gait_width = 10
gait_length = 10
gait_height = 10

height_scale = 0.1

# Internal variables
phase = np.arange(-1, 1, 0.01)

mx = 0.0
my = 1.0
mr = 0.0

freq = 1.0

x = [0] * len(phase)
y = [0] * len(phase)
h = [0] * len(phase)

# Gait control functions
def fx(m, p):
    return np.sin(p * np.pi) * gait_width / 2 * m[0];

def fy(m, p):
    return np.sin(p * np.pi) * gait_length / 2 * m[1];

def fh(m, p):
    if(np.abs(p) > (0.5 + height_scale / 2)):
        return -gait_height / 2;
    elif(np.abs(p) < (0.5 - height_scale / 2)):
        return gait_height / 2;
    elif(p < 0):
        return np.cos((p + height_scale/2) / height_scale * np.pi) * gait_height / 2;
    else:
        return np.cos((p - height_scale/2) / height_scale * np.pi) * gait_height / 2;
        

def ik2(d, h):
    # Calculate length between A & C
    len_ac = np.sqrt(np.pow(d, 2) + np.pow(h, 2));

    # Calculate rotational offset from zero frame (h = 0)
    angle_dh = np.atan(h / d);

    # Split into two regular triangles & calculate length of shared face
    # a^2 + b^2 = c^2 & d = len_ax + len_cx
    #   -> d^2 = len_ab^2 - len_bx^2 + len_bc^2 - len_bx^2
    #   -> len_bx^2 = - (d^2 - len_ab^2 - len_bc^2) / 2
    opposite = -np.sqrt((np.pow(len_ac, 2) - np.pow(len_ab, 2) - np.pow(len_bc, 2)) / 2);

    # Calculate angles
    angle_a = acos(opposite / len_ab);
    angle_c = acos(opposite / len_bc);
    angle_b = M_PI - angle_a - angle_c;

    # Convert back into world frame
    alpha = angle_a + angle_dh;
    beta = angle_c;

    return (alpha, beta)

def ik3(x, y, h):
    # Calculate distance and angle from origin to point (x, y)
    len_xy = np.sqrt(np.pow(x, 2) + np.pow(y, 2));
    angle_xy = np.atan(y / x);

    # Process ik2 equation with total distance (less offset between joints at A)
    alpha, beta = HPOD_leg_ik2(len_xy - offset_a, h);

    # Output all angles
    omega = angle_xy;
    return (alpha, beta, omega)

# Generate new data
m = (mx, my, mr);
for i in range(0, len(phase)):
    x[i] = fx(m, phase[i])
    y[i] = fy(m, phase[i])
    h[i] = fh(m, phase[i])

# Create plots
fig1 = plt.figure()
plt.subplots_adjust(hspace=.5, bottom=0.25)

xsubplt = plt.subplot2grid((3, 2), (0, 0))
plt.title('X')
xsubplt.set_ylim(-gait_width / 2, gait_width / 2);
xplot, = plt.plot(phase, x);

ysubplt = plt.subplot2grid((3, 2), (1, 0))
plt.title('Y')
ysubplt.set_ylim(-gait_length / 2, gait_length / 2);
yplot, = plt.plot(phase, y);

hsubplt = plt.subplot2grid((3, 2), (2, 0))
plt.title('H')
hsubplt.set_ylim(-gait_height / 2, gait_height / 2);
hplot, = plt.plot(phase, h);

animsubplt = plt.subplot2grid((3, 2), (0, 1), rowspan=3, projection='3d')
xyhplot, = animsubplt.plot(x, y, zs=h)
animplot, = animsubplt.plot([0, 1], [0, 1], zs=[0, 1])
animsubplt.set_xlim3d(-gait_width / 2, gait_width / 2);
animsubplt.set_ylim3d(-gait_length / 2, gait_length / 2);
animsubplt.set_zlim3d(-gait_height / 2, gait_height / 2);
animsubplt.set_xlabel('X')
animsubplt.set_ylabel('Y')
animsubplt.set_zlabel('H')

# Create sliders
axcolor = 'lightgoldenrodyellow'
xmove = plt.axes([0.25, 0.15, 0.5, 0.03], axisbg=axcolor)
ymove = plt.axes([0.25, 0.1, 0.5, 0.03], axisbg=axcolor)
fmove = plt.axes([0.25, 0.05, 0.5, 0.03], axisbg=axcolor)

sxmove = Slider(xmove, 'X', -1.0, 1.0, valinit=mx)
symove = Slider(ymove, 'Y', -1.0, 1.0, valinit=my)
sfmove = Slider(fmove, 'F', 0, 2.0, valinit=freq)

# Update data function
def update(val):
    mx = sxmove.val;
    my = symove.val;
    
    m = (mx, my, mr);

    # Generate new data
    for i in range(0, len(phase)):
        x[i] = fx(m, phase[i])
        y[i] = fy(m, phase[i])
        h[i] = fh(m, phase[i])

    # Attach to plots
    xplot.set_data(phase, x)
    yplot.set_data(phase, y)
    hplot.set_data(phase, h)
    
    xyhplot.set_data(x, y);
    xyhplot.set_3d_properties(h)

sxmove.on_changed(update)
symove.on_changed(update)

def update_anim(frame):
    animplot.set_data([0, x[frame]], [0, y[frame]]);
    animplot.set_3d_properties([0, h[frame]])


line_ani = animation.FuncAnimation(fig1, update_anim, len(phase),
                                   interval=10, blit=False)

update(0)

plt.show();


