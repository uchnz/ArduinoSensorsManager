#include <gtest/gtest.h>
#include <IOActuator_test.h>
#include <IOActuator.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(IOActuatorTest, test_1)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    //     // EXPECT_CALL(*arduinoMock, pinMode(A0, INPUT_PULLUP)).Times(0);
    //     // AnalogIOArduino aio(A0, INPUT_PULLUP);
    // EXPECT_CALL(*arduinoMock, pinMode(10, INPUT)).Times(1);
    IOActuator io(10);
    EXPECT_TRUE(io.init());

    // EXPECT_CALL(*arduinoMock, digitalWrite(10)).Times(1);
    EXPECT_TRUE(io.write(1));
    releaseArduinoMock();
}