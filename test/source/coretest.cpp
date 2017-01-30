/**
 * Libhexapod
 * Hexapod Unit Tests
 *
 * https://github.com/ryankurte/libhexapod
 * Copyright 2017 Ryan Kurte
 */

#include "gtest/gtest.h"

#include <stdio.h>

#include "hexapod/hexapod.h"

class HexTest : public ::testing::Test
{
protected:
    HexTest()
    {
        struct hexapod_config_s config = HPOD_DEFAULT_CONFIG;
        HPOD_init(&hexy, &config);
    }

    virtual ~HexTest()
    {

    }
    struct hexapod_s hexy;
};

TEST_F(HexTest, ValidGait)
{
    struct hpod_gait_s gait = HPOD_DEFAULT_GAIT;
    int res = HPOD_gait_valid(&hexy, &gait);
    ASSERT_EQ(res, 0);
}

#define SWEEP_SIZE      100
#define FLOAT_ERROR     0.01

TEST_F(HexTest, DISABLED_IK2SweepX)
{
    //TODO: make this test less fragile
    float alpha, beta, _x, _h;

    float h_error = 0.0;
    float x_error = 0.0;

    float h = 0;

    for (int i = 0; i < SWEEP_SIZE; i++) {
        float x = hexy.config.offset_a + 10 + hexy.config.len_bc / SWEEP_SIZE * i;

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

    float x = hexy.config.len_ab + hexy.config.len_bc / 2;

    for (int i = 0; i < SWEEP_SIZE; i++) {
        float h = -hexy.config.len_bc / 4 + hexy.config.len_bc / 2 / SWEEP_SIZE * i;

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
        float x = hexy.config.offset_a + 10 + hexy.config.len_bc / SWEEP_SIZE * i;
        float h = -hexy.config.len_bc / 4 + hexy.config.len_bc / 2 / SWEEP_SIZE * i;

        HPOD_leg_ik2(&hexy, x, h, &alpha, &beta);
        HPOD_leg_fk2(&hexy, alpha, beta, &_x, &_h);
        //printf("X: %f (%f) H: %f (%f)\r\n", _x, x, _h, h);

        ASSERT_NEAR(h, _h, FLOAT_ERROR);
        ASSERT_NEAR(x, _x, FLOAT_ERROR);
    }
}

TEST_F(HexTest, IK3SweepY)
{
    float alpha, beta, theta;
    struct hpod_vector3_s desired_pos;
    struct hpod_vector3_s actual_pos;

    float h_error = 0.0;
    float x_error = 0.0;
    float y_error = 0.0;

    desired_pos.z = 0;
    desired_pos.x = hexy.config.len_ab + hexy.config.len_bc / 2;

    for (int i = 0; i < SWEEP_SIZE; i++) {
        float y = -hexy.config.length / 4 + hexy.config.length / 2 / SWEEP_SIZE * i;

        HPOD_leg_ik3(&hexy, &desired_pos, &alpha, &beta, &theta);
        HPOD_leg_fk3(&hexy, alpha, beta, theta, &actual_pos);

        ASSERT_NEAR(desired_pos.z, actual_pos.z, FLOAT_ERROR);
        ASSERT_NEAR(desired_pos.x, actual_pos.x, FLOAT_ERROR);
        ASSERT_NEAR(desired_pos.y, actual_pos.y, FLOAT_ERROR);
    }
}


TEST_F(HexTest, BodyTransformNeutral)
{
    float roll, pitch;
    struct hpod_vector3_s world_pos;
    struct hpod_vector3_s joint_pos;

    // No change with no angle
    roll = 0.0; pitch = 0.0;
    world_pos.x = 10; world_pos.y = 20; world_pos.z = 30;

    HPOD_body_transform(&hexy, roll, pitch, 0, 0, &world_pos, &joint_pos);
    ASSERT_NEAR(world_pos.x, joint_pos.x, FLOAT_ERROR);
    ASSERT_NEAR(world_pos.y, joint_pos.y, FLOAT_ERROR);
    ASSERT_NEAR(world_pos.z, joint_pos.z, FLOAT_ERROR);
}

TEST_F(HexTest, BodyTransformPitch)
{
    float roll, pitch;
    struct hpod_vector3_s world_pos;
    struct hpod_vector3_s joint_pos;


    // H and Y change with pitch where Y is zero
    roll = 0.0; pitch = M_PI / 8;
    world_pos.x = 10; world_pos.y = 0; world_pos.z = 30;

    HPOD_body_transform(&hexy, roll, pitch, 0, 0, &world_pos, &joint_pos);

    ASSERT_NEAR(world_pos.x, joint_pos.x, FLOAT_ERROR);
    ASSERT_NEAR(world_pos.z * sin(pitch), joint_pos.y, FLOAT_ERROR);
    ASSERT_NEAR(world_pos.z * cos(pitch), joint_pos.z, FLOAT_ERROR);

    // H changes with pitch and offset Y where Y is zero
    roll = 0.0; pitch = M_PI / 8;
    float offset_y = 30;
    world_pos.x = 10; world_pos.y = 0; world_pos.z = 30;

    HPOD_body_transform(&hexy, roll, pitch, 0, offset_y, &world_pos, &joint_pos);

    ASSERT_NEAR(world_pos.x, joint_pos.x, FLOAT_ERROR);
    ASSERT_NEAR((world_pos.z - sin(pitch) * offset_y) * sin(pitch), joint_pos.y, FLOAT_ERROR);
    ASSERT_NEAR((world_pos.z - sin(pitch) * offset_y) * cos(pitch), joint_pos.z, FLOAT_ERROR);

    // H and Y change where Y is non-zero with no offset
    roll = 0.0; pitch = M_PI / 8;
    world_pos.x = 10; world_pos.y = 10; world_pos.z = 30;

    HPOD_body_transform(&hexy, roll, pitch, 0, 0, &world_pos, &joint_pos);

    ASSERT_NEAR(world_pos.x, joint_pos.x, FLOAT_ERROR);

    //TODO: more tests
}

TEST_F(HexTest, BodyTransformRoll)
{
    float roll, pitch;
    struct hpod_vector3_s world_pos;
    struct hpod_vector3_s joint_pos;

    // H and X change with roll where X is zero
    roll = M_PI / 8; pitch = 0.0;
    world_pos.x = 0; world_pos.y = 10; world_pos.z = 30;

    HPOD_body_transform(&hexy, roll, pitch, 0, 0, &world_pos, &joint_pos);

    ASSERT_NEAR(world_pos.y, joint_pos.y, FLOAT_ERROR);
    ASSERT_NEAR(world_pos.z * sin(roll), joint_pos.x, FLOAT_ERROR);
    ASSERT_NEAR(world_pos.z * cos(roll), joint_pos.z, FLOAT_ERROR);

    // H changes with pitch and offset X where X is zero
    roll = M_PI / 8; pitch = 0.0;
    world_pos.x = 0; world_pos.y = 10; world_pos.z = 30;
    float offset_x = 30;

    HPOD_body_transform(&hexy, roll, pitch, offset_x, 0, &world_pos, &joint_pos);

    ASSERT_NEAR(world_pos.y, joint_pos.y, FLOAT_ERROR);
    ASSERT_NEAR((world_pos.z - sin(roll) * offset_x) * sin(roll), joint_pos.x, FLOAT_ERROR);
    ASSERT_NEAR((world_pos.z - sin(roll) * offset_x) * cos(roll), joint_pos.z, FLOAT_ERROR);

    // H and Y change where Y is non-zero with no offset
    roll = M_PI / 8; pitch = 0.0;
    world_pos.x = 10; world_pos.y = 10; world_pos.z = 30;

    HPOD_body_transform(&hexy, roll, pitch, 0, 0, &world_pos, &joint_pos);

    ASSERT_NEAR(world_pos.y, joint_pos.y, FLOAT_ERROR);
}


TEST_F(HexTest, DISABLED_BodyTransformPitchRoll)
{
    float roll, pitch;
    struct hpod_vector3_s world_pos;
    struct hpod_vector3_s joint_pos;

    // X, Y and H change with roll with zero offsets
    roll = M_PI / 8; pitch = M_PI / 9;
    world_pos.x = 0; world_pos.y = 10; world_pos.z = 30;

    HPOD_body_transform(&hexy, roll, pitch, 0, 0, &world_pos, &joint_pos);

    ASSERT_NEAR(world_pos.y * sin(pitch), joint_pos.y, FLOAT_ERROR);
    ASSERT_NEAR(world_pos.z * sin(roll), joint_pos.x, FLOAT_ERROR);
    ASSERT_NEAR(world_pos.z * cos(roll) * cos(pitch), joint_pos.z, FLOAT_ERROR);

    // H changes with pitch and offset Y where Y is zero
    roll = 0.0; pitch = M_PI / 8;
    world_pos.x = 10; world_pos.y = 0; world_pos.z = 30;
    float offset_y = 30;

    HPOD_body_transform(&hexy, roll, pitch, 0, offset_y, &world_pos, &joint_pos);

    ASSERT_NEAR(world_pos.x, joint_pos.x, FLOAT_ERROR);
    ASSERT_NEAR((world_pos.z - sin(pitch) * offset_y) * sin(pitch), joint_pos.y, FLOAT_ERROR);
    ASSERT_NEAR((world_pos.z - sin(pitch) * offset_y) * cos(pitch), joint_pos.z, 1.0);

    // H and Y change where Y is non-zero with no offset
    roll = 0.0; pitch = M_PI / 8;
    world_pos.x = 10; world_pos.y = 10; world_pos.z = 30;

    HPOD_body_transform(&hexy, roll, pitch, 0, 0, &world_pos, &joint_pos);

    ASSERT_NEAR(world_pos.x, joint_pos.x, FLOAT_ERROR);
}

