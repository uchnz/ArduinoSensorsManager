#include <gtest/gtest.h>
#include <MultiLevel_test.h>
#include <BaseSensor.h>

// using ::testing::_;
// using ::testing::AtLeast;
// using ::testing::Return;

// TEST_F(MultiPortTest, test_nameIsSavedCorrectly)
// {
//     char name[30];
//     MultiLevelArduino mlsensor("multiLevel", nullptr, 0);
//     EXPECT_EQ(10, mlsensor.getName(name));
//     EXPECT_STREQ("multiLevel", name);
// }

TEST_F(MultiPortTest, test_sensor_Has_4_IOs)
{
    char name[30];
    MockIO io1;
    MockIO io2;
    MockIO io3;
    MockIO io4;
    IIOSensor *ios[4] = {&io1, &io2, &io3, &io4};

    MultiLevelArduino mlsensor("multiLevel3", ios, 4);
    uint8_t num = mlsensor.getNumberOfConnectedSensors();
    EXPECT_EQ(4, num);
}

// TEST_F(MultiPortTest, test_sensor_Has_1_IOs)
// {
//     char name[30];
//     MockIO io1;

//     MultiLevelArduino mlsensor("multiLevel3", io1);
//     uint8_t num = mlsensor.getNumberOfConnectedSensors();
//     EXPECT_EQ(1, num);
// }

TEST_F(MultiPortTest, test_multi_io)
{
    MultiPortIOArduino mio(nullptr, 2);
    uint8_t num = mio.getTotalSensors();
    EXPECT_EQ(2, num);
}

TEST_F(MultiPortTest, test_multi_io_compound)
{
    DigitalIOArduino dio1(32, INPUT_PULLUP);
    AnalogIOArduino aio2(A10, INPUT);
    AnalogIOArduino aio3(A11, INPUT);
    IIOSensor *ios[3] = {&dio1, &aio2, &aio3};

    MultiPortIOArduino mio(ios, 3);
    uint8_t num = mio.getTotalSensors();
    EXPECT_EQ(3, num);
}
