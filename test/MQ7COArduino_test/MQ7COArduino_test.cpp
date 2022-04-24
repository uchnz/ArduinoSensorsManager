#include <gtest/gtest.h>
#include <MQ7COArduino.h>
#include <MQ7COArduino_test.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(MQ7COArduinoTest, test_Init_CallsPinMode_toSetPinsToOUT_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A0, INPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(3, OUTPUT)).Times(1);
    MQ7COArduino mq7co(A0, 3);

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(25873405));
    EXPECT_CALL(*arduinoMock, analogWrite(3, mq7co_nm::HIGH_5_0)).Times(1);
    EXPECT_TRUE(mq7co.init("sensorName"));
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_init_WithTheSamePortForSignalAndHeater_Fails_ReturnsFalse)
{
    MQ7COArduino mq7co;
    EXPECT_FALSE(mq7co.init("sensorName"));

    MQ7COArduino mq7(A1, A1);
    EXPECT_FALSE(mq7.init("sensorName"));
}

TEST_F(MQ7COArduinoTest, test_setDifferentSignalAndHeaterPINs_allowsInitReturnTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A2, INPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(1, OUTPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1111111));
    EXPECT_CALL(*arduinoMock, analogWrite(1, mq7co_nm::HIGH_5_0)).Times(1);
    MQ7COArduino mq7co;

    mq7co.changeSignalAndHeaterPINs(A2, 1);

    EXPECT_TRUE(mq7co.init("sensorName"));
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_setSameSignalAndHeaterPINs_failsInit)
{
    MQ7COArduino mq7co;

    mq7co.changeSignalAndHeaterPINs(A3, A3);

    EXPECT_FALSE(mq7co.init("sensorName"));
}

TEST_F(MQ7COArduinoTest, test_getCurrentMeasurementByID_WithoutInit_ReturnsMinusOne)
{
    MQ7COArduino mq7co(A4, 2);
    EXPECT_EQ(-1, mq7co.getCurrentMeasurementByID());
}

TEST_F(MQ7COArduinoTest, test_requestCurrentMeasurement_SavesAverage)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A5, INPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(2, OUTPUT)).Times(1);
    MQ7COArduino mq7co(A5, 2);
    uint32_t startIntervalMillis = 8500;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis));
    EXPECT_CALL(*arduinoMock, analogWrite(2, mq7co_nm::HIGH_5_0)).Times(AtLeast(1));
    mq7co.init("sensorName");

    EXPECT_CALL(*arduinoMock, analogWrite(2, mq7co_nm::LOW_1_4)).Times(1);
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 3000;
    for (int i = 0; i < 60; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        EXPECT_CALL(*arduinoMock, analogRead(A5)).Times(AtLeast(0)).WillRepeatedly(Return(180 + i));
        mq7co.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(224, mq7co.getCurrentMeasurementByID());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_requestCurrentMeasurement_withLargeScanFrequency_calculatesAverageWithPartialData)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A6, INPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(2, OUTPUT)).Times(1);
    MQ7COArduino mq7co(A6, 2);
    uint32_t startIntervalMillis = 8500;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis));
    EXPECT_CALL(*arduinoMock, analogWrite(2, mq7co_nm::HIGH_5_0)).Times(AtLeast(1));
    mq7co.init("sensorName");

    EXPECT_CALL(*arduinoMock, analogWrite(2, mq7co_nm::LOW_1_4)).Times(1);
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 12000;
    for (int i = 0; i < 14; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        EXPECT_CALL(*arduinoMock, analogRead(A6)).Times(AtLeast(0)).WillRepeatedly(Return(520 + i));
        mq7co.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(531, mq7co.getCurrentMeasurementByID());

    nextLoopMillis = 15000;
    EXPECT_CALL(*arduinoMock, analogWrite(2, mq7co_nm::LOW_1_4)).Times(1);
    for (int i = 0; i < 12; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        EXPECT_CALL(*arduinoMock, analogRead(A6)).Times(AtLeast(0)).WillRepeatedly(Return(520 + i));
        mq7co.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(529, mq7co.getCurrentMeasurementByID());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_requestCurrentMeasurement_withSmallScanFrequency_calculatesAverageWithPartialData)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A7, INPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(2, OUTPUT)).Times(1);
    MQ7COArduino mq7co(A7, 2);
    uint32_t startIntervalMillis = 8500;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis));
    EXPECT_CALL(*arduinoMock, analogWrite(2, mq7co_nm::HIGH_5_0)).Times(AtLeast(1));
    mq7co.init("sensorName");

    EXPECT_CALL(*arduinoMock, analogWrite(2, mq7co_nm::LOW_1_4)).Times(1);
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 160; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        EXPECT_CALL(*arduinoMock, analogRead(A7)).Times(AtLeast(0)).WillRepeatedly(Return(730 + i));
        mq7co.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(863, mq7co.getCurrentMeasurementByID());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_requestCurrentMeasurement_withoutInit_ReturnsMinusOne)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    MQ7COArduino mq7co(A8, 2);
    EXPECT_CALL(*arduinoMock, millis).Times(0);
    EXPECT_CALL(*arduinoMock, analogWrite(2, mq7co_nm::HIGH_5_0)).Times(0);

    mq7co.requestCurrentMeasurement();

    EXPECT_EQ(-1, mq7co.getCurrentMeasurementByID());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_requestCurrentMeasurement_withSmallInterval_calculatesAverageWithPartialData)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A9, INPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(2, OUTPUT)).Times(1);
    MQ7COArduino mq7co(A9, 2);
    uint32_t startIntervalMillis = 8500;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis));
    EXPECT_CALL(*arduinoMock, analogWrite(2, mq7co_nm::HIGH_5_0)).Times(AtLeast(1));
    mq7co.init("sensorName", 1000);

    EXPECT_CALL(*arduinoMock, analogWrite(2, mq7co_nm::LOW_1_4)).Times(1);
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 500;
    for (int i = 0; i < 330; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        EXPECT_CALL(*arduinoMock, analogRead(A9)).Times(AtLeast(0)).WillRepeatedly(Return(730 + i));
        mq7co.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(979, mq7co.getCurrentMeasurementByID());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_requestCurrentMeasurement_withLargelInterval_calculatesAverageWithPartialData)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A10, INPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(2, OUTPUT)).Times(1);
    MQ7COArduino mq7co(A10, 2);
    uint32_t startIntervalMillis = 8500;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis));
    EXPECT_CALL(*arduinoMock, analogWrite(2, mq7co_nm::HIGH_5_0)).Times(AtLeast(1));
    mq7co.init("sensorName", 5000);

    EXPECT_CALL(*arduinoMock, analogWrite(2, mq7co_nm::LOW_1_4)).Times(1);
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 165; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        EXPECT_CALL(*arduinoMock, analogRead(A10)).Times(AtLeast(0)).WillRepeatedly(Return(300 + i));
        mq7co.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(435, mq7co.getCurrentMeasurementByID());
    releaseArduinoMock();
}