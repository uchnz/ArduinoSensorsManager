#include <gtest/gtest.h>
#include <MQ7COIOArduino.h>
#include <PhaseTimerArduino.h>
#include <MQ7COIOArduino_test.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(MQ7COIOArduinoTest, test_Init_setsPINs_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A0, INPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(3, OUTPUT)).Times(1);
    ThreePhaseTimerArduino timer;
    MQ7COIOArduino mq7co(A0, 3, timer);

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1000));
    EXPECT_CALL(*arduinoMock, analogWrite(3, 255)).Times(1);
    EXPECT_TRUE(mq7co.init());
    releaseArduinoMock();
}

TEST_F(MQ7COIOArduinoTest, test_init_WithTheSamePortForSignalAndHeater_Fails_ReturnsFalse)
{
    ThreePhaseTimerArduino timer;
    MQ7COIOArduino mq7(A1, A1, timer);
    EXPECT_FALSE(mq7.init());
}

TEST_F(MQ7COIOArduinoTest, test_reading_withoutInit_ReturnsError)
{
    ThreePhaseTimerArduino timer;
    MQ7COIOArduino mq7co(A0, 3, timer);
    EXPECT_EQ(0xFFFFFFFF, mq7co.read());
}

TEST_F(MQ7COIOArduinoTest, test_read_inHeatCycle_ReturnsErrorValue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A1, INPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(4, OUTPUT)).Times(1);
    ThreePhaseTimerArduino timer;
    MQ7COIOArduino mq7(A1, 4, timer);
    EXPECT_CALL(*arduinoMock, analogWrite(4, 255)).Times(1);
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(500));
    EXPECT_TRUE(mq7.init());

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1000));
    timer.setPhase(phasetimer_nm::HEATING);
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(50000));
    EXPECT_EQ(0xFFFFFFFF, mq7.read());

    releaseArduinoMock();
}

TEST_F(MQ7COIOArduinoTest, test_read_inCoolCycle_ReturnsErrorValue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A1, INPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(4, OUTPUT)).Times(1);
    ThreePhaseTimerArduino timer;
    MQ7COIOArduino mq7(A1, 4, timer);
    EXPECT_CALL(*arduinoMock, analogWrite(4, 255)).Times(1);
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(500));
    EXPECT_TRUE(mq7.init());

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(70000));
    timer.setPhase(phasetimer_nm::COOLING);
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(90000));
    EXPECT_EQ(0xFFFFFFFF, mq7.read());

    releaseArduinoMock();
}

TEST_F(MQ7COIOArduinoTest, test_read_inReadCycle_ReturnsValue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A1, INPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(4, OUTPUT)).Times(1);
    ThreePhaseTimerArduino timer;
    MQ7COIOArduino mq7(A1, 4, timer);
    EXPECT_CALL(*arduinoMock, analogWrite(4, 255)).Times(1);
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1000));
    EXPECT_TRUE(mq7.init());

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(10000));
    timer.setPhase(phasetimer_nm::READING);

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(20000));
    EXPECT_FALSE(timer.isPhaseCompleted(phasetimer_nm::READING));
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(20000 + 10));
    EXPECT_CALL(*arduinoMock, analogRead(A1)).Times(1).WillOnce(Return(140));
    EXPECT_EQ(140, mq7.read());

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(30000));
    EXPECT_FALSE(timer.isPhaseCompleted(phasetimer_nm::READING));
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(30000 + 1));
    EXPECT_CALL(*arduinoMock, analogRead(A1)).Times(1).WillOnce(Return(270));
    EXPECT_EQ(270, mq7.read());

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(40000));
    EXPECT_TRUE(timer.isPhaseCompleted(phasetimer_nm::READING));
    EXPECT_CALL(*arduinoMock, analogRead(A1)).Times(0);
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(40000 + 10)).WillOnce(Return(40000 + 50));
    EXPECT_CALL(*arduinoMock, analogWrite(4, 255)).Times(1);
    EXPECT_EQ(0xFFFFFFFF, mq7.read());

    releaseArduinoMock();
}
