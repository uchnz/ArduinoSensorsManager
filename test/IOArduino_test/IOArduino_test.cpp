#include <gtest/gtest.h>
#include <IOArduino.h>
#include <SHTIOArduino.h>
#include <BMP280IOArduino.h>
#include <MQ7COIOArduino.h>
#include <PhaseTimerArduino.h>
#include <IOArduino_test.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(IOArduinoTest, test_analogRead_beforeInit_returnsError)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(A0, INPUT_PULLUP)).Times(0);
    AnalogIOArduino aio(A0, INPUT_PULLUP);

    EXPECT_CALL(*arduinoMock, analogRead(A0)).Times(0);
    double result = aio.read();

    EXPECT_EQ(0xFFFFFFFF, result);
    releaseArduinoMock();
}

TEST_F(IOArduinoTest, test_analogRead_afterInit_returnsValue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    AnalogIOArduino io(A10, INPUT);
    EXPECT_CALL(*arduinoMock, pinMode(A10, INPUT)).Times(1);
    EXPECT_TRUE(io.init());

    EXPECT_CALL(*arduinoMock, analogRead(A10)).Times(1).WillOnce(Return(10));
    double result = io.read();

    EXPECT_EQ(10, result);
    releaseArduinoMock();
}

TEST_F(IOArduinoTest, test_digitalRead_beforeInit_returnsError)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(7, INPUT)).Times(0);
    DigitalIOArduino dio(7, INPUT);

    EXPECT_CALL(*arduinoMock, analogRead(A0)).Times(0);
    double result = dio.read();

    EXPECT_EQ(0xFFFFFFFF, result);
    releaseArduinoMock();
}

TEST_F(IOArduinoTest, test_digitalRead_afterInit_returnsValue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    DigitalIOArduino io(32, INPUT_PULLUP);
    EXPECT_CALL(*arduinoMock, pinMode(32, INPUT_PULLUP)).Times(1);
    EXPECT_TRUE(io.init());

    EXPECT_CALL(*arduinoMock, digitalRead(32)).Times(1).WillOnce(Return(0));
    double result = io.read();
    EXPECT_EQ(0, result);

    EXPECT_CALL(*arduinoMock, digitalRead(32)).Times(1).WillOnce(Return(1));
    result = io.read();
    EXPECT_EQ(1, result);
    releaseArduinoMock();
}

TEST_F(SHTIOArduinoTest, test_read2Sensors_beforeInit_returnErrors)
{
    iarduino_I2C_SHT shtI2C(0x09);
    EXPECT_CALL(shtI2C, begin()).Times(0);
    SHTIOArduino io(shtI2C);

    EXPECT_CALL(shtI2C, getTem()).Times(0);
    double result = io.read();
    EXPECT_EQ(0xFFFFFFFF, result);

    EXPECT_CALL(shtI2C, getHum()).Times(0);
    result = io.read(1);
    EXPECT_EQ(0xFFFFFFFF, result);
}

TEST_F(SHTIOArduinoTest, test_readSensorAboveID1_returnsErrorValue)
{
    iarduino_I2C_SHT shtI2C(0x09);
    EXPECT_CALL(shtI2C, begin()).Times(1).WillOnce(Return(true));
    SHTIOArduino io(shtI2C);
    io.init();

    EXPECT_CALL(shtI2C, getTem()).Times(0);
    EXPECT_CALL(shtI2C, getHum()).Times(0);
    double result = io.read(2);

    EXPECT_EQ(0xFFFFFFFF, result);
}

TEST_F(SHTIOArduinoTest, test_read2Sensors_afterInit_returnValues)
{
    iarduino_I2C_SHT shtI2C(0x09);
    EXPECT_CALL(shtI2C, begin()).Times(1).WillOnce(Return(true));
    SHTIOArduino io(shtI2C);
    EXPECT_TRUE(io.init());

    EXPECT_CALL(shtI2C, getTem()).Times(1).WillOnce(Return(-36.6));
    double result = io.read();
    EXPECT_EQ(-36.6, result);

    EXPECT_CALL(shtI2C, getHum()).Times(1).WillOnce(Return(54.20));
    result = io.read(1);
    EXPECT_EQ(54.20, result);
}

TEST_F(BMP280IOArduinoTest, test_read3Sensors_beforeInit_returnErrors)
{
    iarduino_Pressure_BMP bmpI2C(0x09);
    EXPECT_CALL(bmpI2C, begin()).Times(0);
    BMP280IOArduino io(bmpI2C);

    EXPECT_CALL(bmpI2C, read()).Times(0);
    double result = io.read(0);
    EXPECT_EQ(0xFFFFFFFF, result);

    EXPECT_CALL(bmpI2C, read()).Times(0);
    result = io.read(1);
    EXPECT_EQ(0xFFFFFFFF, result);

    EXPECT_CALL(bmpI2C, read()).Times(0);
    result = io.read(2);
    EXPECT_EQ(0xFFFFFFFF, result);
}

TEST_F(BMP280IOArduinoTest, test_readSensorAboveID2_returnsErrorValue)
{
    iarduino_Pressure_BMP bmpI2C(0x09);
    EXPECT_CALL(bmpI2C, begin()).Times(1).WillOnce(Return(true));
    BMP280IOArduino io(bmpI2C);
    io.init();

    EXPECT_CALL(bmpI2C, read()).Times(0);
    double result = io.read(3);

    EXPECT_EQ(0xFFFFFFFF, result);
}

TEST_F(BMP280IOArduinoTest, test_read3Sensors_afterInit_returnValues)
{
    iarduino_Pressure_BMP bmpI2C(0x09);
    bmpI2C.temperature = 36.6;
    bmpI2C.pressure = 756;
    bmpI2C.altitude = -0.5;
    EXPECT_CALL(bmpI2C, begin()).Times(1).WillOnce(Return(true));
    BMP280IOArduino io(bmpI2C);
    EXPECT_TRUE(io.init());

    EXPECT_CALL(bmpI2C, read()).Times(1).WillOnce(Return(true));
    double result = io.read(0);
    EXPECT_FLOAT_EQ(36.6, result);

    EXPECT_CALL(bmpI2C, read()).Times(1).WillOnce(Return(true));
    result = io.read(1);
    EXPECT_EQ(756, result);

    EXPECT_CALL(bmpI2C, read()).Times(1).WillOnce(Return(true));
    result = io.read(2);
    EXPECT_FLOAT_EQ(-0.5, result);
}

TEST_F(BMP280IOArduinoTest, test_read3Sensors_withFailRead_returnErrors)
{
    iarduino_Pressure_BMP bmpI2C(0x09);
    EXPECT_CALL(bmpI2C, begin()).Times(1).WillOnce(Return(true));
    BMP280IOArduino io(bmpI2C);
    EXPECT_TRUE(io.init());

    EXPECT_CALL(bmpI2C, read()).Times(1).WillOnce(Return(false));
    double result = io.read(0);
    EXPECT_EQ(0xFFFFFFFF, result);

    EXPECT_CALL(bmpI2C, read()).Times(1).WillOnce(Return(false));
    result = io.read(1);
    EXPECT_EQ(0xFFFFFFFF, result);

    EXPECT_CALL(bmpI2C, read()).Times(1).WillOnce(Return(false));
    result = io.read(2);
    EXPECT_EQ(0xFFFFFFFF, result);
}

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