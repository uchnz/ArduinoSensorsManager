#include <gtest/gtest.h>
#include <BMP280Arduino.h>
#include <BMP280Arduino_test.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(BMP280ArduinoTest, test_Init_Successful_getAddressReturns_correctAddress)
{
    iarduino_Pressure_BMP i2c(0x77);
    EXPECT_CALL(i2c, begin()).Times(1).WillOnce(Return(true));
    BMP280Arduino bmp(i2c);
    EXPECT_TRUE(bmp.init());

    // EXPECT_CALL(i2c, getAddress()).Times(1).WillOnce(Return(0x77));
    EXPECT_EQ(0x77, bmp.getAddress());
}

TEST_F(BMP280ArduinoTest, test_Init_fails_getAddressReturns_Zero)
{
    iarduino_Pressure_BMP i2c(0xff);
    EXPECT_CALL(i2c, begin()).Times(1).WillOnce(Return(false));
    BMP280Arduino bmp(i2c);
    EXPECT_FALSE(bmp.init());

    // EXPECT_CALL(i2c, getAddress()).Times(0);
    EXPECT_EQ(0, bmp.getAddress());
}

TEST_F(BMP280ArduinoTest, test_gettingMeasurementsWithoutInit_fails)
{
    iarduino_Pressure_BMP i2c(0x2A);
    EXPECT_CALL(i2c, begin()).Times(0);
    BMP280Arduino bmp(i2c);

    EXPECT_CALL(i2c, read(_)).Times(0);
    bmp.requestCurrentMeasurement();

    float measure = bmp.getCurrentMeasurementByID();
    EXPECT_FLOAT_EQ(-127, measure);
    measure = bmp.getCurrentMeasurementByID(1);
    EXPECT_FLOAT_EQ(-127, measure);
    measure = bmp.getCurrentMeasurementByID(2);
    EXPECT_FLOAT_EQ(-127, measure);
}

TEST_F(BMP280ArduinoTest, test_gettingTemperatureWithoutRequesting_fails)
{
    iarduino_Pressure_BMP i2c(0x2A);
    EXPECT_CALL(i2c, begin()).Times(1).WillOnce(Return(true));
    BMP280Arduino bmp(i2c);
    EXPECT_TRUE(bmp.init());

    EXPECT_CALL(i2c, read(_)).Times(0);
    float measure = bmp.getCurrentMeasurementByID();
    EXPECT_FLOAT_EQ(-127, measure);
    measure = bmp.getCurrentMeasurementByID(1);
    EXPECT_FLOAT_EQ(-127, measure);
    measure = bmp.getCurrentMeasurementByID(2);
    EXPECT_FLOAT_EQ(-127, measure);
}

TEST_F(BMP280ArduinoTest, test_gettingSensorIDAboveExisting_fails)
{
    iarduino_Pressure_BMP i2c(0x2A);
    EXPECT_CALL(i2c, begin()).Times(0);
    BMP280Arduino bmp(i2c);

    EXPECT_CALL(i2c, read(_)).Times(0);
    bmp.requestCurrentMeasurement();

    float nonexist = bmp.getCurrentMeasurementByID(4);
    EXPECT_FLOAT_EQ(-127, nonexist);
}

TEST_F(BMP280ArduinoTest, test_requestCurrentMeasurement_withinTimeInterval_SavesArrayOfMeasurements)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    iarduino_Pressure_BMP i2c(0x2A);
    EXPECT_CALL(i2c, begin()).Times(1).WillOnce(Return(true));
    BMP280Arduino bmp(i2c);
    EXPECT_TRUE(bmp.init());

    EXPECT_CALL(i2c, read()).Times(4);
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 10; i++)
    {
        i2c.temperature = i;
        i2c.pressure = i + 1.1;
        i2c.altitude = i * 2.5;
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        bmp.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_FLOAT_EQ(3, bmp.getCurrentMeasurementByID());
    EXPECT_FLOAT_EQ(4.1, bmp.getCurrentMeasurementByID(1));
    EXPECT_FLOAT_EQ(7.5, bmp.getCurrentMeasurementByID(2));
    EXPECT_EQ(-127, bmp.getCurrentMeasurementByID(3));

    incrementIntervalMillis = 0;
    nextLoopMillis = 3000;
    EXPECT_CALL(i2c, read()).Times(3);
    for (int i = 0; i < 3; i++)
    {
        i2c.temperature = i;
        i2c.pressure = i - 2.1;
        i2c.altitude = i * (-3.1);
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        bmp.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_NEAR(3.33, bmp.getCurrentMeasurementByID(), 0.01);
    EXPECT_FLOAT_EQ(2.3, bmp.getCurrentMeasurementByID(1));
    EXPECT_NEAR(6.47, bmp.getCurrentMeasurementByID(2), 0.01);
    EXPECT_EQ(-127, bmp.getCurrentMeasurementByID(4));
    releaseArduinoMock();
}

TEST_F(BMP280ArduinoTest, test_requestCurrentMeasurement_withDifferentMeasurementIntervals_SavesArrayOfMeasurements)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    iarduino_Pressure_BMP i2c(0x7A);
    EXPECT_CALL(i2c, begin()).Times(1).WillOnce(Return(true));
    BMP280Arduino bmp(i2c);
    EXPECT_TRUE(bmp.init(1000));

    EXPECT_CALL(i2c, read()).Times(4);
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 4; i++)
    {
        i2c.temperature = i;
        i2c.pressure = i - 2.1;
        i2c.altitude = i * (-3.1);
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        bmp.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_FLOAT_EQ(1, bmp.getCurrentMeasurementByID());
    EXPECT_FLOAT_EQ(-1.1, bmp.getCurrentMeasurementByID(1));
    EXPECT_FLOAT_EQ(-3.1, bmp.getCurrentMeasurementByID(2));

    EXPECT_CALL(i2c, begin()).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(bmp.init(2000));
    incrementIntervalMillis = 0;
    nextLoopMillis = 3000;
    EXPECT_CALL(i2c, read()).Times(4);
    for (int i = 0; i < 4; i++)
    {
        i2c.temperature = i;
        i2c.pressure = i - 2.1;
        i2c.altitude = i * (-3.1);
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        bmp.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_NEAR(1.33, bmp.getCurrentMeasurementByID(), 0.01);
    EXPECT_NEAR(-0.77, bmp.getCurrentMeasurementByID(1), 0.01);
    EXPECT_NEAR(-4.13, bmp.getCurrentMeasurementByID(2), 0.01);
    releaseArduinoMock();
}