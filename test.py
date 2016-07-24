import numpy as np
import matplotlib.pyplot as plt


gait_width = 10;
gait_length = 10;
gait_height = 10;

phase = np.arange(-1, 1, 0.1)

def fx(p):
    return gait_width

def fy(p):
    return np.sin(p * np.pi) * gait_length;

def fh(p):
    if(np.abs(p * np.pi) > (np.pi / 2)):
        return gait_height / 2;
    else:
        return -gait_height / 2;


fig1 = plt.figure()

x = [0] * len(phase)
y = [0] * len(phase)
h = [0] * len(phase)

for i in range(0, len(phase)):
    x[i] = fx(phase[i])
    y[i] = fy(phase[i])
    h[i] = fh(phase[i])


plt.subplot(311)
plt.title('X')
plt.plot(phase, x);

plt.subplot(312)
plt.title('Y')
plt.plot(phase, y);

plt.subplot(313)
plt.title('H')
plt.plot(phase, h);

plt.show();


