/**
 * Libhexapod
 * Hexapod Unit Tests
 *
 * https://github.com/ryankurte/libhexapod
 * Copyright 2017 Ryan Kurte
 */

#include "gtest/gtest.h"

#include <stdio.h>
#include <math.h>

#include "hexapod/servo.h"

#define RANGE_RADS (300.0 / 180.0 * M_PI)

class ServoTest : public ::testing::Test
{
protected:
    ServoTest()
    {
        HPOD_servo_init(&servo, range_rads, output_range, output_offset);
    }

    virtual ~ServoTest()
    {
    }

    struct hpod_servo_s servo;
    float range_rads = RANGE_RADS;
    int output_range  = 1024;
    int output_offset = 512;
};


TEST_F(ServoTest, Center)
{
    int output;

    output = HPOD_servo_scale(&servo, 0);
    ASSERT_EQ(output, output_offset);
}

TEST_F(ServoTest, FullLeft)
{
    int output = HPOD_servo_scale(&servo, RANGE_RADS / 2);
    ASSERT_EQ(output, 1024);
}

TEST_F(ServoTest, FullRight)
{
    int output = HPOD_servo_scale(&servo, -RANGE_RADS / 2);
    ASSERT_EQ(output, 0);
}

TEST_F(ServoTest, HalfLeft)
{
    int output = HPOD_servo_scale(&servo, RANGE_RADS / 4);
    ASSERT_EQ(output, 768);
}

TEST_F(ServoTest, HalfRight)
{
    int output = HPOD_servo_scale(&servo, -RANGE_RADS / 4);
    ASSERT_EQ(output, 256);
}

