#include <gtest/gtest.h>
#include <IOArduino.h>
#include <IOArduino_test.h>

using ::testing::Return;

TEST_F(ArduinoIOTest, test_analogRead)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(0, INPUT)).Times(1);
    AnalogIOArduino io(0, INPUT);

    EXPECT_CALL(*arduinoMock, analogRead(0)).Times(1).WillOnce(Return(10));
    int result = io.read();

    EXPECT_EQ(10, result);
    releaseArduinoMock();
}

TEST_F(ArduinoIOTest, test_digitalRead)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(10, INPUT)).Times(1);
    DigitalIOArduino io(10, INPUT);

    EXPECT_CALL(*arduinoMock, digitalRead(10)).Times(1).WillOnce(Return(0));
    int result = io.read();
    EXPECT_EQ(0, result);

    EXPECT_CALL(*arduinoMock, digitalRead(10)).Times(1).WillOnce(Return(1));
    result = io.read();
    EXPECT_EQ(1, result);

    releaseArduinoMock();
}