#include <gtest/gtest.h>
#include <SASArduino.h>
#include <SASArduino_test.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(SASArduinoTest, test_Init_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A0, INPUT)).Times(1);
    MockAnalogIOArduino io(A0, INPUT);
    SASArduino moisure("mois", io);
    TimerArduino timer;
    EXPECT_TRUE(moisure.init(&timer));

    char name[30];
    EXPECT_EQ(4, moisure.getName(name));
    EXPECT_STREQ("mois", name);

    EXPECT_TRUE(moisure.setName("new long name here"));
    EXPECT_CALL(*arduinoMock, pinMode(A0, INPUT)).Times(1);
    EXPECT_TRUE(moisure.init(&timer));
    EXPECT_EQ(18, moisure.getName(name));
    EXPECT_STREQ("new long name here", name);

    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_Init_withNameToLong_truncatesNameToSizeOfBuffer)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A2, INPUT)).Times(1);
    MockAnalogIOArduino io(A2, INPUT);
    SASArduino moisure("new long name here, longer than buffer size", io);
    TimerArduino timer;
    EXPECT_TRUE(moisure.init(&timer));

    char name[30];
    EXPECT_EQ(29, moisure.getName(name));
    EXPECT_STREQ("new long name here, longer th", name);
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_Init_withoutName_ReturnsFalse)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A3, INPUT)).Times(0);
    MockAnalogIOArduino io(A3, INPUT);
    SASArduino moisure(nullptr, io);

    char name[30];
    TimerArduino timer;
    EXPECT_FALSE(moisure.init(&timer));
    EXPECT_EQ(0, moisure.getName(name));

    moisure.setName("");
    EXPECT_FALSE(moisure.init(&timer));
    EXPECT_EQ(0, moisure.getName(name));
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_getName_beforeInit_ReturnsName)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A4, INPUT)).Times(0);
    MockAnalogIOArduino io(A4, INPUT);
    SASArduino moisure("name", io);

    char name[30];
    EXPECT_EQ(4, moisure.getName(name));
    EXPECT_STREQ("name", name);
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_getCurrentMeasurementByID_withoutInit_ReturnsMinusOne)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A5, INPUT)).Times(0);
    MockAnalogIOArduino io(A5, INPUT);
    SASArduino moisure(nullptr, io);

    EXPECT_EQ(0xFFFFFFFF, moisure.getCurrentMeasurementByID());
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_requestCurrentMeasurement_withoutInit_Fails)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A5, INPUT)).Times(0);
    MockAnalogIOArduino io(A5, INPUT);
    SASArduino moisure("s", io);

    moisure.requestCurrentMeasurement();
    EXPECT_EQ(0xFFFFFFFF, moisure.getCurrentMeasurementByID());
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_getCurrentMeasurementByID_withoutRequestCurrentMeasurement_ReturnsMinusOne)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A5, INPUT)).Times(1);
    MockAnalogIOArduino io(A5, INPUT);
    SASArduino moisure("name", io);
    TimerArduino timer;
    EXPECT_TRUE(moisure.init(&timer));

    EXPECT_CALL(*arduinoMock, analogRead(0)).Times(0);
    EXPECT_EQ(0x8000, moisure.getCurrentMeasurementByID());
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_analogReadings_returnsAverageOf3Readings)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A0, INPUT)).Times(1);
    MockAnalogIOArduino io(A0, INPUT);
    SASArduino moisure("sensorname", io);
    TimerArduino timer;
    EXPECT_TRUE(moisure.init(&timer));

    EXPECT_CALL(*arduinoMock, analogRead(A0)).Times(4).WillOnce(Return(140)).WillOnce(Return(150)).WillOnce(Return(160)).WillOnce(Return(270));
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
    EXPECT_CALL(*arduinoMock, analogRead(A0)).Times(3).WillOnce(Return(270)).WillOnce(Return(280)).WillOnce(Return(300));
    for (int i = 0; i < 3; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        moisure.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(273, moisure.getCurrentMeasurementByID());
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_analogReadings_withDifferentMeasurementIntervals_returnsAverageOf3Readings)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A2, INPUT)).Times(1);
    MockAnalogIOArduino io(A2, INPUT);
    SASArduino moisure("sensorname", io);
    TimerArduino timer(1000);
    EXPECT_TRUE(moisure.init(&timer));

    EXPECT_CALL(*arduinoMock, analogRead(A2)).Times(4).WillOnce(Return(140)).WillOnce(Return(150)).WillOnce(Return(150)).WillOnce(Return(270));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        moisure.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(147, moisure.getCurrentMeasurementByID());

    timer.setReadingInterval(2000);
    incrementIntervalMillis = 0;
    nextLoopMillis = 3000;
    EXPECT_CALL(*arduinoMock, analogRead(A2)).Times(4).WillOnce(Return(280)).WillOnce(Return(290)).WillOnce(Return(300)).WillOnce(Return(300));
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        moisure.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(280, moisure.getCurrentMeasurementByID());
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_digitalReadings_returnsAverageOf3Readings)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(15, INPUT)).Times(1);
    MockDigitalIOArduino io(15, INPUT);
    SASArduino onoff("sensorname", io);
    TimerArduino timer;
    EXPECT_TRUE(onoff.init(&timer));

    EXPECT_CALL(*arduinoMock, digitalRead(15)).Times(4).WillOnce(Return(1)).WillOnce(Return(0)).WillOnce(Return(1)).WillOnce(Return(0));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 10; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        onoff.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(1, onoff.getCurrentMeasurementByID());

    incrementIntervalMillis = 0;
    nextLoopMillis = 3000;
    EXPECT_CALL(*arduinoMock, digitalRead(15)).Times(3).WillOnce(Return(1)).WillOnce(Return(0)).WillOnce(Return(0));
    for (int i = 0; i < 3; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        onoff.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(0, onoff.getCurrentMeasurementByID());
    releaseArduinoMock();
}

TEST_F(SASArduinoTest, test_digitalReadings_withDifferentMeasurementIntervals_returnsAverageOf3Readings)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(7, INPUT)).Times(1);
    MockDigitalIOArduino io(7, INPUT);
    SASArduino onoff("sensorname", io);
    TimerArduino timer(1000);
    EXPECT_TRUE(onoff.init(&timer));

    EXPECT_CALL(*arduinoMock, digitalRead(7)).Times(4).WillOnce(Return(0)).WillOnce(Return(0)).WillOnce(Return(1)).WillOnce(Return(1));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        onoff.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(0, onoff.getCurrentMeasurementByID());

    timer.setReadingInterval(2000);
    incrementIntervalMillis = 0;
    nextLoopMillis = 3000;
    EXPECT_CALL(*arduinoMock, digitalRead(7)).Times(4).WillOnce(Return(0)).WillOnce(Return(1)).WillOnce(Return(1)).WillOnce(Return(0));
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        onoff.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(1, onoff.getCurrentMeasurementByID());
    releaseArduinoMock();
}
