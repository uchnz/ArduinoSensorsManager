
#include <gtest/gtest.h>
#include <DallasArduino.h>
#include <DallasArduino_test.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(DallasArduinoTest, test_Init_Successful_getDeviceCount_ReturnsNumberOfDevices)
{
    OneWire ow(22);
    DallasTemperature sensor(&ow);
    DallasArduino dallas(sensor);
    EXPECT_CALL(sensor, begin()).Times(1);

    EXPECT_CALL(sensor, getDeviceCount()).Times(1).WillOnce(Return(1));
    EXPECT_TRUE(dallas.init("sensorName"));

    EXPECT_EQ(1, dallas.getNumberOfConnectedSensors());
}

TEST_F(DallasArduinoTest, test_withoutInit_getDeviceCount_ReturnsZero)
{
    OneWire ow(22);
    DallasTemperature sensor(&ow);
    DallasArduino dallas(sensor);
    EXPECT_CALL(sensor, begin()).Times(0);

    EXPECT_CALL(sensor, getDeviceCount()).Times(0);

    EXPECT_EQ(0, dallas.getNumberOfConnectedSensors());
}

TEST_F(DallasArduinoTest, test_reInitFails_getDeviceCount_ReturnsPreviousNumber)
{
    OneWire ow(44);
    DallasTemperature sensor(&ow);
    DallasArduino dallas(sensor);
    EXPECT_CALL(sensor, begin()).Times(1);

    EXPECT_CALL(sensor, getDeviceCount()).Times(1).WillOnce(Return(7));
    EXPECT_TRUE(dallas.init("sensorName"));
    EXPECT_EQ(7, dallas.getNumberOfConnectedSensors());

    EXPECT_CALL(sensor, begin()).Times(0);
    EXPECT_CALL(sensor, getDeviceCount()).Times(0);
    EXPECT_FALSE(dallas.init("sensorName"));
    EXPECT_EQ(7, dallas.getNumberOfConnectedSensors());
}

TEST_F(DallasArduinoTest, test_InitFails_getDeviceCount_ReturnsZero)
{
    OneWire ow;
    DallasTemperature sensor(&ow);
    DallasArduino dallas(sensor);
    EXPECT_CALL(sensor, begin()).Times(1);

    EXPECT_CALL(sensor, getDeviceCount()).Times(1).WillOnce(Return(0));
    EXPECT_FALSE(dallas.init("sensorName"));

    EXPECT_EQ(0, dallas.getNumberOfConnectedSensors());
}

TEST_F(DallasArduinoTest, test_getCurrentMeasurementByID_withoutRequestingMeasurements_ReturnsError)
{
    OneWire ow;
    DallasTemperature sensor(&ow);
    DallasArduino dallas(sensor);

    EXPECT_EQ(-127, dallas.getCurrentMeasurementByID());
}

TEST_F(DallasArduinoTest, test_requestMeasurements_withoutSensors_ReturnsError)
{
    OneWire ow;
    DallasTemperature sensor(&ow);
    DallasArduino dallas(sensor);

    dallas.requestCurrentMeasurement();

    EXPECT_EQ(-127, dallas.getCurrentMeasurementByID());
}

TEST_F(DallasArduinoTest, test_getCurrentMeasurementByID_withOneSensor_ReturnsCorrectTemperature)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    OneWire ow(17);
    DallasTemperature sensor(&ow);
    DallasArduino dallas(sensor);
    EXPECT_CALL(sensor, begin()).Times(1);
    EXPECT_CALL(sensor, getDeviceCount()).Times(1).WillOnce(Return(1));
    EXPECT_TRUE(dallas.init("sensorName"));

    EXPECT_CALL(sensor, requestTemperatures()).Times(4);
    EXPECT_CALL(sensor, getTempCByIndex(0)).Times(4).WillOnce(Return(-10.8)).WillOnce(Return(-11)).WillOnce(Return(-11.2)).WillOnce(Return(74.8));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 50;
    for (int i = 0; i < 7; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        dallas.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_DOUBLE_EQ(-11, dallas.getCurrentMeasurementByID());

    EXPECT_CALL(sensor, requestTemperatures()).Times(4);
    EXPECT_CALL(sensor, getTempCByIndex(0)).Times(4).WillOnce(Return(75.1)).WillOnce(Return(75.4)).WillOnce(Return(75.8)).WillOnce(Return(90));
    incrementIntervalMillis = 0;
    nextLoopMillis = 100;
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        dallas.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_DOUBLE_EQ(75.1, dallas.getCurrentMeasurementByID());
    releaseArduinoMock();
}

TEST_F(DallasArduinoTest, test_getCurrentMeasurementByID_withOneSensor_withDifferentIntervals_ReturnsCorrectTemperature)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    OneWire ow(17);
    DallasTemperature sensor(&ow);
    DallasArduino dallas(sensor);
    EXPECT_CALL(sensor, begin()).Times(1);
    EXPECT_CALL(sensor, getDeviceCount()).Times(1).WillOnce(Return(1));
    EXPECT_TRUE(dallas.init("sensorName", 1000));

    EXPECT_CALL(sensor, requestTemperatures()).Times(4);
    EXPECT_CALL(sensor, getTempCByIndex(0)).Times(4).WillOnce(Return(0)).WillOnce(Return(-2)).WillOnce(Return(2)).WillOnce(Return(117.7));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 1000;
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        dallas.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_DOUBLE_EQ(0, dallas.getCurrentMeasurementByID());

    EXPECT_CALL(sensor, requestTemperatures()).Times(3);
    EXPECT_CALL(sensor, getTempCByIndex(0)).Times(3).WillOnce(Return(17.7)).WillOnce(Return(-82.3)).WillOnce(Return(1320));
    incrementIntervalMillis = 0;
    nextLoopMillis = 500;
    for (int i = 0; i < 5; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        dallas.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_DOUBLE_EQ(17.7, dallas.getCurrentMeasurementByID());

    releaseArduinoMock();
}

TEST_F(DallasArduinoTest, test_getTemperatures_ManySensors_ReturnsCorrectTemperatures)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    OneWire ow(17);
    DallasTemperature sensor(&ow);
    DallasArduino dallas(sensor);
    EXPECT_CALL(sensor, begin()).Times(1);
    EXPECT_CALL(sensor, getDeviceCount()).Times(1).WillOnce(Return(3));
    EXPECT_TRUE(dallas.init("sensorName"));

    EXPECT_CALL(sensor, requestTemperatures()).Times(4);
    EXPECT_CALL(sensor, getTempCByIndex(0)).Times(4).WillOnce(Return(-40.1)).WillOnce(Return(-39.9)).WillOnce(Return(-39.7)).WillOnce(Return(20));
    EXPECT_CALL(sensor, getTempCByIndex(1)).Times(4).WillOnce(Return(-0.5)).WillOnce(Return(0)).WillOnce(Return(0.5)).WillOnce(Return(17));
    EXPECT_CALL(sensor, getTempCByIndex(2)).Times(4).WillOnce(Return(6.33)).WillOnce(Return(6.35)).WillOnce(Return(6.37)).WillOnce(Return(-7.8));
    uint32_t startIntervalMillis = 8500;
    uint32_t incrementIntervalMillis = 0;
    uint16_t nextLoopMillis = 50;
    for (int i = 0; i < 7; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        dallas.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_DOUBLE_EQ(-39.9, dallas.getCurrentMeasurementByID());
    EXPECT_DOUBLE_EQ(0, dallas.getCurrentMeasurementByID(1));
    EXPECT_DOUBLE_EQ(6.35, dallas.getCurrentMeasurementByID(2));

    EXPECT_CALL(sensor, requestTemperatures()).Times(3);
    EXPECT_CALL(sensor, getTempCByIndex(0)).Times(3).WillOnce(Return(22)).WillOnce(Return(24)).WillOnce(Return(125));
    EXPECT_CALL(sensor, getTempCByIndex(1)).Times(3).WillOnce(Return(27.5)).WillOnce(Return(38)).WillOnce(Return(-125));
    EXPECT_CALL(sensor, getTempCByIndex(2)).Times(3).WillOnce(Return(-8)).WillOnce(Return(-8.2)).WillOnce(Return(0));
    incrementIntervalMillis = 0;
    nextLoopMillis = 200;
    for (int i = 0; i < 3; i++)
    {
        EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
        dallas.requestCurrentMeasurement();
        incrementIntervalMillis += nextLoopMillis;
    }
    EXPECT_DOUBLE_EQ(22, dallas.getCurrentMeasurementByID());
    EXPECT_DOUBLE_EQ(27.5, dallas.getCurrentMeasurementByID(1));
    EXPECT_DOUBLE_EQ(-8, dallas.getCurrentMeasurementByID(2));
    releaseArduinoMock();
}

TEST_F(DallasArduinoTest, test_nonExistentSensorID_ReturnsErrorValue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    OneWire ow(17);
    DallasTemperature sensor(&ow);
    DallasArduino dallas(sensor);
    EXPECT_CALL(sensor, begin()).Times(1);
    EXPECT_CALL(sensor, getDeviceCount()).Times(1).WillOnce(Return(2));
    EXPECT_TRUE(dallas.init("sensorName"));

    EXPECT_CALL(sensor, requestTemperatures()).Times(1);
    EXPECT_CALL(sensor, getTempCByIndex(_)).Times(2).WillOnce(Return(1)).WillOnce(Return(-1));
    EXPECT_CALL(*arduinoMock, millis).Times(2).WillRepeatedly(Return(1000));
    dallas.requestCurrentMeasurement();

    EXPECT_DOUBLE_EQ(-127, dallas.getCurrentMeasurementByID(2));
    EXPECT_DOUBLE_EQ(-127, dallas.getCurrentMeasurementByID(5));
    releaseArduinoMock();
}
