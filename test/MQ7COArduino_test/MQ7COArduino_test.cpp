#include <gtest/gtest.h>
#include <MQ7COArduino_test.h>
#include <Arduino.h>
#include <MQ7COArduino.h>
#include <MQ7COArduino.cpp>
#include <MQ7Impl.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

// TEST_F(MQ7COArduinoTest, testinitcallsPinModetoSetPinsToOUTReturnsTrue)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
//     MQ7COArduino mq7co(2, 3);

//     EXPECT_TRUE(mq7co.init());
//     releaseArduinoMock();
// }

// TEST_F(MQ7COArduinoTest, test_init_WithTheSamePortSignalAndHeater_FailureReturnsFalse)
// {
//     MQ7COArduino mq7co;
//     EXPECT_FALSE(mq7co.init());

//     MQ7COArduino mq7(7, 7);
//     EXPECT_FALSE(mq7.init());
// }

// TEST_F(MQ7COArduinoTest, test_setDifferentSignalAndHeaterPINs_allowsInitReturnTrue)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
//     MQ7COArduino mq7co;

//     mq7co.changeSignalAndHeaterPINs(4, 1);

//     EXPECT_TRUE(mq7co.init());
//     releaseArduinoMock();
// }

// TEST_F(MQ7COArduinoTest, test_setSameSignalAndHeaterPINs_failsInit)
// {
//     MQ7COArduino mq7co;

//     mq7co.changeSignalAndHeaterPINs(14, 14);

//     EXPECT_FALSE(mq7co.init());
// }

// TEST_F(MQ7COArduinoTest, test_changeOfPhase_ChangesVoltageAndState)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
//     MQ7COArduino mq7co(1, 2);
//     mq7co.init();

//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(1);
//     mq7co.setPhase(HEATING);
//     EXPECT_TRUE(mq7co.isInPhase(HEATING));
//     EXPECT_FALSE(mq7co.isInPhase(COOLING));
//     EXPECT_FALSE(mq7co.isInPhase(READING));

//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(7772));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
//     mq7co.setPhase(COOLING);
//     EXPECT_FALSE(mq7co.isInPhase(HEATING));
//     EXPECT_TRUE(mq7co.isInPhase(COOLING));
//     EXPECT_FALSE(mq7co.isInPhase(READING));

//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(12031));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
//     mq7co.setPhase(READING);
//     EXPECT_FALSE(mq7co.isInPhase(HEATING));
//     EXPECT_FALSE(mq7co.isInPhase(COOLING));
//     EXPECT_TRUE(mq7co.isInPhase(READING));
//     releaseArduinoMock();
// }

// TEST_F(MQ7COArduinoTest, test_changeOfPhase_withoutInitFails)
// {
//     MQ7COArduino mq7co(1, 2);

//     EXPECT_FALSE(mq7co.setPhase(HEATING));
//     EXPECT_FALSE(mq7co.setPhase(COOLING));
//     EXPECT_FALSE(mq7co.setPhase(READING));
// }

// TEST_F(MQ7COArduinoTest, test_isPhaseCompleted_afterInvervalCompletes_ReturnsTrue)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
//     MQ7COArduino mq7co(1, 2);
//     mq7co.init();

//     uint32_t startIntervalMillis = 1223;
//     uint32_t milldeIntervalMillis = 10000;
//     EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillOnce(Return(startIntervalMillis + milldeIntervalMillis));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(1);
//     mq7co.setPhase(HEATING);
//     EXPECT_FALSE(mq7co.isPhaseCompleted(HEATING));
//     uint32_t endIntervalMillis = 60000;
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
//     EXPECT_TRUE(mq7co.isPhaseCompleted(HEATING));

//     startIntervalMillis = 107878;
//     milldeIntervalMillis = 35000;
//     EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillOnce(Return(startIntervalMillis + milldeIntervalMillis));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
//     mq7co.setPhase(COOLING);
//     EXPECT_FALSE(mq7co.isPhaseCompleted(COOLING));
//     endIntervalMillis = 60000;
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
//     EXPECT_TRUE(mq7co.isPhaseCompleted(COOLING));

//     startIntervalMillis = 71251878;
//     milldeIntervalMillis = 28000;
//     EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(startIntervalMillis)).WillOnce(Return(startIntervalMillis + milldeIntervalMillis));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
//     mq7co.setPhase(READING);
//     EXPECT_FALSE(mq7co.isPhaseCompleted(READING));
//     endIntervalMillis = 30000;
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + endIntervalMillis));
//     EXPECT_TRUE(mq7co.isPhaseCompleted(READING));
//     releaseArduinoMock();
// }

// TEST_F(MQ7COArduinoTest, test_isPhaseCompleted_OnMillisOverflow_ReturnsTrue)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
//     MQ7COArduino mq7co(1, 2);
//     mq7co.init();

//     uint32_t superLargeStartMillis = UINT32_MAX - 1000;
//     uint32_t interimMillisAfterOverflowUINT = 29000;
//     EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(superLargeStartMillis)).WillOnce(Return(interimMillisAfterOverflowUINT));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(1);
//     mq7co.setPhase(HEATING);
//     EXPECT_TRUE(mq7co.isInPhase(HEATING));
//     EXPECT_FALSE(mq7co.isInPhase(COOLING));
//     EXPECT_FALSE(mq7co.isPhaseCompleted(HEATING));

//     interimMillisAfterOverflowUINT = 59000;
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(interimMillisAfterOverflowUINT));
//     EXPECT_TRUE(mq7co.isInPhase(HEATING));
//     EXPECT_FALSE(mq7co.isInPhase(COOLING));
//     EXPECT_TRUE(mq7co.isPhaseCompleted(HEATING));
//     releaseArduinoMock();
// }

// TEST_F(MQ7COArduinoTest, test_isPhaseCompleted_OnMillisOverflow_atUINT32Boundary_ReturnsTrue)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
//     MQ7COArduino mq7co(1, 2);
//     mq7co.init();

//     uint32_t superLargeStartMillis = UINT32_MAX;
//     uint32_t interimMillisAfterOverflowUINT = 59998;
//     EXPECT_CALL(*arduinoMock, millis).Times(2).WillOnce(Return(superLargeStartMillis)).WillOnce(Return(interimMillisAfterOverflowUINT));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
//     mq7co.setPhase(COOLING);
//     EXPECT_TRUE(mq7co.isInPhase(COOLING));
//     EXPECT_FALSE(mq7co.isInPhase(HEATING));
//     EXPECT_FALSE(mq7co.isPhaseCompleted(COOLING));

//     interimMillisAfterOverflowUINT = 59999;
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(interimMillisAfterOverflowUINT));
//     EXPECT_TRUE(mq7co.isInPhase(COOLING));
//     EXPECT_FALSE(mq7co.isInPhase(HEATING));
//     EXPECT_TRUE(mq7co.isPhaseCompleted(COOLING));
//     releaseArduinoMock();
// }

// TEST_F(MQ7COArduinoTest, test_readSensorValue_outsideReadingPhase_ReturnsFalse)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
//     MQ7COArduino mq7co(1, 2);
//     mq7co.init();

//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(1);
//     mq7co.setPhase(HEATING);
//     EXPECT_TRUE(mq7co.isInPhase(HEATING));
//     EXPECT_FALSE(mq7co.isInPhase(READING));
//     EXPECT_FALSE(mq7co.readNextMeasurement());

//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
//     mq7co.setPhase(COOLING);
//     EXPECT_TRUE(mq7co.isInPhase(COOLING));
//     EXPECT_FALSE(mq7co.isInPhase(READING));
//     EXPECT_FALSE(mq7co.readNextMeasurement());
//     releaseArduinoMock();
// }

// TEST_F(MQ7COArduinoTest, test_readSensorValue_inReadingPhase_savesValues)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
//     MQ7COArduino mq7co(1, 2);
//     mq7co.init();

//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(1231));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
//     mq7co.setPhase(READING);
//     EXPECT_TRUE(mq7co.isInPhase(READING));

//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(7500));
//     EXPECT_CALL(*arduinoMock, analogRead(1)).Times(1).WillOnce(Return(155));
//     EXPECT_TRUE(mq7co.readNextMeasurement());
//     releaseArduinoMock();
// }

// TEST_F(MQ7COArduinoTest, test_readSensorValue_afterReadingPhase_savesAverageValue)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
//     MQ7COArduino mq7co(1, 2);
//     mq7co.init();

//     uint32_t startIntervalMillis = 12300;
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
//     mq7co.setPhase(READING);
//     uint32_t incrementIntervalMillis = 0;
//     uint16_t nextLoopMillis = 3000;
//     for (int i = 0; i < 10; i++)
//     {
//         EXPECT_CALL(*arduinoMock, analogRead(1)).Times(1).WillOnce(Return(155 + i));
//         EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + incrementIntervalMillis));
//         EXPECT_TRUE(mq7co.readNextMeasurement());
//         EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + incrementIntervalMillis));
//         EXPECT_FALSE(mq7co.saveAverageMeasurement());
//         EXPECT_EQ(mq7co.getCurrentMeasurementByID(), -1);
//         incrementIntervalMillis += nextLoopMillis;
//     }
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + incrementIntervalMillis));
//     EXPECT_TRUE(mq7co.saveAverageMeasurement());
//     EXPECT_EQ(mq7co.getCurrentMeasurementByID(), 159);
//     releaseArduinoMock();
// }

// TEST_F(MQ7COArduinoTest, test_readSensorValue_ifReadingInterval_IsLessThan3sec_skipReading)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
//     MQ7COArduino mq7co(1, 2);
//     mq7co.init();

//     uint32_t startIntervalMillis = 8500;
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
//     mq7co.setPhase(READING);
//     uint32_t incrementIntervalMillis = 0;
//     uint16_t nextLoopMillis = 1000;
//     for (int i = 0; i < 30; i++)
//     {
//         EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + incrementIntervalMillis));
//         EXPECT_CALL(*arduinoMock, analogRead(1)).Times(AtLeast(0)).WillRepeatedly(Return(155 + i));
//         mq7co.readNextMeasurement();
//         EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + incrementIntervalMillis));
//         EXPECT_FALSE(mq7co.saveAverageMeasurement());
//         EXPECT_EQ(mq7co.getCurrentMeasurementByID(), -1);
//         incrementIntervalMillis += nextLoopMillis;
//     }
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis + incrementIntervalMillis));
//     EXPECT_TRUE(mq7co.saveAverageMeasurement());
//     EXPECT_EQ(mq7co.getCurrentMeasurementByID(), 168);
//     releaseArduinoMock();
// }

// TEST_F(MQ7COArduinoTest, test_getCurrentMeasurementByID_WithoutInit_ReturnsMinusOne)
// {
//     MQ7COArduino mq7co(1, 2);
//     EXPECT_EQ(mq7co.getCurrentMeasurementByID(), -1);
// }

// TEST_F(MQ7COArduinoTest, test_requestCurrentMeasurement_SavesAverage)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
//     MQ7COArduino mq7co(1, 2);
//     mq7co.init();
//     uint32_t startIntervalMillis = 8500;
//     EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis));
//     EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(AtLeast(1));
//     mq7co.setPhase(HEATING);

//     EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(2);
//     uint32_t incrementIntervalMillis = 0;
//     uint16_t nextLoopMillis = 3000;
//     for (int i = 0; i < 60; i++)
//     {
//         EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
//         EXPECT_CALL(*arduinoMock, analogRead(1)).Times(AtLeast(0)).WillRepeatedly(Return(180 + i));
//         mq7co.requestCurrentMeasurement();
//         incrementIntervalMillis += nextLoopMillis;
//     }
//     EXPECT_EQ(mq7co.getCurrentMeasurementByID(), 224);
//     releaseArduinoMock();
// }

TEST_F(MQ7COArduinoTest, test_requestCurrentMeasurement_new_SavesAverage)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, OUTPUT)).Times(2);
    MQ7COArduino mq7co(1, 2);
    uint32_t startIntervalMillis = 8500;
    EXPECT_CALL(*arduinoMock, millis).Times(1).WillOnce(Return(startIntervalMillis));
    EXPECT_CALL(*arduinoMock, analogWrite(2, HIGH_5_0)).Times(AtLeast(1));
    mq7co.init_new();

    EXPECT_CALL(*arduinoMock, analogWrite(2, LOW_1_4)).Times(1);
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 3000;
    for (int i = 0; i < 60; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        EXPECT_CALL(*arduinoMock, analogRead(1)).Times(AtLeast(0)).WillRepeatedly(Return(180 + i));
        mq7co.requestCurrentMeasurement_new();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_EQ(mq7co.getCurrentMeasurementByID(), 224);
    releaseArduinoMock();
}

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

TEST_F(MockMQ7Impl, test_isPhaseCompleted_afterInvervalCompletes_ReturnsTrue)
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
