#include <gtest/gtest.h>
#include <SHT20Arduino.h>
#include <SHTIOArduino.h>
#include <SHT20Arduino_test.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(SHT20ArduinoTest, test_Init_Successful)
{
    iarduino_I2C_SHT shtI2C(0x09);
    EXPECT_CALL(shtI2C, begin()).Times(1).WillOnce(Return(true));
    SHTIOArduino io(shtI2C);
    SHT20Arduino sht20("sht", io);

    TimerArduino timer;
    EXPECT_TRUE(sht20.init(&timer));
}

TEST_F(SHT20ArduinoTest, test_gettingMeasurementsWithoutInit_fails)
{
    iarduino_I2C_SHT shtI2C(0x09);
    EXPECT_CALL(shtI2C, begin()).Times(0);
    SHTIOArduino io(shtI2C);
    SHT20Arduino sht20("sht", io);

    EXPECT_CALL(shtI2C, getTem()).Times(0);
    EXPECT_CALL(shtI2C, getHum()).Times(0);
    sht20.requestCurrentMeasurement();

    double measurement = sht20.getCurrentMeasurementByID();
    EXPECT_DOUBLE_EQ(0xFFFFFFFF, measurement);
    measurement = sht20.getCurrentMeasurementByID(1);
    EXPECT_DOUBLE_EQ(0xFFFFFFFF, measurement);
}

TEST_F(SHT20ArduinoTest, test_gettingSensorIDAboveExisting_fails)
{
    iarduino_I2C_SHT shtI2C(0x09);
    EXPECT_CALL(shtI2C, begin()).Times(1).WillOnce(Return(true));
    SHTIOArduino io(shtI2C);
    SHT20Arduino sht20("sht", io);

    TimerArduino timer;
    EXPECT_TRUE(sht20.init(&timer));

    double nonexist = sht20.getCurrentMeasurementByID(5);
    EXPECT_DOUBLE_EQ(0xFFFFFFFF, nonexist);
}

TEST_F(SHT20ArduinoTest, test_gettingTemperatureWithoutFirstTimeRequesting_fails)
{
    iarduino_I2C_SHT shtI2C(0x09);
    EXPECT_CALL(shtI2C, begin()).Times(1).WillOnce(Return(true));
    SHTIOArduino io(shtI2C);
    SHT20Arduino sht20("sht", io);

    TimerArduino timer;
    EXPECT_TRUE(sht20.init(&timer));

    EXPECT_CALL(shtI2C, getTem()).Times(0);
    EXPECT_CALL(shtI2C, getHum()).Times(0);
    double measurement = sht20.getCurrentMeasurementByID();
    EXPECT_DOUBLE_EQ(0xFFFFFFFF, measurement);
    measurement = sht20.getCurrentMeasurementByID(1);
    EXPECT_DOUBLE_EQ(0xFFFFFFFF, measurement);
}

TEST_F(SHT20ArduinoTest, test_requestCurrentMeasurement_withinTimeInterval_SavesArrayOfMeasurements)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    iarduino_I2C_SHT shtI2C(0x09);
    SHTIOArduino io(shtI2C);
    SHT20Arduino sht20("sht", io);

    TimerArduino timer;
    EXPECT_CALL(shtI2C, begin()).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(sht20.init(&timer));

    EXPECT_CALL(shtI2C, getTem()).Times(4).WillOnce(Return(-11)).WillOnce(Return(-11.50)).WillOnce(Return(-12)).WillOnce(Return(33.3));
    EXPECT_CALL(shtI2C, getHum()).Times(4).WillOnce(Return(25)).WillOnce(Return(25.5)).WillOnce(Return(26)).WillOnce(Return(69.1));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 10; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        sht20.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_DOUBLE_EQ(-11.5, sht20.getCurrentMeasurementByID());
    EXPECT_DOUBLE_EQ(25.5, sht20.getCurrentMeasurementByID(1));
    EXPECT_EQ(0xFFFFFFFF, sht20.getCurrentMeasurementByID(2));

    incrementIntervalMillis = 0;
    nextLoopMillis = 3000;
    EXPECT_CALL(shtI2C, getTem()).Times(3).WillOnce(Return(33.4)).WillOnce(Return(33.5)).WillOnce(Return(77.1));
    EXPECT_CALL(shtI2C, getHum()).Times(3).WillOnce(Return(79.1)).WillOnce(Return(89.1)).WillOnce(Return(99.1));
    for (int i = 0; i < 3; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        sht20.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_DOUBLE_EQ(33.4, sht20.getCurrentMeasurementByID());
    EXPECT_DOUBLE_EQ(79.1, sht20.getCurrentMeasurementByID(1));
    EXPECT_EQ(0xFFFFFFFF, sht20.getCurrentMeasurementByID(4));
    releaseArduinoMock();
}

TEST_F(SHT20ArduinoTest, test_requestCurrentMeasurement_withDifferentMeasurementIntervals_SavesArrayOfMeasurements)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    iarduino_I2C_SHT shtI2C(0x09);
    SHTIOArduino io(shtI2C);
    SHT20Arduino sht20("sht", io);

    TimerArduino timer(1000);
    EXPECT_CALL(shtI2C, begin()).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(sht20.init(&timer));

    EXPECT_CALL(shtI2C, getTem()).Times(4).WillOnce(Return(74)).WillOnce(Return(84)).WillOnce(Return(94)).WillOnce(Return(21.9));
    EXPECT_CALL(shtI2C, getHum()).Times(4).WillOnce(Return(0.15)).WillOnce(Return(0.20)).WillOnce(Return(0.25)).WillOnce(Return(87.33));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        sht20.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_DOUBLE_EQ(84, sht20.getCurrentMeasurementByID());
    EXPECT_DOUBLE_EQ(0.20, sht20.getCurrentMeasurementByID(1));

    timer.setReadingInterval(2000);
    incrementIntervalMillis = 0;
    nextLoopMillis = 3000;
    EXPECT_CALL(shtI2C, getTem()).Times(4).WillOnce(Return(22.1)).WillOnce(Return(22.3)).WillOnce(Return(22.3)).WillOnce(Return(0));
    EXPECT_CALL(shtI2C, getHum()).Times(4).WillOnce(Return(77.33)).WillOnce(Return(67.33)).WillOnce(Return(22.3)).WillOnce(Return(24));
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        sht20.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_DOUBLE_EQ(22.1, sht20.getCurrentMeasurementByID());
    EXPECT_DOUBLE_EQ(77.33, sht20.getCurrentMeasurementByID(1));
    releaseArduinoMock();
}