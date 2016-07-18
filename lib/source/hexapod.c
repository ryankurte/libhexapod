

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
void HEX_leg_ik2(float d, float h, float* alpha, float* beta)
{

    // Calculate length between A & C
    float len_ac = sqrt(pow(d, 2), pow(h, 2));

    // Calculate rotational offset from zero frame (h = 0)
    float angle_dh = atan(h / d);

    // Split into two regular triangles & calculate length of shared face
    // a^2 + b^2 = c^2 & d = len_ax + len_cx
    //   -> d^2 = len_ab^2 - len_bx^2 + len_bc^2 - len_bx^2
    //   -> len_bx^2 = - (d^2 - len_ab^2 - len_bc^2) / 2
    float opposite = -sqrt((pow(len_ac, 2) - pow(len_ab, 2) - pow(len_bc, 2)) / 2);

    // Calculate angles
    float angle_a = acos(opposite / len_ab);
    float angle_c = acos(opposite / len_bc);
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
void HEX_leg_ik3(float x, float y, float h, float* alpha, float* beta, float* omega)
{
    // Calculate distance and angle from origin to point (x, y)
    float len_xy = sqrt(pow(x, 2), pow(y, 2));
    float angle_xy = atan(y / x);

    // Process ik2 equation with total distance
    HEX_leg_ik2(len_xy, h, alpha, beta);

    // Output angle omega
    *omega = angle_xy;
}

