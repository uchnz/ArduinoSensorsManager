#include <gtest/gtest.h>
#include <MQ7COArduino_test.h>
#include <Arduino.h>
#include <MQ7COArduino.h>

using ::testing::_;
// using ::testing::Return;

TEST_F(MQ7COArduinoTest, test_initSetsPinsToOUT)
{
    ArduinoMock *arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_, _)).Times(2);

    MQ7COArduino mq7co(2, 3);
    mq7co.init();
    releaseArduinoMock();
}
