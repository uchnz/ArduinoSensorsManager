#include <gtest/gtest.h>
#include <MockMQ7Impl.h>
#include <MQ7Impl.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(MockMQ7Impl, test_changeOfPhase)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    MQ7Impl mq7co;

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231));
    mq7co.setPhase(HEATING);
    EXPECT_TRUE(mq7co.isInPhase(HEATING));
    EXPECT_FALSE(mq7co.isInPhase(COOLING));
    EXPECT_FALSE(mq7co.isInPhase(READING));

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(7772));
    mq7co.setPhase(COOLING);
    EXPECT_FALSE(mq7co.isInPhase(HEATING));
    EXPECT_TRUE(mq7co.isInPhase(COOLING));
    EXPECT_FALSE(mq7co.isInPhase(READING));

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(12031));
    mq7co.setPhase(READING);
    EXPECT_FALSE(mq7co.isInPhase(HEATING));
    EXPECT_FALSE(mq7co.isInPhase(COOLING));
    EXPECT_TRUE(mq7co.isInPhase(READING));
    releaseArduinoMock();
}

TEST_F(MockMQ7Impl, test_isPhaseCompleted_afterIntervalCompletes_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    MQ7Impl mq7co;

    uint32_t startIntervalMillis = 1223;
    uint32_t milldeIntervalMillis = 10000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillOnce(Return(startIntervalMillis + milldeIntervalMillis));
    mq7co.setPhase(HEATING);
    EXPECT_FALSE(mq7co.isPhaseCompleted(HEATING));
    uint32_t endIntervalMillis = 60000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
    EXPECT_TRUE(mq7co.isPhaseCompleted(HEATING));

    startIntervalMillis = 107878;
    milldeIntervalMillis = 35000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillOnce(Return(startIntervalMillis + milldeIntervalMillis));
    mq7co.setPhase(COOLING);
    EXPECT_FALSE(mq7co.isPhaseCompleted(COOLING));
    endIntervalMillis = 60000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
    EXPECT_TRUE(mq7co.isPhaseCompleted(COOLING));

    startIntervalMillis = 71251878;
    milldeIntervalMillis = 28000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillOnce(Return(startIntervalMillis + milldeIntervalMillis));
    mq7co.setPhase(READING);
    EXPECT_FALSE(mq7co.isPhaseCompleted(READING));
    endIntervalMillis = 30000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
    EXPECT_TRUE(mq7co.isPhaseCompleted(READING));
    releaseArduinoMock();
}

TEST_F(MockMQ7Impl, test_isPhaseCompleted_OnMillisOverflow_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    MQ7Impl mq7co;

    uint32_t superLargeStartMillis = UINT32_MAX - 1000;
    uint32_t interimMillisAfterOverflowUINT = 29000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(superLargeStartMillis)).WillOnce(Return(interimMillisAfterOverflowUINT));
    mq7co.setPhase(HEATING);
    EXPECT_TRUE(mq7co.isInPhase(HEATING));
    EXPECT_FALSE(mq7co.isInPhase(COOLING));
    EXPECT_FALSE(mq7co.isInPhase(READING));
    EXPECT_FALSE(mq7co.isPhaseCompleted(HEATING));

    interimMillisAfterOverflowUINT = 59000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(interimMillisAfterOverflowUINT));
    EXPECT_TRUE(mq7co.isInPhase(HEATING));
    EXPECT_FALSE(mq7co.isInPhase(COOLING));
    EXPECT_FALSE(mq7co.isInPhase(READING));
    EXPECT_TRUE(mq7co.isPhaseCompleted(HEATING));
    releaseArduinoMock();
}

TEST_F(MockMQ7Impl, test_isPhaseCompleted_OnMillisOverflow_atUINT32Boundary_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    MQ7Impl mq7co;

    uint32_t superLargeStartMillis = UINT32_MAX;
    uint32_t interimMillisAfterOverflowUINT = 59998;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(superLargeStartMillis)).WillOnce(Return(interimMillisAfterOverflowUINT));
    mq7co.setPhase(COOLING);
    EXPECT_TRUE(mq7co.isInPhase(COOLING));
    EXPECT_FALSE(mq7co.isInPhase(HEATING));
    EXPECT_FALSE(mq7co.isInPhase(READING));
    EXPECT_FALSE(mq7co.isPhaseCompleted(COOLING));

    interimMillisAfterOverflowUINT = 59999;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(interimMillisAfterOverflowUINT));
    EXPECT_TRUE(mq7co.isInPhase(COOLING));
    EXPECT_FALSE(mq7co.isInPhase(HEATING));
    EXPECT_FALSE(mq7co.isInPhase(READING));
    EXPECT_TRUE(mq7co.isPhaseCompleted(COOLING));
    releaseArduinoMock();
}
