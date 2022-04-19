#include <gtest/gtest.h>
#include <OnOffSensorArduino.h>
#include <OnOffSensorArduino_test.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(OnOffSensorArduinoTest, test_Init_CallsPinMode_toSetPinToInput_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(2, INPUT_PULLUP)).Times(1);
    OnOffSensorArduino onoff(2);

    EXPECT_TRUE(onoff.init());
    releaseArduinoMock();
}

TEST_F(OnOffSensorArduinoTest, test_Init_FailsWithoutPIN_ReturnsFalse)
{
    OnOffSensorArduino onoff;

    EXPECT_FALSE(onoff.init());
}

TEST_F(OnOffSensorArduinoTest, test_SetPINWithNonZero_allowsInitReturnTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    OnOffSensorArduino onoff;
    EXPECT_FALSE(onoff.init());

    EXPECT_CALL(*arduinoMock, pinMode(13, INPUT_PULLUP)).Times(1);
    onoff.SetPIN(13);

    EXPECT_TRUE(onoff.init());
    releaseArduinoMock();
}

TEST_F(OnOffSensorArduinoTest, test_getCurrentMeasurementByID_withoutInit_ReturnsMinusOne)
{
    OnOffSensorArduino onoff;
    onoff.SetPIN(4);

    EXPECT_EQ(onoff.getCurrentMeasurementByID(), -1);
}

TEST_F(OnOffSensorArduinoTest, test_SetPIN_resetsInitStatus)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    OnOffSensorArduino onoff(1);
    EXPECT_CALL(*arduinoMock, pinMode(1, INPUT_PULLUP)).Times(1);
    EXPECT_TRUE(onoff.init());
    onoff.SetPIN(4);

    EXPECT_EQ(onoff.getCurrentMeasurementByID(), -1);
    releaseArduinoMock();
}

TEST_F(OnOffSensorArduinoTest, test_requestCurrentMeasurement_withoutInit_Fails)
{
    OnOffSensorArduino onoff;
    onoff.requestCurrentMeasurement();
    EXPECT_EQ(onoff.getCurrentMeasurementByID(), -1);

    onoff.SetPIN(13);
    EXPECT_EQ(onoff.getCurrentMeasurementByID(), -1);
}

TEST_F(OnOffSensorArduinoTest, test_getCurrentMeasurementByID_withoutRequestCurrentMeasurement_ReturnsMinusOne)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    OnOffSensorArduino onoff(2);
    EXPECT_CALL(*arduinoMock, pinMode(2, INPUT_PULLUP)).Times(1);
    EXPECT_TRUE(onoff.init());

    EXPECT_CALL(*arduinoMock, digitalRead(0)).Times(0);
    EXPECT_EQ(onoff.getCurrentMeasurementByID(), -1);

    releaseArduinoMock();
}

TEST_F(OnOffSensorArduinoTest, test_requestCurrentMeasurement_withinTimeInterval_SavesArrayOfMeasurements)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    OnOffSensorArduino onoff(2);
    EXPECT_CALL(*arduinoMock, pinMode(2, INPUT_PULLUP)).Times(1);
    EXPECT_TRUE(onoff.init());

    EXPECT_CALL(*arduinoMock, digitalRead(2)).Times(4).WillOnce(Return(HIGH)).WillOnce(Return(LOW)).WillOnce(Return(HIGH)).WillOnce(Return(LOW));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 500;
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        onoff.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(1, onoff.getCurrentMeasurementByID());

    incrementIntervalMillis = 0;
    nextLoopMillis = 1000;
    EXPECT_CALL(*arduinoMock, digitalRead(2)).Times(3).WillOnce(Return(LOW)).WillOnce(Return(HIGH)).WillOnce(Return(LOW));
    for (int i = 0; i < 3; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        onoff.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(0, onoff.getCurrentMeasurementByID());

    releaseArduinoMock();
}

TEST_F(OnOffSensorArduinoTest, test_requestCurrentMeasurement_withDifferentMeasurementIntervals_SavesArrayOfMeasurements)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    OnOffSensorArduino onoff(2);
    EXPECT_CALL(*arduinoMock, pinMode(2, INPUT_PULLUP)).Times(1);
    EXPECT_TRUE(onoff.init(100));

    EXPECT_CALL(*arduinoMock, digitalRead(2)).Times(4).WillOnce(Return(LOW)).WillOnce(Return(HIGH)).WillOnce(Return(LOW)).WillOnce(Return(HIGH));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 50;
    for (int i = 0; i < 7; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        onoff.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(0, onoff.getCurrentMeasurementByID());

    onoff.SetPIN(4);
    EXPECT_CALL(*arduinoMock, pinMode(4, INPUT_PULLUP)).Times(1);
    EXPECT_TRUE(onoff.init(10));
    incrementIntervalMillis = 0;
    nextLoopMillis = 30;
    EXPECT_CALL(*arduinoMock, digitalRead(4)).Times(4).WillOnce(Return(LOW)).WillOnce(Return(HIGH)).WillOnce(Return(LOW)).WillOnce(Return(LOW));
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        onoff.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(0, onoff.getCurrentMeasurementByID());

    releaseArduinoMock();
}

// TEST_F(OnOffSensorArduinoTest, DISABLED_test_requestCurrentMeasurement_WhenToShortReadingInterval_ReturnErrorValue)
// {
//     FAIL();
// }