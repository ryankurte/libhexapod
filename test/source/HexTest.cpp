

#include "gtest/gtest.h"

#include "hexapod/hexapod.h"

#include <stdio.h>

class HexTest : public ::testing::Test
{
protected:
    HexTest()
    {
        hexy.length = 200;
        hexy.width = 100;
        hexy.offset_a = 20;
        hexy.len_ab = 80;
        hexy.len_bc = 100;
    }

    virtual ~HexTest()
    {

    }
    struct hexapod_s hexy;
};

#define SWEEP_SIZE      100
#define FLOAT_ERROR     0.01

TEST_F(HexTest, IK2SweepX)
{
    float alpha, beta, _x, _h;

    float h_error = 0.0;
    float x_error = 0.0;

    float h = 0;

    for (int i = 0; i < SWEEP_SIZE; i++) {
        float x = hexy.offset_a + 10 + hexy.len_bc / SWEEP_SIZE * i;

        HPOD_leg_ik2(&hexy, x, h, &alpha, &beta);
        HPOD_leg_fk2(&hexy, alpha, beta, &_x, &_h);
        //printf("X: %f (%f) H: %f (%f)\r\n", _x, x, _h, h);

        ASSERT_NEAR(h, _h, FLOAT_ERROR);
        ASSERT_NEAR(x, _x, FLOAT_ERROR);
    }
}

TEST_F(HexTest, IK2SweepH)
{
    float alpha, beta, _x, _h;

    float h_error = 0.0;
    float x_error = 0.0;

    float x = hexy.len_ab + hexy.len_bc / 2;

    for (int i = 0; i < SWEEP_SIZE; i++) {
        float h = -hexy.len_bc / 4 + hexy.len_bc / 2 / SWEEP_SIZE * i;

        HPOD_leg_ik2(&hexy, x, h, &alpha, &beta);
        HPOD_leg_fk2(&hexy, alpha, beta, &_x, &_h);
        //printf("X: %f (%f) H: %f (%f)\r\n", _x, x, _h, h);

        ASSERT_NEAR(h, _h, FLOAT_ERROR);
        ASSERT_NEAR(x, _x, FLOAT_ERROR);
    }
}

TEST_F(HexTest, IK2SweepXH)
{
    float alpha, beta, _x, _h;

    float h_error = 0.0;
    float x_error = 0.0;

    for (int i = 0; i < SWEEP_SIZE; i++) {
        float x = hexy.offset_a + 10 + hexy.len_bc / SWEEP_SIZE * i;
        float h = -hexy.len_bc / 4 + hexy.len_bc / 2 / SWEEP_SIZE * i;

        HPOD_leg_ik2(&hexy, x, h, &alpha, &beta);
        HPOD_leg_fk2(&hexy, alpha, beta, &_x, &_h);
        //printf("X: %f (%f) H: %f (%f)\r\n", _x, x, _h, h);

        ASSERT_NEAR(h, _h, FLOAT_ERROR);
        ASSERT_NEAR(x, _x, FLOAT_ERROR);
    }
}

TEST_F(HexTest, IK3SweepY)
{
    float alpha, beta, theta, _x, _y, _h;

    float h_error = 0.0;
    float x_error = 0.0;
    float y_error = 0.0;

    float h = 0;
    float x = hexy.len_ab + hexy.len_bc / 2;

    for (int i = 0; i < SWEEP_SIZE; i++) {
        float y = -hexy.length / 4 + hexy.length / 2 / SWEEP_SIZE * i;

        HPOD_leg_ik3(&hexy, x, y, h, &alpha, &beta, &theta);
        HPOD_leg_fk3(&hexy, alpha, beta, theta, &_x, &_y, &_h);
        //printf("X: %f (%f) Y: %f (%f) H: %f (%f)\r\n", _x, x, _y, y, _h, h);

        ASSERT_NEAR(h, _h, FLOAT_ERROR);
        ASSERT_NEAR(x, _x, FLOAT_ERROR);
        ASSERT_NEAR(y, _y, FLOAT_ERROR);
    }
}

void HPOD_body_transform(struct hexapod_s* hexapod, float alpha, float beta, int offset_x, int offset_y,
                         float x, float y, float z, float* joint_x, float* joint_y, float* joint_z);


TEST_F(HexTest, BodyTransformNeutral)
{
    float x, y, h, roll, pitch, _x, _y, _h;

    // No change with no angle
    roll = 0.0; pitch = 0.0; x = 10; y = 20; h = 30;
    HPOD_body_transform(&hexy, roll, pitch, 0, 0, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(x, _x, FLOAT_ERROR);
    ASSERT_NEAR(y, _y, FLOAT_ERROR);
    ASSERT_NEAR(h, _h, FLOAT_ERROR);
}

TEST_F(HexTest, BodyTransformPitch)
{
    float x, y, h, roll, pitch, _x, _y, _h;

    // H and Y change with pitch where Y is zero
    roll = 0.0; pitch = M_PI / 8; x = 10; y = 0; h = 30;
    HPOD_body_transform(&hexy, roll, pitch, 0, 0, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(x, _x, FLOAT_ERROR);
    ASSERT_NEAR(h * sin(pitch), _y, FLOAT_ERROR);
    ASSERT_NEAR(h * cos(pitch), _h, FLOAT_ERROR);

    // H changes with pitch and offset Y where Y is zero
    roll = 0.0; pitch = M_PI / 8; x = 10; y = 0; h = 30;
    float offset_y = 30;
    HPOD_body_transform(&hexy, roll, pitch, 0, offset_y, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(x, _x, FLOAT_ERROR);
    ASSERT_NEAR((h - sin(pitch) * offset_y) * sin(pitch), _y, FLOAT_ERROR);
    ASSERT_NEAR((h - sin(pitch) * offset_y) * cos(pitch), _h, FLOAT_ERROR);

    // H and Y change where Y is non-zero with no offset
    roll = 0.0; pitch = M_PI / 8; x = 10; y = 10; h = 30;
    HPOD_body_transform(&hexy, roll, pitch, 0, 0, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(x, _x, FLOAT_ERROR);

    //TODO: more tests
}

TEST_F(HexTest, BodyTransformRoll)
{
    float x, y, h, roll, pitch, _x, _y, _h;

    // H and X change with roll where X is zero
    roll = M_PI / 8; pitch = 0.0; x = 0; y = 10; h = 30;
    HPOD_body_transform(&hexy, roll, pitch, 0, 0, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(y, _y, FLOAT_ERROR);
    ASSERT_NEAR(h * sin(roll), _x, FLOAT_ERROR);
    ASSERT_NEAR(h * cos(roll), _h, FLOAT_ERROR);

    // H changes with pitch and offset X where X is zero
    roll = M_PI / 8; pitch = 0.0; x = 0; y = 10; h = 30;
    float offset_x = 30;
    HPOD_body_transform(&hexy, roll, pitch, offset_x, 0, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(y, _y, FLOAT_ERROR);
    ASSERT_NEAR((h - sin(roll) * offset_x) * sin(roll), _x, FLOAT_ERROR);
    ASSERT_NEAR((h - sin(roll) * offset_x) * cos(roll), _h, FLOAT_ERROR);

    // H and Y change where Y is non-zero with no offset
    roll = M_PI / 8; pitch = 0.0; x = 10; y = 10; h = 30;
    HPOD_body_transform(&hexy, roll, pitch, 0, 0, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(y, _y, FLOAT_ERROR);

}


TEST_F(HexTest, DISABLED_BodyTransformPitchRoll)
{
    float x, y, h, roll, pitch, _x, _y, _h;

    // X, Y and H change with roll with zero offsets
    roll = M_PI / 8; pitch = M_PI / 9; x = 0; y = 10; h = 30;
    HPOD_body_transform(&hexy, roll, pitch, 0, 0, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(y * sin(pitch), _y, FLOAT_ERROR);
    ASSERT_NEAR(h * sin(roll), _x, FLOAT_ERROR);
    ASSERT_NEAR(h * cos(roll) * cos(pitch), _h, FLOAT_ERROR);

    // H changes with pitch and offset Y where Y is zero
    roll = 0.0; pitch = M_PI / 8; x = 10; y = 0; h = 30;
    float offset_y = 30;
    HPOD_body_transform(&hexy, roll, pitch, 0, offset_y, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(x, _x, FLOAT_ERROR);
    ASSERT_NEAR((h - sin(pitch) * offset_y) * sin(pitch), _y, FLOAT_ERROR);
    ASSERT_NEAR((h - sin(pitch) * offset_y) * cos(pitch), _h, 1.0);

    // H and Y change where Y is non-zero with no offset
    roll = 0.0; pitch = M_PI / 8; x = 10; y = 10; h = 30;
    HPOD_body_transform(&hexy, roll, pitch, 0, 0, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(x, _x, FLOAT_ERROR);
}

