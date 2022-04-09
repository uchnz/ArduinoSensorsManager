#include <gtest/gtest.h>
#include <MQ7COArduino_test.h>
#include <Arduino.h>
#include <MQ7COArduino.h>

using ::testing::_;
using ::testing::Return;

TEST_F(MQ7COArduinoTest, test_init_SetsPinsToOUT_SuccessReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(2, 3);

    EXPECT_TRUE(mq7co.init());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_initWithTheSamePortSignalAndHeater_FailureReturnsFalse)
{
    MQ7COArduino mq7co;
    EXPECT_FALSE(mq7co.init());

    MQ7COArduino mq7(7, 7);
    EXPECT_FALSE(mq7.init());
}

TEST_F(MQ7COArduinoTest, test_setDifferentSignalAndHeaterPINs_allowsSuccessfulInit)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
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

TEST_F(MQ7COArduinoTest, test_setHighAndLowVoltageForHeater)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(1, 2);
    mq7co.init();

    EXPECT_CALL(*arduinoMock, millis).Times(2).WillRepeatedly(Return(0));
    EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(1);
    EXPECT_TRUE(mq7co.setHeaterVoltage(HIGH_5_0));
    EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
    EXPECT_TRUE(mq7co.setHeaterVoltage(LOW_1_4));
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_setHighAndLowVoltageForHeater_failsWithoutInit)
{
    MQ7COArduino mq7co(1, 2);

    EXPECT_FALSE(mq7co.setHeaterVoltage(HIGH_5_0));
    EXPECT_FALSE(mq7co.setHeaterVoltage(LOW_1_4));
}

TEST_F(MQ7COArduinoTest, test_setHighAndLowVoltageForHeater_failsAfterPINsChange_WithoutInit)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(1, 2);
    mq7co.init();

    EXPECT_CALL(*arduinoMock, millis).Times(2).WillRepeatedly(Return(0));
    EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(1);
    EXPECT_TRUE(mq7co.setHeaterVoltage(HIGH_5_0));
    EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
    EXPECT_TRUE(mq7co.setHeaterVoltage(LOW_1_4));

    mq7co.changeSignalAndHeaterPINs(4, 1);
    EXPECT_FALSE(mq7co.setHeaterVoltage(HIGH_5_0));
    EXPECT_FALSE(mq7co.setHeaterVoltage(LOW_1_4));
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_isInHeatingPhase_WhenHeating_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(1, 2);
    mq7co.init();

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231));
    EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(1);
    mq7co.setHeaterVoltage(HIGH_5_0);

    EXPECT_TRUE(mq7co.isInHeatingPhase());
    EXPECT_FALSE(mq7co.isInCoolingPhase());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231 + 30000));
    EXPECT_FALSE(mq7co.isPhaseCompleted());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_isPhaseCompleted_whenHeatingCompleted_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(1, 2);
    mq7co.init();

    EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(1);
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(1231)).WillOnce(Return(1231 + 30000));
    mq7co.setHeaterVoltage(HIGH_5_0);
    EXPECT_TRUE(mq7co.isInHeatingPhase());
    EXPECT_FALSE(mq7co.isInCoolingPhase());
    EXPECT_FALSE(mq7co.isPhaseCompleted());

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231 + 30000 + 30000));
    EXPECT_TRUE(mq7co.isInHeatingPhase());
    EXPECT_FALSE(mq7co.isInCoolingPhase());
    EXPECT_TRUE(mq7co.isPhaseCompleted());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_isPhaseCompleted_whenHeatingCompleted_ReturnsTrue_OnMillisOverflow)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(1);
    MQ7COArduino mq7co(1, 2);
    mq7co.init();

    uint32_t superLargeStartMillis = UINT32_MAX - 1000;
    uint32_t interimMillisAfterOverflowUINT = 29000;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(superLargeStartMillis)).WillOnce(Return(interimMillisAfterOverflowUINT));
    mq7co.setHeaterVoltage(HIGH_5_0);
    EXPECT_TRUE(mq7co.isInHeatingPhase());
    EXPECT_FALSE(mq7co.isInCoolingPhase());
    EXPECT_FALSE(mq7co.isPhaseCompleted());

    interimMillisAfterOverflowUINT = 59000;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(interimMillisAfterOverflowUINT));
    EXPECT_TRUE(mq7co.isInHeatingPhase());
    EXPECT_FALSE(mq7co.isInCoolingPhase());
    EXPECT_TRUE(mq7co.isPhaseCompleted());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_isPhaseCompleted_whenCoolingCompleted_ReturnsTrue_OnMillisOverflow_atUINT32Boundary)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
    MQ7COArduino mq7co(1, 2);
    mq7co.init();

    uint32_t superLargeStartMillis = UINT32_MAX;
    uint32_t interimMillisAfterOverflowUINT = 59998;
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(superLargeStartMillis)).WillOnce(Return(interimMillisAfterOverflowUINT));
    mq7co.setHeaterVoltage(LOW_1_4);
    EXPECT_TRUE(mq7co.isInCoolingPhase());
    EXPECT_FALSE(mq7co.isInHeatingPhase());
    EXPECT_FALSE(mq7co.isPhaseCompleted());

    interimMillisAfterOverflowUINT = 59999;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(interimMillisAfterOverflowUINT));
    EXPECT_TRUE(mq7co.isInCoolingPhase());
    EXPECT_FALSE(mq7co.isInHeatingPhase());
    EXPECT_TRUE(mq7co.isPhaseCompleted());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_isInCoolingPhase_WhenCooling_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(1, 2);
    mq7co.init();

    EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231));
    mq7co.setHeaterVoltage(LOW_1_4);

    EXPECT_TRUE(mq7co.isInCoolingPhase());
    EXPECT_FALSE(mq7co.isInHeatingPhase());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231 + 59500));
    EXPECT_FALSE(mq7co.isPhaseCompleted());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_isPhaseCompleted_whenCoolingCompleted_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(1, 2);
    mq7co.init();

    EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(1231)).WillOnce(Return(1231 + 30000));
    mq7co.setHeaterVoltage(LOW_1_4);
    EXPECT_TRUE(mq7co.isInCoolingPhase());
    EXPECT_FALSE(mq7co.isInHeatingPhase());
    EXPECT_FALSE(mq7co.isPhaseCompleted());

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231 + 30000 + 30000));
    EXPECT_TRUE(mq7co.isInCoolingPhase());
    EXPECT_FALSE(mq7co.isInHeatingPhase());
    EXPECT_TRUE(mq7co.isPhaseCompleted());
    releaseArduinoMock();
}

TEST_F(MQ7COArduinoTest, test_isInReadingPhase_whenLowVoltage_AfterCoolingDown_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(1, 2);
    mq7co.init();

    EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231));
    mq7co.setHeaterVoltage(LOW_1_4);

    EXPECT_TRUE(mq7co.isInCoolingPhase());
    EXPECT_FALSE(mq7co.isInHeatingPhase());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231 + 37500));
    EXPECT_FALSE(mq7co.isPhaseCompleted());

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231 + 60000));
    EXPECT_TRUE(mq7co.isPhaseCompleted());

    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231 + 60010));
    bool result = mq7co.readMeasurement();
    EXPECT_TRUE(mq7co.isInReadingPhase());
    EXPECT_FALSE(mq7co.isInCoolingPhase());
    EXPECT_FALSE(mq7co.isInHeatingPhase());
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231 + 60010 + 15000));
    EXPECT_FALSE(mq7co.isPhaseCompleted());

    result = mq7co.readMeasurement();
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231 + 60010 + 30000));
    EXPECT_TRUE(mq7co.isInReadingPhase());
    EXPECT_TRUE(mq7co.isPhaseCompleted());

    releaseArduinoMock();
}

// TEST_F(MQ7COArduinoTest, test_requestNewMeasurement_AfterInit_ReturnsTrue)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
//     EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(1);
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(6000));

//     MQ7COArduino mq7co(1, 2);
//     mq7co.init();

//     EXPECT_TRUE(mq7co.requestNewMeasurement());
//     bool result = mq7co.isInHeatingPhase();
//     EXPECT_TRUE(result);
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(6000 + 60000));
//     mq7co.requestNewMeasurement();
//     result = mq7co.isPhaseCompleted(HEATING_INTERVAL);
//     EXPECT_TRUE(result);
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(6000 + 60000 + 90000));
//     mq7co.requestNewMeasurement();
//     result = mq7co.isPhaseCompleted(COOLING_INTERVAL);
//     EXPECT_TRUE(result);
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(6000 + 60000 + 90000 + 45000));
//     mq7co.requestNewMeasurement();
//     result = mq7co.isPhaseCompleted(READING_INTERVAL);
//     EXPECT_TRUE(result);

//     releaseArduinoMock();
// }

// TEST_F(MQ7COArduinoTest, test_requestNewMeasurement_WithoutInit_ReturnsFalse)
// {
//     MQ7COArduino mq7co(1, 2);
//     EXPECT_FALSE(mq7co.requestNewMeasurement());

//     MQ7COArduino mq7;
//     mq7.init();
//     EXPECT_FALSE(mq7.requestNewMeasurement());
// }

// TEST_F(MQ7COArduinoTest, test_requestNewMeasurement_AfterPINsChangeWithoutInit_ReturnsFalse)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);

//     MQ7COArduino mq7co(1, 2);
//     mq7co.init();
//     EXPECT_TRUE(mq7co.requestNewMeasurement());

//     mq7co.changeSignalAndHeaterPINs(7, 14);
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(0);
//     EXPECT_FALSE(mq7co.requestNewMeasurement());

//     releaseArduinoMock();
// }
