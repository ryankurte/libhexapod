
#include "hexapod/hexapod.h"

#include <stdint.h>
#include <math.h>

void HPOD_init(struct hexapod_s* hexapod, float width, float offset_a, float len_ab, float len_bc)
{
    hexapod->width = width;
    hexapod->offset_a = offset_a;
    hexapod->len_ab = len_ab;
    hexapod->len_bc = len_bc;
}

/**
 *  2 Joint Arm Inverse Kinematics
 *  Joints at A and B, Arm end at point C
 *  Displacement (from A) represented by d(istance) and h(eight)
 *  Angle outputs are alpha for A (wrt. horizontal plane) and beta for B
 *  (local space)
 *
 *    2D Original          Rotated Flat
 *       B                    B   B      _
 *       /\                  /|   |\
 *      /  \                / |   | \      opposite
 *     /    \              /  |   |  \
 *    /      \   _        /___|   |___\  _
 *   A        \    h     A    X   X    C
 *             C _       |    |   |    |
 *   |         |           x1       x2
 *        d
 *
 */
void HPOD_leg_ik2(struct hexapod_s* hexapod, float d, float h, float* alpha, float* beta)
{

    // Calculate length between A & C
    float len_ac = sqrt(pow(d, 2) + pow(h, 2));

    // Calculate rotational offset from zero frame (h = 0)
    float angle_dh = atan(h / d);

    // Split into two regular triangles & calculate length of shared face
    // a^2 + b^2 = c^2 & d = len_ax + len_cx
    //   -> d^2 = len_ab^2 - len_bx^2 + len_bc^2 - len_bx^2
    //   -> len_bx^2 = - (d^2 - len_ab^2 - len_bc^2) / 2
    float opposite = -sqrt((pow(len_ac, 2) - pow(hexapod->len_ab, 2) - pow(hexapod->len_bc, 2)) / 2);

    // Calculate angles
    float angle_a = acos(opposite / hexapod->len_ab);
    float angle_c = acos(opposite / hexapod->len_bc);
    float angle_b = M_PI - angle_a - angle_c;

    // Convert back into world frame
    *alpha = angle_a + angle_dh;
    *beta = angle_c;
}


/**
 * 3 Joint Arm Inverse Kinematics
 * Adds planar rotation at joint A
 * X direction is outwards from the hexapod, Y is forwards and backward
 * H is offset from zero (in line) position
 */
void HPOD_leg_ik3(struct hexapod_s* hexapod, float x, float y, float h,
                  float* alpha, float* beta, float* omega)
{
    // Calculate distance and angle from origin to point (x, y)
    float len_xy = sqrt(pow(x, 2) + pow(y, 2));
    float angle_xy = atan(y / x);

    // Process ik2 equation with total distance (less offset between joints at A)
    HPOD_leg_ik2(hexapod, len_xy - hexapod->offset_a, h, alpha, beta);

    // Output angle omega
    *omega = angle_xy;
}

/**
 * Calculate the position of a limb for a provided gait with specified motion at a given walking phase
 * Phase is -1 to 1, with contact between -0.5 and 0.5 to help merge movements.
 */
void HPOD_gait_calc(struct hexapod_s* hexapod, struct hpod_gait_s *gait, struct hpod_vector_s *movement,
                    float phase_scl, float* x, float* y, float* h)
{

    float phase_scl_wrapped = HPOD_WRAP_SCL(phase_scl);
    float phase_rads = HPOD_SCL_TO_RAD(phase_scl_wrapped);

    // Forward or reverse motion achieved by inverting the phase of the underlying sinusoid
    int phase_invert_y = (movement->y >= 0) ? -1 : 1;
    int phase_invert_x = (movement->x >= 0) ? -1 : 1;

    // Forward walk
    *x = sin(phase_rads) * gait->movement_width * movement->x * phase_invert_x + gait->offset_width;
    *y = sin(phase_rads) * gait->movement_length * movement->y * phase_invert_y;
    *h = ((fabs(phase_rads) < M_PI / 2) ? gait->movement_height : -gait->movement_height) + gait->offset_height;

    // TODO: how does rotation fit into this?
}




