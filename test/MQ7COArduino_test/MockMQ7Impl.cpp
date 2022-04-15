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
    mq7co.setPhase(mq7impl::HEATING);
    EXPECT_TRUE(mq7co.isInPhase(mq7impl::HEATING));
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::COOLING));
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::READING));

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(7772));
    mq7co.setPhase(mq7impl::COOLING);
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::HEATING));
    EXPECT_TRUE(mq7co.isInPhase(mq7impl::COOLING));
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::READING));

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(12031));
    mq7co.setPhase(mq7impl::READING);
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::HEATING));
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::COOLING));
    EXPECT_TRUE(mq7co.isInPhase(mq7impl::READING));
    releaseArduinoMock();
}

TEST_F(MockMQ7Impl, test_isPhaseCompleted_afterIntervalCompletes_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    MQ7Impl mq7co;

    uint32_t startIntervalMillis = 1223;
    uint32_t midIntervalMillis = 10000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillOnce(Return(startIntervalMillis + midIntervalMillis));
    mq7co.setPhase(mq7impl::HEATING);
    EXPECT_FALSE(mq7co.isPhaseCompleted(mq7impl::HEATING));
    uint32_t endIntervalMillis = 60000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
    EXPECT_TRUE(mq7co.isPhaseCompleted(mq7impl::HEATING));

    startIntervalMillis = 107878;
    midIntervalMillis = 35000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillOnce(Return(startIntervalMillis + midIntervalMillis));
    mq7co.setPhase(mq7impl::COOLING);
    EXPECT_FALSE(mq7co.isPhaseCompleted(mq7impl::COOLING));
    endIntervalMillis = 60000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
    EXPECT_TRUE(mq7co.isPhaseCompleted(mq7impl::COOLING));

    startIntervalMillis = 71251878;
    midIntervalMillis = 28000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillOnce(Return(startIntervalMillis + midIntervalMillis));
    mq7co.setPhase(mq7impl::READING);
    EXPECT_FALSE(mq7co.isPhaseCompleted(mq7impl::READING));
    endIntervalMillis = 30000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
    EXPECT_TRUE(mq7co.isPhaseCompleted(mq7impl::READING));
    releaseArduinoMock();
}

TEST_F(MockMQ7Impl, test_isPhaseCompleted_OnMillisOverflow_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    MQ7Impl mq7co;

    uint32_t superLargeStartMillis = UINT32_MAX - 1000;
    uint32_t interimMillisAfterOverflowUINT = 29000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(superLargeStartMillis)).WillOnce(Return(interimMillisAfterOverflowUINT));
    mq7co.setPhase(mq7impl::HEATING);
    EXPECT_TRUE(mq7co.isInPhase(mq7impl::HEATING));
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::COOLING));
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::READING));
    EXPECT_FALSE(mq7co.isPhaseCompleted(mq7impl::HEATING));

    interimMillisAfterOverflowUINT = 59000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(interimMillisAfterOverflowUINT));
    EXPECT_TRUE(mq7co.isInPhase(mq7impl::HEATING));
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::COOLING));
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::READING));
    EXPECT_TRUE(mq7co.isPhaseCompleted(mq7impl::HEATING));
    releaseArduinoMock();
}

TEST_F(MockMQ7Impl, test_isPhaseCompleted_OnMillisOverflow_atUINT32Boundary_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    MQ7Impl mq7co;

    uint32_t superLargeStartMillis = UINT32_MAX;
    uint32_t interimMillisAfterOverflowUINT = 59998;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(superLargeStartMillis)).WillOnce(Return(interimMillisAfterOverflowUINT));
    mq7co.setPhase(mq7impl::COOLING);
    EXPECT_TRUE(mq7co.isInPhase(mq7impl::COOLING));
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::HEATING));
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::READING));
    EXPECT_FALSE(mq7co.isPhaseCompleted(mq7impl::COOLING));

    interimMillisAfterOverflowUINT = 59999;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(interimMillisAfterOverflowUINT));
    EXPECT_TRUE(mq7co.isInPhase(mq7impl::COOLING));
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::HEATING));
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::READING));
    EXPECT_TRUE(mq7co.isPhaseCompleted(mq7impl::COOLING));
    releaseArduinoMock();
}

TEST_F(MockMQ7Impl, test_isTimeToReadMeasurement_withDefaultReadingInterval_startingRightAfterPhaseSet)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    MQ7Impl mq7co;

    uint32_t startIntervalMillis = 50;
    uint32_t midIntervalMillis = 1500;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillRepeatedly(Return(startIntervalMillis + midIntervalMillis));
    mq7co.setPhase(mq7impl::READING);
    EXPECT_FALSE(mq7co.isTimeToReadMeasurement());

    uint32_t endIntervalMillis = 3000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
    EXPECT_TRUE(mq7co.isTimeToReadMeasurement());
    releaseArduinoMock();
}

TEST_F(MockMQ7Impl, test_isTimeToReadMeasurement_notInReadingPhase)
{
    MQ7Impl mq7co;
    EXPECT_FALSE(mq7co.isInPhase(mq7impl::READING));

    EXPECT_FALSE(mq7co.isTimeToReadMeasurement());
}

TEST_F(MockMQ7Impl, test_isTimeToReadMeasurement_withDifferentIntervals)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    MQ7Impl mq7co(1000);

    uint32_t startIntervalMillis = 50;
    uint32_t midIntervalMillis = 500;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillRepeatedly(Return(startIntervalMillis + midIntervalMillis));
    mq7co.setPhase(mq7impl::READING);
    EXPECT_FALSE(mq7co.isTimeToReadMeasurement());

    uint32_t endIntervalMillis = 1000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
    EXPECT_TRUE(mq7co.isTimeToReadMeasurement());

    MQ7Impl mq7co2(5000);
    startIntervalMillis = 70050;
    midIntervalMillis = 3000;
    EXPECT_CALL(*arduinoMock, millis).Times(3).WillOnce(Return(startIntervalMillis)).WillRepeatedly(Return(startIntervalMillis + midIntervalMillis));
    mq7co2.setPhase(mq7impl::READING);
    EXPECT_TRUE(mq7co2.isTimeToReadMeasurement());
    EXPECT_FALSE(mq7co2.isTimeToReadMeasurement());

    endIntervalMillis = midIntervalMillis + 6000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
    EXPECT_TRUE(mq7co2.isTimeToReadMeasurement());
    releaseArduinoMock();
}

TEST_F(MockMQ7Impl, test_Init_withIncorrectReadIntervals)
{
    // read interval is less than 3000mls set to 3000
    // read interval is larger than 9000mls set to 9000
}