

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


void HPOD_leg_ik3(struct hexapod_s* hexapod, float x, float y, float h,
                  float* alpha, float* beta, float* theta);

void HPOD_leg_fk(struct hexapod_s* hexapod, float alpha, float beta, float theta,
                 float* x, float* y, float* h);

void HPOD_leg_ik2(struct hexapod_s* hexapod, float d, float h, float* alpha, float* beta);

#define SWEEP_SIZE      10

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

        x_error += (x - _x);
        h_error += (h - _h);
    }

    ASSERT_EQ(true, h_error < 0.1);
    ASSERT_EQ(true, x_error < 0.1);
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

        h_error += (h - _h);
        x_error += (x - _x);
    }

    ASSERT_EQ(true, h_error < 0.1);
    ASSERT_EQ(true, x_error < 0.1);
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

        h_error += (h - _h);
        x_error += (x - _x);
    }

    ASSERT_EQ(true, h_error < 0.1);
    ASSERT_EQ(true, x_error < 0.1);
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
        HPOD_leg_fk3(&hexy, alpha, beta, theta, &_x, &y, &_h);
        //printf("X: %f (%f) Y: %f (%f) H: %f (%f)\r\n", _x, x, _y, y, _h, h);

        x_error += (x - _x);
        y_error += (y - _y);
        h_error += (h - _h);
    }

    ASSERT_EQ(true, h_error < 0.1);
    ASSERT_EQ(true, x_error < 0.1);
    ASSERT_EQ(true, y_error < 0.1);
}

