#include <gtest/gtest.h>
#include <IOActuator_test.h>
#include <IOActuator.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(IOActuatorTest, test_write_afterInit_setsVoltage_ReturnsTrue)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(10, OUTPUT)).Times(1);
    IOActuator io(10);
    EXPECT_TRUE(io.init());

    EXPECT_CALL(*arduinoMock, digitalWrite(10, LOW)).Times(1);
    EXPECT_TRUE(io.write(0));

    EXPECT_CALL(*arduinoMock, digitalWrite(10, HIGH)).Times(1);
    EXPECT_TRUE(io.write(1));
    releaseArduinoMock();
}

TEST_F(IOActuatorTest, test_write_beforeInit_doesNothing_returnsFalse)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(5, OUTPUT)).Times(0);
    IOActuator io(5);

    EXPECT_CALL(*arduinoMock, digitalWrite(5, LOW)).Times(0);
    EXPECT_FALSE(io.write(0));

    EXPECT_CALL(*arduinoMock, digitalWrite(5, HIGH)).Times(0);
    EXPECT_FALSE(io.write(1));
    releaseArduinoMock();
}

TEST_F(IOActuatorTest, test_write_withIncorrectArgument_doesNothing_returnsFalse)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(27, OUTPUT)).Times(1);
    IOActuator io(27);
    EXPECT_TRUE(io.init());

    EXPECT_CALL(*arduinoMock, digitalWrite(27, LOW)).Times(0);
    EXPECT_FALSE(io.write(7));
}