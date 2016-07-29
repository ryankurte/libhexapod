

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


TEST_F(HexTest, BodyTransform)
{
    float x, y, h, alpha, beta, _x, _y, _h;

    // No change with no angle
    alpha = 0.0; beta = 0.0; x = 10; y = 20; h = 30;
    HPOD_body_transform(&hexy, alpha, beta, 0, 0, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(x, _x, FLOAT_ERROR);
    ASSERT_NEAR(y, _y, FLOAT_ERROR);
    ASSERT_NEAR(h, _h, FLOAT_ERROR);

    // H changes with angle beta (pitch)
    alpha = 0.0; beta = M_PI / 8; x = 10; y = 20; h = 30;
    HPOD_body_transform(&hexy, alpha, beta, 0, 0, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(x, _x, FLOAT_ERROR);
    ASSERT_NEAR(y, _y, FLOAT_ERROR);
    ASSERT_NEAR(h / cos(beta), _h, FLOAT_ERROR);

    // H changes with angle beta (pitch) and offset Y
    alpha = 0.0; beta = M_PI / 8; x = 10; y = 20; h = 30;
    float offset_y = - h / sin(beta);
    HPOD_body_transform(&hexy, alpha, beta, 0, offset_y, x, y, h, &_x, &_y, &_h);
    ASSERT_NEAR(x, _x, FLOAT_ERROR);
    ASSERT_NEAR(y, _y, FLOAT_ERROR);
    ASSERT_NEAR(0, _h, 1.0);

    

}
