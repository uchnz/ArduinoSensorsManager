#include <gtest/gtest.h>
#include <MQ7COArduino.h>
#include <MQ7COArduino_test.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(MQ7COArduinoTest, test_Init_CallsPinMode_toSetPinsToOUT_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(2, 3);

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(25873405));
    EXPECT_CALL(*arduinoMock, analogWrite(3, HIGH_5_0)).Times(1);
    EXPECT_TRUE(mq7co.init());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_init_WithTheSamePortForSignalAndHeater_Fails_ReturnsFalse)
{
    MQ7COArduino mq7co;
    EXPECT_FALSE(mq7co.init());

    MQ7COArduino mq7(7, 7);
    EXPECT_FALSE(mq7.init());
}

TEST_F(MQ7COArduinoTest, test_setDifferentSignalAndHeaterPINs_allowsInitReturnTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1111111));
    EXPECT_CALL(*arduinoMock, analogWrite(1, HIGH_5_0)).Times(1);
    MQ7COArduino mq7co;

    mq7co.changeSignalAndHeaterPINs(4, 1);

    EXPECT_TRUE(mq7co.init());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_setSameSignalAndHeaterPINs_failsInit)
{
    MQ7COArduino mq7co;

    mq7co.changeSignalAndHeaterPINs(14, 14);

    EXPECT_FALSE(mq7co.init());
}

TEST_F(MQ7COArduinoTest, test_getCurrentMeasurementByID_WithoutInit_ReturnsMinusOne)
{
    MQ7COArduino mq7co(1, 2);
    EXPECT_EQ(mq7co.getCurrentMeasurementByID(), -1);
}

TEST_F(MQ7COArduinoTest, test_requestCurrentMeasurement_SavesAverage)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(1, 2);
    uint32_t startIntervalMillis = 8500;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis));
    EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(AtLeast(1));
    mq7co.init();

    EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 3000;
    for (int i = 0; i < 60; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        EXPECT_CALL(*arduinoMock, analogRead(1)).Times(AtLeast(0)).WillRepeatedly(Return(180 + i));
        mq7co.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(mq7co.getCurrentMeasurementByID(), 224);
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_requestCurrentMeasurement_withLargeIntervals_calculatesAverageWithPartialData)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(1, 2);
    uint32_t startIntervalMillis = 8500;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis));
    EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(AtLeast(1));
    mq7co.init();

    EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 12000;
    for (int i = 0; i < 14; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        EXPECT_CALL(*arduinoMock, analogRead(1)).Times(AtLeast(0)).WillRepeatedly(Return(520 + i));
        mq7co.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(mq7co.getCurrentMeasurementByID(), 531);

    nextLoopMillis = 15000;
    EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
    for (int i = 0; i < 12; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        EXPECT_CALL(*arduinoMock, analogRead(1)).Times(AtLeast(0)).WillRepeatedly(Return(520 + i));
        mq7co.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(mq7co.getCurrentMeasurementByID(), 529);
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_requestCurrentMeasurement_withSmallIntervals_calculatesAverageWithPartialData)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(1, 2);
    uint32_t startIntervalMillis = 8500;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis));
    EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(AtLeast(1));
    mq7co.init();

    EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 160; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        EXPECT_CALL(*arduinoMock, analogRead(1)).Times(AtLeast(0)).WillRepeatedly(Return(730 + i));
        mq7co.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(mq7co.getCurrentMeasurementByID(), 863);
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_requestCurrentMeasurement_withoutInit_ReturnsMinusOne)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    MQ7COArduino mq7co(1, 2);
    EXPECT_CALL(*arduinoMock, millis).Times(0);
    EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(0);

    mq7co.requestCurrentMeasurement();

    EXPECT_EQ(mq7co.getCurrentMeasurementByID(), -1);
    releaseArduinoMock();
}