
#include "hexapod/hexapod.h"

#include <stdint.h>
#include <math.h>


void HPOD_init(struct hexapod_s* hexapod, float width, float length, float offset_a, float len_ab, float len_bc)
{
    hexapod->width = width;
    hexapod->length = length;
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
 * Adds planar rotation theta at joint A (offset by hexapod.offset_a)
 * X direction is outwards from the hexapod, Y is forwards and backward
 * H is offset from zero (in line) position
 */
void HPOD_leg_ik3(struct hexapod_s* hexapod, float x, float y, float h,
                  float* alpha, float* beta, float* theta)
{
    // Calculate distance and angle from origin to point (x, y)
    float len_xy = sqrt(pow(x, 2) + pow(y, 2));
    float angle_xy = atan(y / x);

    // Process ik2 equation with total distance (less offset between joints at A)
    HPOD_leg_ik2(hexapod, len_xy - hexapod->offset_a, h, alpha, beta);

    // Output angle theta
    *theta = angle_xy;
}

/**
 * 2 Joint Arm Forward Kinematics
 * Calculates the position in space from a given control tuple
 * X direction is outwards from the hexapod,
 * H is offset from zero (in line) position
 */
void HPOD_leg_fk2(struct hexapod_s* hexapod, float alpha, float beta,
                 float* x, float* h)
{
    // Joint B position
    float b_x = hexapod->len_ab * cos(alpha);
    float b_h = hexapod->len_ab * sin(alpha);

    // Joint C position
    float world_beta = M_PI - alpha - beta;
    *x = b_x + hexapod->len_bc * cos(world_beta);
    *h = b_h - hexapod->len_bc * sin(world_beta);
}


/**
 * 3 Joint Arm Forward Kinematics
 * Calculates the position in space from a given control tuple
 * X direction is outwards from the hexapod,
 * H is offset from zero (in line) position
 * Y is forwards and backward
 */
void HPOD_leg_fk3(struct hexapod_s* hexapod, float alpha, float beta, float theta,
                 float* x, float* y, float* h)
{
    // Joint A position
    float a_x = cos(theta) * hexapod->offset_a;
    float a_y = sin(theta) * hexapod->offset_a;
    float a_h = 0;

    float c_d, c_h;

    HPOD_leg_fk2(hexapod, alpha, beta, &c_d, &c_h);

    float len_ac = sqrt(pow(c_d, 2) + pow(c_h, 2));

    *x = a_x + c_d * cos(theta);
    *y = a_y + c_d * sin(theta);
    *h = c_h;
}


/**
 * Body to joint space transformation
 * Computes rotation about alpha (x axis) and beta (y axis) with provided offsets to
 * transform locations from world space to joint space for leg IK equations
 * alpha is the body roll, beta is body pitch
 * X direction is outwards from the hexapod, Y is forwards and backward
 * H is body height from zero (sitting on XY plane)
 *
 *    Roll Compensation
 *
 *     World Frame  Local Frame        
 *       A -            A       
 *      /|             /\         
 *     / | Z          /  \         
 *    /  |           /   _\D    
 *   /___| -        /_---       
 *   B   C          B             
 *   | X |                        
 *
 *    
 *    Pitch Compensation
 *
 *    World Frame  Local Frame        
 *       E -            E       
 *      /|             /\         
 *     / | Z          /  \         
 *    /  |           /   _\H    
 *   /___| -        /_---       
 *   F   G          F             
 *   | Y |                     
 *
 */
void HPOD_body_transform_pitch(struct hexapod_s* hexapod, float roll, float pitch, int offset_x, int offset_y,
                         float x, float y, float z, float* joint_x, float* joint_y, float* joint_z)
{
    // Calculate change in Z (height) for body pitch at a given joint
    float world_z = z - offset_y * sin(pitch);

    // Calculate shared length for world and local frame
    float len_ef = sqrt(pow(y, 2) + pow(world_z, 2));

    // Calculate the require angle in the world frame
    float angle_feg = atan(y / world_z);

    // Calculate required angle in the local frame so that adb = pi/2
    float angle_feh = angle_feg + pitch;

    // Calculate local space lengths
    float len_eh = cos(angle_feh) * len_ef;     // Local Z
    float len_fh = sin(angle_feh) * len_ef;     // Local Y

    // Shift X and Y locations by body width in local frame
    *joint_x = x;
    *joint_y = len_fh;
    *joint_z = len_eh;
}

void HPOD_body_transform_roll(struct hexapod_s* hexapod, float roll, float pitch, int offset_x, int offset_y,
                         float x, float y, float z, float* joint_x, float* joint_y, float* joint_z)
{
    // Calculate change in Z (height) for body pitch at a given joint
    float world_z = z - offset_x * sin(roll);

    // Calculate shared length for world and local frame
    float len_ab = sqrt(pow(x, 2) + pow(world_z, 2));

    // Calculate the require angle in the world frame
    float angle_bac = atan(x / world_z);

    // Calculate required angle in the local frame so that adb = pi/2
    float angle_bad = angle_bac + roll;

    // Calculate local space lengths
    float len_ad = cos(angle_bad) * len_ab;     // Local Z
    float len_bd = sin(angle_bad) * len_ab;     // Local X

    // Shift X and Y locations by body width in local frame
    *joint_x = len_bd;
    *joint_y = y;
    *joint_z = len_ad;
}

void HPOD_body_transform(struct hexapod_s* hexapod, float roll, float pitch, int offset_x, int offset_y,
                         float x, float y, float z, float* joint_x, float* joint_y, float* joint_z)
{
    float _joint_x, _joint_y, _joint_z;
    HPOD_body_transform_pitch(hexapod, roll, pitch, offset_x, offset_y, x, y, z, &_joint_x, &_joint_y, &_joint_z);
    HPOD_body_transform_roll(hexapod, roll, pitch, offset_x, offset_y, _joint_x, _joint_y, _joint_z, joint_x, joint_y, joint_z);
}


void HPOD_world_roll_pitch(struct hexapod_s* hexapod, float angle, int offset,
                         float xy, float z, float* adj_xy, float* adj_z)
{
    // Calculate change in Z (height) for body pitch at a given joint
    float shifted_z = z + offset * sin(angle);

    // Convert world heights to hexapod local heights (wrt. body frame)
    *adj_z = z + offset * sin(angle);
    *adj_xy = xy + offset / cos(angle) - offset;
}


/**
 * Calculate the position of a limb for a provided gait with specified motion at a given walking phase
 * Phase is -1 to 1, with contact between -0.5 and 0.5 to help merge movements.
 */
void HPOD_gait_calc(struct hexapod_s* hexapod, struct hpod_gait_s *gait, struct hpod_vector3_s *movement,
                    float phase_scl, float* x, float* y, float* h)
{

    float phase_scl_wrapped = HPOD_WRAP_SCL(phase_scl);
    float phase_rads = HPOD_SCL_TO_RAD(phase_scl_wrapped);

    // Forward walk
    *x = sin(phase_rads) * gait->movement.x * movement->x + gait->offset.y;
    *y = sin(phase_rads) * gait->movement.y * movement->y;

    // Height morphing determined by height_scale as a fraction of the phase for the height to change over
    if (fabs(phase_scl_wrapped) > (0.5 + gait->height_scale / 2)) {
        *h = -gait->movement.z / 2 + gait->offset.z;
    } else if (fabs(phase_scl_wrapped) < (0.5 - gait->height_scale / 2)) {
        *h = gait->movement.z / 2 + gait->offset.z;
    } else {
        *h = cos((phase_scl_wrapped - gait->height_scale / 2) / gait->height_scale * M_PI)
             * gait->movement.z / 2 + gait->offset.z;
    }

    // TODO: how does rotation fit into this?
}



