#include <gtest/gtest.h>
// #include <gmock/gmock.h>
#include <PhaseTimerArduino_test.h>
#include <PhaseTimerArduino.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(PhaseTimerArduinoTest, test_changeOfPhase)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    ThreePhaseTimerArduino mq7co;

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231));
    mq7co.setPhase(phasetimer_nm::HEATING);
    EXPECT_TRUE(mq7co.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::COOLING));
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::READING));

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(7772));
    mq7co.setPhase(phasetimer_nm::COOLING);
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::HEATING));
    EXPECT_TRUE(mq7co.isInPhase(phasetimer_nm::COOLING));
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::READING));

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(12031));
    mq7co.setPhase(phasetimer_nm::READING);
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::COOLING));
    EXPECT_TRUE(mq7co.isInPhase(phasetimer_nm::READING));
    releaseArduinoMock();
}

TEST_F(PhaseTimerArduinoTest, test_isPhaseCompleted_afterIntervalCompletes_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    ThreePhaseTimerArduino mq7co;

    uint32_t startIntervalMillis = 1223;
    uint32_t midIntervalMillis = 10000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillOnce(Return(startIntervalMillis + midIntervalMillis));
    mq7co.setPhase(phasetimer_nm::HEATING);
    EXPECT_FALSE(mq7co.isPhaseCompleted(phasetimer_nm::HEATING));
    uint32_t endIntervalMillis = 60000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
    EXPECT_TRUE(mq7co.isPhaseCompleted(phasetimer_nm::HEATING));

    startIntervalMillis = 107878;
    midIntervalMillis = 35000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillOnce(Return(startIntervalMillis + midIntervalMillis));
    mq7co.setPhase(phasetimer_nm::COOLING);
    EXPECT_FALSE(mq7co.isPhaseCompleted(phasetimer_nm::COOLING));
    endIntervalMillis = 60000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
    EXPECT_TRUE(mq7co.isPhaseCompleted(phasetimer_nm::COOLING));

    startIntervalMillis = 71251878;
    midIntervalMillis = 28000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillOnce(Return(startIntervalMillis + midIntervalMillis));
    mq7co.setPhase(phasetimer_nm::READING);
    EXPECT_FALSE(mq7co.isPhaseCompleted(phasetimer_nm::READING));
    endIntervalMillis = 30000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
    EXPECT_TRUE(mq7co.isPhaseCompleted(phasetimer_nm::READING));
    releaseArduinoMock();
}

TEST_F(PhaseTimerArduinoTest, test_isPhaseCompleted_OnMillisOverflow_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    ThreePhaseTimerArduino mq7co;

    uint32_t superLargeStartMillis = UINT32_MAX - 1000;
    uint32_t interimMillisAfterOverflowUINT = 29000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(superLargeStartMillis)).WillOnce(Return(interimMillisAfterOverflowUINT));
    mq7co.setPhase(phasetimer_nm::HEATING);
    EXPECT_TRUE(mq7co.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::COOLING));
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::READING));
    EXPECT_FALSE(mq7co.isPhaseCompleted(phasetimer_nm::HEATING));

    interimMillisAfterOverflowUINT = 59000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(interimMillisAfterOverflowUINT));
    EXPECT_TRUE(mq7co.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::COOLING));
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::READING));
    EXPECT_TRUE(mq7co.isPhaseCompleted(phasetimer_nm::HEATING));
    releaseArduinoMock();
}

TEST_F(PhaseTimerArduinoTest, test_isPhaseCompleted_OnMillisOverflow_atUINT32Boundary_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    ThreePhaseTimerArduino mq7co;

    uint32_t superLargeStartMillis = UINT32_MAX;
    uint32_t interimMillisAfterOverflowUINT = 59998;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(superLargeStartMillis)).WillOnce(Return(interimMillisAfterOverflowUINT));
    mq7co.setPhase(phasetimer_nm::COOLING);
    EXPECT_TRUE(mq7co.isInPhase(phasetimer_nm::COOLING));
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::READING));
    EXPECT_FALSE(mq7co.isPhaseCompleted(phasetimer_nm::COOLING));

    interimMillisAfterOverflowUINT = 59999;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(interimMillisAfterOverflowUINT));
    EXPECT_TRUE(mq7co.isInPhase(phasetimer_nm::COOLING));
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(mq7co.isInPhase(phasetimer_nm::READING));
    EXPECT_TRUE(mq7co.isPhaseCompleted(phasetimer_nm::COOLING));
    releaseArduinoMock();
}

TEST_F(PhaseTimerArduinoTest, test_Init_withIncorrectReadIntervals)
{
    // read interval is less than 3000mls set to 3000
    // read interval is larger than 9000mls set to 9000
}

TEST_F(PhaseTimerArduinoTest, test_setHeatingPhase_for60Seconds)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    ThreePhaseTimerArduino timer;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1000));
    timer.setPhase(phasetimer_nm::HEATING);
    EXPECT_TRUE(timer.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::COOLING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::READING));

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(30000));
    EXPECT_FALSE(timer.isPhaseCompleted(phasetimer_nm::HEATING));
    EXPECT_TRUE(timer.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::COOLING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::READING));

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(61000 + 10));
    EXPECT_TRUE(timer.isPhaseCompleted(phasetimer_nm::HEATING));
    EXPECT_TRUE(timer.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::COOLING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::READING));
    releaseArduinoMock();
}

TEST_F(PhaseTimerArduinoTest, test_setCoolingPhase_for60Seconds)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    ThreePhaseTimerArduino timer;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1000));
    timer.setPhase(phasetimer_nm::COOLING);
    EXPECT_TRUE(timer.isInPhase(phasetimer_nm::COOLING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::READING));

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(45000));
    EXPECT_FALSE(timer.isPhaseCompleted(phasetimer_nm::COOLING));
    EXPECT_TRUE(timer.isInPhase(phasetimer_nm::COOLING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::READING));

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(61000));
    EXPECT_TRUE(timer.isPhaseCompleted(phasetimer_nm::COOLING));
    EXPECT_TRUE(timer.isInPhase(phasetimer_nm::COOLING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::READING));
    releaseArduinoMock();
}

TEST_F(PhaseTimerArduinoTest, test_setReadingPhase_for30Seconds)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    ThreePhaseTimerArduino timer;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1000));
    timer.setPhase(phasetimer_nm::READING);
    EXPECT_TRUE(timer.isInPhase(phasetimer_nm::READING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::COOLING));

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(20000));
    EXPECT_FALSE(timer.isPhaseCompleted(phasetimer_nm::READING));
    EXPECT_TRUE(timer.isInPhase(phasetimer_nm::READING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::COOLING));

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(31000));
    EXPECT_TRUE(timer.isPhaseCompleted(phasetimer_nm::READING));
    EXPECT_TRUE(timer.isInPhase(phasetimer_nm::READING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::HEATING));
    EXPECT_FALSE(timer.isInPhase(phasetimer_nm::COOLING));
    releaseArduinoMock();
}
