#include <gtest/gtest.h>
#include <SASArduino.h>
#include <SASArduino_test.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(SASArduinoTest, test_Init_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(2, INPUT)).Times(1);
    SASArduino moisure(2);

    EXPECT_TRUE(moisure.init("mois"));
    char name[30];
    EXPECT_EQ(4, moisure.getName(name));
    EXPECT_STREQ("mois", name);

    EXPECT_CALL(*arduinoMock, pinMode(2, INPUT)).Times(1);
    EXPECT_TRUE(moisure.init("new long name here"));
    EXPECT_EQ(18, moisure.getName(name));
    EXPECT_STREQ("new long name here", name);

    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_Init_withNameToLong_truncatesNameToSizeOfBuffer)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(2, INPUT)).Times(1);
    SASArduino moisure(2);

    EXPECT_TRUE(moisure.init("new long name here, longer than buffer size"));

    char name[50];
    EXPECT_EQ(29, moisure.getName(name));
    EXPECT_STREQ("new long name here, longer th", name);
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_Init_withoutName_ReturnsFalse)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    SASArduino moisure(2);

    char name[30];
    EXPECT_FALSE(moisure.init(nullptr));
    EXPECT_EQ(0, moisure.getName(name));

    EXPECT_FALSE(moisure.init(""));
    EXPECT_EQ(0, moisure.getName(name));
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_getName_beforeInit_ReturnsZero)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    SASArduino moisure(2);

    char name[30];
    EXPECT_EQ(0, moisure.getName(name));
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_Init_FailsWithoutPIN_ReturnsFalse)
{
    SASArduino moisure;

    EXPECT_FALSE(moisure.init("name"));
}

TEST_F(SASArduinoTest, test_SetPINWithNonZero_allowsInitReturnTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    SASArduino moisure;
    EXPECT_FALSE(moisure.init("name"));

    EXPECT_CALL(*arduinoMock, pinMode(13, INPUT)).Times(1);
    moisure.SetPIN(13);

    EXPECT_TRUE(moisure.init("longNameHere"));
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_getCurrentMeasurementByID_withoutInit_ReturnsMinusOne)
{
    SASArduino moisure;
    moisure.SetPIN(4);

    EXPECT_EQ(moisure.getCurrentMeasurementByID(), -1);
}

TEST_F(SASArduinoTest, test_SetPIN_resetsInitStatus)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    SASArduino moisure(1);
    EXPECT_CALL(*arduinoMock, pinMode(1, INPUT)).Times(1);
    EXPECT_TRUE(moisure.init("somename"));
    moisure.SetPIN(4);

    EXPECT_EQ(moisure.getCurrentMeasurementByID(), -1);
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_requestCurrentMeasurement_withoutInit_Fails)
{
    SASArduino moisure;
    moisure.requestCurrentMeasurement();
    EXPECT_EQ(moisure.getCurrentMeasurementByID(), -1);

    moisure.SetPIN(13);
    EXPECT_EQ(moisure.getCurrentMeasurementByID(), -1);
}

TEST_F(SASArduinoTest, test_getCurrentMeasurementByID_withoutRequestCurrentMeasurement_ReturnsMinusOne)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    SASArduino moisure(2);
    EXPECT_CALL(*arduinoMock, pinMode(2, INPUT)).Times(1);
    EXPECT_TRUE(moisure.init("name"));

    EXPECT_CALL(*arduinoMock, analogRead(0)).Times(0);
    EXPECT_EQ(moisure.getCurrentMeasurementByID(), -1);

    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_requestCurrentMeasurement_withinTimeInterval_SavesArrayOfMeasurements)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    SASArduino moisure(2);
    EXPECT_CALL(*arduinoMock, pinMode(2, INPUT)).Times(1);
    EXPECT_TRUE(moisure.init("sensorname"));

    EXPECT_CALL(*arduinoMock, analogRead(2)).Times(4).WillOnce(Return(140)).WillOnce(Return(150)).WillOnce(Return(160)).WillOnce(Return(270));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 10; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        moisure.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(150, moisure.getCurrentMeasurementByID());

    incrementIntervalMillis = 0;
    nextLoopMillis = 3000;
    EXPECT_CALL(*arduinoMock, analogRead(2)).Times(3).WillOnce(Return(280)).WillOnce(Return(290)).WillOnce(Return(300));
    for (int i = 0; i < 3; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        moisure.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(280, moisure.getCurrentMeasurementByID());

    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_requestCurrentMeasurement_withDifferentMeasurementIntervals_SavesArrayOfMeasurements)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    SASArduino moisure(2);
    EXPECT_CALL(*arduinoMock, pinMode(2, INPUT)).Times(1);
    EXPECT_TRUE(moisure.init("sensorname", 1000));

    EXPECT_CALL(*arduinoMock, analogRead(2)).Times(4).WillOnce(Return(140)).WillOnce(Return(150)).WillOnce(Return(160)).WillOnce(Return(270));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        moisure.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(150, moisure.getCurrentMeasurementByID());

    moisure.SetPIN(4);
    EXPECT_CALL(*arduinoMock, pinMode(4, INPUT)).Times(1);
    EXPECT_TRUE(moisure.init("sensorname", 2000));
    incrementIntervalMillis = 0;
    nextLoopMillis = 3000;
    EXPECT_CALL(*arduinoMock, analogRead(4)).Times(4).WillOnce(Return(280)).WillOnce(Return(290)).WillOnce(Return(300)).WillOnce(Return(300));
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        moisure.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(290, moisure.getCurrentMeasurementByID());

    releaseArduinoMock();
}

TEST_F(SASArduinoTest, DISABLED_test_requestCurrentMeasurement_WhenToShortReadingInterval_ReturnErrorValue)
{
    FAIL();
}