#include <gtest/gtest.h>
// #include <gmock/gmock.h>
#include <TimerArduino_test.h>
#include <TimerArduino.h>

using ::testing::Return;

TEST_F(TimerArduinoTest, test_timerDeafultInterval)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    TimerArduino timer;

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1000));
    EXPECT_FALSE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(3000));
    EXPECT_TRUE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(5550));
    EXPECT_FALSE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(7000));
    EXPECT_TRUE(timer.isReadyForNextRead());
    releaseArduinoMock();
}

TEST_F(TimerArduinoTest, test_timerSpecifiedInterval)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    TimerArduino timer(100);

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(100));
    EXPECT_TRUE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(150));
    EXPECT_FALSE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(200));
    EXPECT_TRUE(timer.isReadyForNextRead());
    releaseArduinoMock();
}

TEST_F(TimerArduinoTest, test_zeroInterval_willBeSameAsDefault)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    TimerArduino timer(0);

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(5000));
    EXPECT_TRUE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(7000));
    EXPECT_FALSE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(8000));
    EXPECT_TRUE(timer.isReadyForNextRead());
    releaseArduinoMock();
}

TEST_F(TimerArduinoTest, test_maxValueInterval)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    TimerArduino timer(0xFFFF);

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(50000));
    EXPECT_FALSE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(70000));
    EXPECT_TRUE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(100000));
    EXPECT_FALSE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(150000));
    EXPECT_TRUE(timer.isReadyForNextRead());
    releaseArduinoMock();
}

TEST_F(TimerArduinoTest, test_setInterval)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    TimerArduino timer(0xFFFF);

    timer.setReadingInterval(500);

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(100));
    EXPECT_FALSE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(600));
    EXPECT_TRUE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1000));
    EXPECT_FALSE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1100));
    EXPECT_TRUE(timer.isReadyForNextRead());
    releaseArduinoMock();
}

TEST_F(TimerArduinoTest, test_setIntervalWithZero_willBeSameAsDefault)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    TimerArduino timer(500);

    timer.setReadingInterval(0);

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(2500));
    EXPECT_FALSE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(3000));
    EXPECT_TRUE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(5999));
    EXPECT_FALSE(timer.isReadyForNextRead());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(6000));
    EXPECT_TRUE(timer.isReadyForNextRead());
    releaseArduinoMock();
}