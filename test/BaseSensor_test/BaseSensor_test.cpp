
#include <gtest/gtest.h>
// #include <IOArduino.h>
#include <BaseSensor_test.h>

using ::testing::_;
using ::testing::Return;

TEST_F(BaseSensorTest, test_name)
{
    char name[30];
    MockIO io;
    BaseSensor sensor2("namezzzzz", io);
    EXPECT_EQ(9, sensor2.getName(name));
    EXPECT_STREQ("namezzzzz", name);

    BaseSensor sensor("testname", io);
    EXPECT_EQ(8, sensor.getName(name));
    EXPECT_STREQ("testname", name);
}

TEST_F(BaseSensorTest, test_emptyName_ReturnsZeroAndEmptyString)
{
    char name[30];
    MockIO io;
    BaseSensor sensor2("", io);
    EXPECT_EQ(0, sensor2.getName(name));
    EXPECT_STREQ("", name);
}

TEST_F(BaseSensorTest, test_nullName_ReturnsZeroAndEmptyString)
{
    char name[30];
    MockIO io;
    BaseSensor sensor2(nullptr, io);
    EXPECT_EQ(0, sensor2.getName(name));
    EXPECT_STREQ("", name);
}

TEST_F(BaseSensorTest, test_toBigName_TruncatesToMaxSize)
{
    char name[30];
    MockIO io;
    BaseSensor sensor2("Name is to big to fit into member var", io);
    EXPECT_EQ(29, sensor2.getName(name));
    EXPECT_STREQ("Name is to big to fit into me", name);
}

TEST_F(BaseSensorTest, test_getName_withNullptr)
{
    char *name = nullptr;
    MockIO io;
    BaseSensor sensor2("Name", io);
    EXPECT_EQ(0, sensor2.getName(name));
    EXPECT_EQ(nullptr, name);
}

TEST_F(BaseSensorTest, test_setName_savesCorrectName)
{
    char name[30];
    MockIO io;
    BaseSensor sensor("old name", io);
    EXPECT_EQ(8, sensor.getName(name));
    EXPECT_STREQ("old name", name);

    EXPECT_TRUE(sensor.setName("correct name"));

    EXPECT_EQ(12, sensor.getName(name));
    EXPECT_STREQ("correct name", name);
}

TEST_F(BaseSensorTest, test_setName_withNull_changesNothing)
{
    char name[30];
    MockIO io;
    BaseSensor sensor("prev name", io);
    EXPECT_FALSE(sensor.setName(nullptr));
    EXPECT_EQ(9, sensor.getName(name));
    EXPECT_STREQ("prev name", name);
}

TEST_F(BaseSensorTest, test_setName_withEmptyString_changesNothing)
{
    char name[30];
    MockIO io;
    BaseSensor sensor("prev name", io);
    EXPECT_FALSE(sensor.setName(""));
    EXPECT_EQ(9, sensor.getName(name));
    EXPECT_STREQ("prev name", name);
}

TEST_F(BaseSensorTest, test_setName_withToBigName_TruncatesToMaxSize)
{
    char name[30];
    MockIO io;
    BaseSensor sensor("s", io);
    EXPECT_EQ(1, sensor.getName(name));

    EXPECT_TRUE(sensor.setName("Name is to big to fit into member var"));
    EXPECT_EQ(29, sensor.getName(name));
    EXPECT_STREQ("Name is to big to fit into me", name);
}

TEST_F(BaseSensorTest, test_init_whenNoname_Fails)
{
    MockIO io;
    BaseSensor sensor("", io);
    MockTimer timer;

    EXPECT_FALSE(sensor.init(&timer));
}

TEST_F(BaseSensorTest, test_init_whenSensorHasName_Successful)
{
    MockIO io;
    EXPECT_CALL(io, getTotalSensors()).Times(3).WillRepeatedly(Return(1));
    EXPECT_CALL(io, init()).Times(1).WillOnce(Return(true));
    BaseSensor sensor("with name", io);
    MockTimer timer;

    EXPECT_TRUE(sensor.init(&timer));
}

// TEST_F(BaseSensorTest, test_init_afterNameChanged_Successful)
// {
//     // MockIO io;
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(A2, INPUT)).Times(1);
//     AnalogIOArduino io(A2, INPUT);
//     BaseSensor sensor("with name", io);
//     MockTimer timer;
//     // EXPECT_CALL(io, init()).Times(1).WillOnce(Return(true));
//     // EXPECT_TRUE(sensor.init(&timer));
//     sensor.setName("new name");
//     // EXPECT_CALL(io, init()).Times(1).WillOnce(Return(true));
//     // EXPECT_TRUE(sensor.init(&timer));

//     BaseSensor sensor2(nullptr, io);
//     // EXPECT_CALL(io, init()).Times(0);
//     EXPECT_FALSE(sensor2.init(&timer));
//     // EXPECT_CALL(io, init()).Times(1).WillOnce(Return(true));
//     sensor2.setName("n");
//     EXPECT_TRUE(sensor2.init(&timer));
//     releaseArduinoMock();
// }

TEST_F(BaseSensorTest, test_init_withNullTimer_returnsFalse)
{
    MockIO io;
    BaseSensor sensor("sensorname", io);

    EXPECT_FALSE(sensor.init(nullptr));
}

TEST_F(BaseSensorTest, test_getNumberOfSensors_withoutInit_ReturnsErrorNumber)
{
    MockIO io;
    BaseSensor sensor("", io);
    EXPECT_EQ(basesensor_nm::NOT_INITIALIZED_SENSOR, sensor.getNumberOfConnectedSensors());

    BaseSensor sensor2(nullptr, io);
    EXPECT_EQ(basesensor_nm::NOT_INITIALIZED_SENSOR, sensor2.getNumberOfConnectedSensors());

    BaseSensor sensor3("with name", io);
    EXPECT_EQ(basesensor_nm::NOT_INITIALIZED_SENSOR, sensor3.getNumberOfConnectedSensors());
}

// TEST_F(BaseSensorTest, test_getNumberOfSensors_afterInit_ReturnsZero)
// {
//     MockIO io;
//     BaseSensor sensor2("with name", io);
//     MockTimer timer;
//     EXPECT_CALL(io, init()).Times(1).WillOnce(Return(true));
//     sensor2.init(&timer);
//     EXPECT_EQ(0, sensor2.getNumberOfConnectedSensors());
// }

TEST_F(BaseSensorTest, test_getCurrentMeasurement_withoutInit_Fails)
{
    MockIO io;
    BaseSensor sensor("", io);
    MockTimer timer;
    EXPECT_CALL(io, init()).Times(0);
    EXPECT_FALSE(sensor.init(&timer));
    EXPECT_EQ(0xFFFFFFFF, sensor.getCurrentMeasurementByID());

    EXPECT_CALL(io, init()).Times(0);
    EXPECT_FALSE(sensor.init(nullptr));
    EXPECT_EQ(0xFFFFFFFF, sensor.getCurrentMeasurementByID());
}

TEST_F(BaseSensorTest, test_requestCurrentMeasurement_withoutInit_Fails)
{
    MockIO io;
    BaseSensor sensor("s", io);
    MockTimer timer;
    EXPECT_CALL(io, init()).Times(0);
    EXPECT_FALSE(sensor.init(nullptr));

    EXPECT_FALSE(sensor.requestCurrentMeasurement());
}

// TEST_F(BaseSensorTest, test_requestCurrentMeasurement_withNullTimerAfterInit_returnsFalse)
// {
//     MockIO io;
//     EXPECT_CALL(io, getTotalSensors()).Times(2).WillRepeatedly(Return(1));
//     MockTimer *timer = new MockTimer();
//     BaseSensor sensor("sensorname", io);
//     EXPECT_CALL(io, init()).Times(1).WillOnce(Return(true));
//     EXPECT_TRUE(sensor.init(timer));
//     delete timer;

//     // can't test for deleted pointer before calling isReadyForNextRead, because of using raw pointers
//     // _timer contains address to deleted object in this case
//     // change code to use std::unique_ptr instead

//     // EXPECT_CALL(*timer, isReadyForNextRead).Times(0);
//     // EXPECT_FALSE(sensor.requestCurrentMeasurement());
// }

////////////////////////////
TEST_F(BaseSensorTest, test_requestCurrentMeasurement_withinTimeInterval_returnsTrue)
{
    MockIO io;
    MockTimer timer;
    BaseSensor sensor("sensorname", io);
    EXPECT_CALL(io, init()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(io, getTotalSensors()).Times(2).WillRepeatedly(Return(1));
    sensor.init(&timer);

    EXPECT_CALL(timer, isReadyForNextRead).Times(2).WillOnce(Return(false)).WillOnce(Return(true));
    EXPECT_CALL(io, getTotalSensors()).Times(0);
    EXPECT_CALL(io, read(_)).Times(0);
    EXPECT_FALSE(sensor.requestCurrentMeasurement());
    EXPECT_CALL(io, getTotalSensors()).Times(2).WillRepeatedly(Return(1));
    EXPECT_CALL(io, read(_)).Times(1).WillOnce(Return(false));
    EXPECT_TRUE(sensor.requestCurrentMeasurement());
}

// TEST_F(BaseSensorTest, test_getCurrentMeasurement_afterInit_ReturnsZero)
// {
//     MockIO io;
//     BaseSensor sensor("www", io);
//     MockTimer timer;
//     EXPECT_CALL(io, init()).Times(1).WillOnce(Return(true));
//     EXPECT_TRUE(sensor.init(&timer));
//     EXPECT_EQ(0, sensor.getCurrentMeasurementByID());
// }
