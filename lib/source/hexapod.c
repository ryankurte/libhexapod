
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
    float angle_a = acos((pow(len_ac, 2) + pow(hexapod->len_ab, 2) - pow(hexapod->len_bc, 2))
                         / (2 * len_ac * hexapod->len_ab));
    float angle_b = acos((pow(hexapod->len_ab, 2) + pow(hexapod->len_bc, 2) - pow(len_ac, 2))
                         / (2 * hexapod->len_ab * hexapod->len_bc));

    // Convert back into world frame
    *alpha = angle_a + angle_dh;
    *beta = angle_b;
}

/**
 * 3 Joint Arm Inverse Kinematics
 * Adds planar rotation Omega at joint A (offset by hexapod.offset_a)
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
 * 3 Joint Arm Forward Kinematics
 * Calculates the position in space from a given control tuple
 * X direction is outwards from the hexapod, Y is forwards and backward
 * H is offset from zero (in line) position
 */
void HPOD_leg_fk(struct hexapod_s* hexapod, float alpha, float beta, float omega,
                 float* x, float* y, float* h)
{
    // Joint A position
    float a_x = cos(omega) * hexapod->offset_a;
    float a_y = sin(omega) * hexapod->offset_a;
    float a_h = 0;

    // Joint B position
    float len_ab_xy = cos(alpha) * hexapod->len_ab;
    float b_x = a_x + cos(omega) * len_ab_xy;
    float b_y = a_y + sin(omega) * len_ab_xy;
    float b_h = a_h + sin(alpha) * hexapod->len_ab;

    // Joint C position
    float world_beta = alpha + beta - M_PI;
    float len_bc_xy = cos(world_beta) * hexapod->len_bc;
    *x = b_x + cos(omega) * len_bc_xy;
    *y = b_y + sin(omega) * len_bc_xy;
    *h = b_h + sin(world_beta) * hexapod->len_bc;
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

    // Forward walk
    *x = sin(phase_rads) * gait->movement_width * movement->x + gait->offset_width;
    *y = sin(phase_rads) * gait->movement_length * movement->y;

    // Height morphing determined by height_scale as a fraction of the phase for the height to change over
    if (abs(phase_scl_wrapped) > (0.5 + gait->height_scale / 2)) {
        *h = -gait->movement_height / 2 + gait->offset_height;
    } else if (abs(phase_scl_wrapped) < (0.5 - gait->height_scale / 2)) {
        *h = gait->movement_height / 2 + gait->offset_height;
    } else {
        *h = cos((phase_scl_wrapped - gait->height_scale / 2) / gait->height_scale * M_PI)
             * gait->movement_height / 2 + gait->offset_height;
    }

    // TODO: how does rotation fit into this?
}



