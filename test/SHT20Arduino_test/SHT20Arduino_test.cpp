#include <gtest/gtest.h>
#include <SHT20Arduino.h>
#include <SHT20Arduino_test.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(SHT20ArduinoTest, test_Init_Successful_getAddressReturns_correctAddress)
{
    iarduino_I2C_SHT i2c(0x3f);
    EXPECT_CALL(i2c, begin()).Times(1).WillOnce(Return(true));
    SHT20Arduino sht(i2c);
    EXPECT_TRUE(sht.init());

    EXPECT_CALL(i2c, getAddress()).Times(1).WillOnce(Return(0x3f));
    EXPECT_EQ(0x3f, sht.getAddress());
}

TEST_F(SHT20ArduinoTest, test_Init_fails_getAddressReturns_Zero)
{
    iarduino_I2C_SHT i2c(0xff);
    EXPECT_CALL(i2c, begin()).Times(1).WillOnce(Return(false));
    SHT20Arduino sht(i2c);
    EXPECT_FALSE(sht.init());

    EXPECT_CALL(i2c, getAddress()).Times(0);
    EXPECT_EQ(0, sht.getAddress());
}

TEST_F(SHT20ArduinoTest, test_gettingMeasurementsWithoutInit_fails)
{
    iarduino_I2C_SHT i2c(0x2A);
    EXPECT_CALL(i2c, begin()).Times(0);
    SHT20Arduino sht(i2c);

    EXPECT_CALL(i2c, getTem()).Times(0);
    sht.requestCurrentMeasurement();

    float measurement = sht.getCurrentMeasurementByID();
    EXPECT_FLOAT_EQ(-127, measurement);
    measurement = sht.getCurrentMeasurementByID(1);
    EXPECT_FLOAT_EQ(-127, measurement);
}

TEST_F(SHT20ArduinoTest, test_gettingSensorIDAboveExisting_fails)
{
    iarduino_I2C_SHT i2c(0x2A);
    EXPECT_CALL(i2c, begin()).Times(0);
    SHT20Arduino sht(i2c);

    EXPECT_CALL(i2c, getTem()).Times(0);
    EXPECT_CALL(i2c, getHum()).Times(0);
    sht.requestCurrentMeasurement();

    float nonexist = sht.getCurrentMeasurementByID(5);
    EXPECT_FLOAT_EQ(-127, nonexist);
}

TEST_F(SHT20ArduinoTest, test_gettingTemperatureWithoutRequesting_fails)
{
    iarduino_I2C_SHT i2c(0x2A);
    EXPECT_CALL(i2c, begin()).Times(1).WillOnce(Return(true));
    SHT20Arduino sht(i2c);
    EXPECT_TRUE(sht.init());

    EXPECT_CALL(i2c, getTem()).Times(0);
    EXPECT_CALL(i2c, getHum()).Times(0);
    float measurement = sht.getCurrentMeasurementByID();
    EXPECT_FLOAT_EQ(-127, measurement);
    measurement = sht.getCurrentMeasurementByID(1);
    EXPECT_FLOAT_EQ(-127, measurement);
}

TEST_F(SHT20ArduinoTest, test_requestCurrentMeasurement_withinTimeInterval_SavesArrayOfMeasurements)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    iarduino_I2C_SHT i2c(0x2A);
    EXPECT_CALL(i2c, begin()).Times(1).WillOnce(Return(true));
    SHT20Arduino sht(i2c);
    EXPECT_TRUE(sht.init());

    EXPECT_CALL(i2c, getTem()).Times(4).WillOnce(Return(-11)).WillOnce(Return(-11.50)).WillOnce(Return(-12)).WillOnce(Return(33.3));
    EXPECT_CALL(i2c, getHum()).Times(4).WillOnce(Return(25)).WillOnce(Return(25.5)).WillOnce(Return(26)).WillOnce(Return(69.1));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 10; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        sht.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_FLOAT_EQ(-11.5, sht.getCurrentMeasurementByID());
    EXPECT_FLOAT_EQ(25.5, sht.getCurrentMeasurementByID(1));
    EXPECT_EQ(-127, sht.getCurrentMeasurementByID(2));

    incrementIntervalMillis = 0;
    nextLoopMillis = 3000;
    EXPECT_CALL(i2c, getTem()).Times(3).WillOnce(Return(33.4)).WillOnce(Return(33.5)).WillOnce(Return(77.1));
    EXPECT_CALL(i2c, getHum()).Times(3).WillOnce(Return(79.1)).WillOnce(Return(89.1)).WillOnce(Return(99.1));
    for (int i = 0; i < 3; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        sht.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_FLOAT_EQ(33.4, sht.getCurrentMeasurementByID());
    EXPECT_FLOAT_EQ(79.1, sht.getCurrentMeasurementByID(1));
    EXPECT_EQ(-127, sht.getCurrentMeasurementByID(4));
    releaseArduinoMock();
}

TEST_F(SHT20ArduinoTest, test_requestCurrentMeasurement_withDifferentMeasurementIntervals_SavesArrayOfMeasurements)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    iarduino_I2C_SHT i2c(0x7A);
    EXPECT_CALL(i2c, begin()).Times(1).WillOnce(Return(true));
    SHT20Arduino sht(i2c);
    EXPECT_TRUE(sht.init(1000));

    EXPECT_CALL(i2c, getTem()).Times(4).WillOnce(Return(74)).WillOnce(Return(84)).WillOnce(Return(94)).WillOnce(Return(21.9));
    EXPECT_CALL(i2c, getHum()).Times(4).WillOnce(Return(0.15)).WillOnce(Return(0.20)).WillOnce(Return(0.25)).WillOnce(Return(87.33));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        sht.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_FLOAT_EQ(84, sht.getCurrentMeasurementByID());
    EXPECT_FLOAT_EQ(0.20, sht.getCurrentMeasurementByID(1));

    EXPECT_CALL(i2c, begin()).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(sht.init(2000));
    incrementIntervalMillis = 0;
    nextLoopMillis = 3000;
    EXPECT_CALL(i2c, getTem()).Times(4).WillOnce(Return(22.1)).WillOnce(Return(22.3)).WillOnce(Return(22.3)).WillOnce(Return(0));
    EXPECT_CALL(i2c, getHum()).Times(4).WillOnce(Return(77.33)).WillOnce(Return(67.33)).WillOnce(Return(22.3)).WillOnce(Return(24));
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        sht.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_FLOAT_EQ(22.1, sht.getCurrentMeasurementByID());
    EXPECT_FLOAT_EQ(77.33, sht.getCurrentMeasurementByID(1));
    releaseArduinoMock();
}