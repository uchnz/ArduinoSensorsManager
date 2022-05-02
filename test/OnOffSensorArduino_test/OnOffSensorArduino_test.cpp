// #include <gtest/gtest.h>
// #include <OnOffSensorArduino.h>
// #include <OnOffSensorArduino_test.h>

// using ::testing::_;
// using ::testing::AtLeast;
// using ::testing::Return;

// TEST_F(OnOffSensorArduinoTest, test_Init_ReturnsTrue)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(A0, INPUT)).Times(1);
//     MockArduinoAnalogIO io(A0, INPUT);
//     OnOffSensorArduino onoff("mois", io);
//     TimerArduino timer;
//     EXPECT_TRUE(onoff.init(&timer));

//     char name[30];
//     EXPECT_EQ(4, onoff.getName(name));
//     EXPECT_STREQ("mois", name);

//     EXPECT_TRUE(onoff.setName("new long name here"));
//     EXPECT_TRUE(onoff.init(&timer));
//     EXPECT_EQ(18, onoff.getName(name));
//     EXPECT_STREQ("new long name here", name);

//     releaseArduinoMock();
// }

// TEST_F(OnOffSensorArduinoTest, test_Init_withNameToLong_truncatesNameToSizeOfBuffer)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(A2, INPUT)).Times(1);
//     MockArduinoAnalogIO io(A2, INPUT);
//     OnOffSensorArduino onoff("new long name here, longer than buffer size", io);
//     TimerArduino timer;
//     EXPECT_TRUE(onoff.init(&timer));

//     char name[30];
//     EXPECT_EQ(29, onoff.getName(name));
//     EXPECT_STREQ("new long name here, longer th", name);
//     releaseArduinoMock();
// }

// TEST_F(OnOffSensorArduinoTest, test_Init_withoutName_ReturnsFalse)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(A3, INPUT)).Times(1);
//     MockArduinoAnalogIO io(A3, INPUT);
//     OnOffSensorArduino onoff(nullptr, io);

//     char name[30];
//     TimerArduino timer;
//     EXPECT_FALSE(onoff.init(&timer));
//     EXPECT_EQ(0, onoff.getName(name));

//     onoff.setName("");
//     EXPECT_FALSE(onoff.init(&timer));
//     EXPECT_EQ(0, onoff.getName(name));
//     releaseArduinoMock();
// }

// TEST_F(OnOffSensorArduinoTest, test_getName_beforeInit_ReturnsName)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(A4, INPUT)).Times(1);
//     MockArduinoAnalogIO io(A4, INPUT);
//     OnOffSensorArduino onoff("name", io);

//     char name[30];
//     EXPECT_EQ(4, onoff.getName(name));
//     EXPECT_STREQ("name", name);
//     releaseArduinoMock();
// }

// TEST_F(OnOffSensorArduinoTest, test_getCurrentMeasurementByID_withoutInit_ReturnsMinusOne)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(A5, INPUT)).Times(1);
//     MockArduinoAnalogIO io(A5, INPUT);
//     OnOffSensorArduino onoff(nullptr, io);

//     EXPECT_EQ(0xFFFFFFFF, onoff.getCurrentMeasurementByID());
//     releaseArduinoMock();
// }

// TEST_F(OnOffSensorArduinoTest, test_requestCurrentMeasurement_withoutInit_Fails)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(A5, INPUT)).Times(1);
//     MockArduinoAnalogIO io(A5, INPUT);
//     OnOffSensorArduino onoff("s", io);

//     onoff.requestCurrentMeasurement();
//     EXPECT_EQ(0xFFFFFFFF, onoff.getCurrentMeasurementByID());
//     releaseArduinoMock();
// }

// TEST_F(OnOffSensorArduinoTest, test_getCurrentMeasurementByID_withoutRequestCurrentMeasurement_ReturnsMinusOne)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(A5, INPUT)).Times(1);
//     MockArduinoAnalogIO io(A5, INPUT);
//     OnOffSensorArduino onoff("name", io);
//     TimerArduino timer;
//     EXPECT_TRUE(onoff.init(&timer));

//     EXPECT_CALL(*arduinoMock, analogRead(0)).Times(0);
//     EXPECT_EQ(0x8000, onoff.getCurrentMeasurementByID());
//     releaseArduinoMock();
// }

// TEST_F(OnOffSensorArduinoTest, test_requestCurrentMeasurement_withinTimeInterval_SavesArrayOfMeasurements)
// {
//     ArduinoMock *arduinoMock = arduinoMockInstance();
//     EXPECT_CALL(*arduinoMock, pinMode(A0, INPUT)).Times(1);
//     MockArduinoAnalogIO io(A0, INPUT);
//     OnOffSensorArduino onoff("sensorname", io);
//     TimerArduino timer;
//     EXPECT_TRUE(onoff.init(&timer));

//     EXPECT_CALL(*arduinoMock, analogRead(A0)).Times(4).WillOnce(Return(140)).WillOnce(Return(150)).WillOnce(Return(160)).WillOnce(Return(270));
//     uint32_t startIntervalMillis = 8500;
//     uint32_t incrementIntervalMillis = 0;
//     uint16_t nextLoopMillis = 1000;
//     for (int i = 0; i < 10; i++)
//     {
//         EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
//         onoff.requestCurrentMeasurement();
//         incrementIntervalMillis += nextLoopMillis;
//     }
//     EXPECT_EQ(150, onoff.getCurrentMeasurementByID());

//     incrementIntervalMillis = 0;
//     nextLoopMillis = 3000;
//     EXPECT_CALL(*arduinoMock, analogRead(A0)).Times(3).WillOnce(Return(280)).WillOnce(Return(290)).WillOnce(Return(300));
//     for (int i = 0; i < 3; i++)
//     {
//         EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
//         onoff.requestCurrentMeasurement();
//         incrementIntervalMillis += nextLoopMillis;
//     }
//     EXPECT_EQ(280, onoff.getCurrentMeasurementByID());
//     releaseArduinoMock();
// }

// // TEST_F(OnOffSensorArduinoTest, test_requestCurrentMeasurement_withDifferentMeasurementIntervals_SavesArrayOfMeasurements)
// // {
// //     ArduinoMock *arduinoMock = arduinoMockInstance();
// //     EXPECT_CALL(*arduinoMock, pinMode(A2, INPUT)).Times(1);
// //     MockArduinoAnalogIO io(A2, INPUT);
// //     OnOffSensorArduino onoff("sensorname", io);
// //     TimerArduino timer(1000);
// //     EXPECT_TRUE(onoff.init(&timer));

// //     EXPECT_CALL(*arduinoMock, analogRead(A2)).Times(4).WillOnce(Return(140)).WillOnce(Return(150)).WillOnce(Return(160)).WillOnce(Return(270));
// //     uint32_t startIntervalMillis = 8500;
// //     uint32_t incrementIntervalMillis = 0;
// //     uint16_t nextLoopMillis = 1000;
// //     for (int i = 0; i < 4; i++)
// //     {
// //         EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
// //         onoff.requestCurrentMeasurement();
// //         incrementIntervalMillis += nextLoopMillis;
// //     }
// //     EXPECT_EQ(150, onoff.getCurrentMeasurementByID());

// //     timer.setReadingInterval(2000);
// //     incrementIntervalMillis = 0;
// //     nextLoopMillis = 3000;
// //     EXPECT_CALL(*arduinoMock, analogRead(A2)).Times(4).WillOnce(Return(280)).WillOnce(Return(290)).WillOnce(Return(300)).WillOnce(Return(300));
// //     for (int i = 0; i < 4; i++)
// //     {
// //         EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
// //         onoff.requestCurrentMeasurement();
// //         incrementIntervalMillis += nextLoopMillis;
// //     }
// //     EXPECT_EQ(280, onoff.getCurrentMeasurementByID());
// //     releaseArduinoMock();
// // }

// /////////////////////////////////////////////////////

// // #include <gtest/gtest.h>
// // #include <OnOffSensorArduino.h>
// // #include <OnOffSensorArduino_test.h>

// // using ::testing::_;
// // using ::testing::AtLeast;
// // using ::testing::Return;

// // TEST_F(OnOffSensorArduinoTest, test_Init_CallsPinMode_toSetPinToInput_ReturnsTrue)
// // {
// //     ArduinoMock *arduinoMock = arduinoMockInstance();
// //     EXPECT_CALL(*arduinoMock, pinMode(2, INPUT_PULLUP)).Times(1);
// //     OnOffSensorArduino onoff(2);

// //     EXPECT_TRUE(onoff.init("sensorName"));
// //     releaseArduinoMock();
// // }

// // TEST_F(OnOffSensorArduinoTest, test_Init_FailsWithoutPIN_ReturnsFalse)
// // {
// //     OnOffSensorArduino onoff;

// //     EXPECT_FALSE(onoff.init("sensorName"));
// // }

// // TEST_F(OnOffSensorArduinoTest, test_SetPINWithNonZero_allowsInitReturnTrue)
// // {
// //     ArduinoMock *arduinoMock = arduinoMockInstance();
// //     OnOffSensorArduino onoff;
// //     EXPECT_FALSE(onoff.init("sensorName"));

// //     EXPECT_CALL(*arduinoMock, pinMode(13, INPUT_PULLUP)).Times(1);
// //     onoff.SetPIN(13);

// //     EXPECT_TRUE(onoff.init("sensorName"));
// //     releaseArduinoMock();
// // }

// // TEST_F(OnOffSensorArduinoTest, test_getCurrentMeasurementByID_withoutInit_ReturnsMinusOne)
// // {
// //     OnOffSensorArduino onoff;
// //     onoff.SetPIN(4);

// //     EXPECT_EQ(onoff.getCurrentMeasurementByID(), -1);
// // }

// // TEST_F(OnOffSensorArduinoTest, test_SetPIN_resetsInitStatus)
// // {
// //     ArduinoMock *arduinoMock = arduinoMockInstance();
// //     OnOffSensorArduino onoff(1);
// //     EXPECT_CALL(*arduinoMock, pinMode(1, INPUT_PULLUP)).Times(1);
// //     EXPECT_TRUE(onoff.init("sensorName"));
// //     onoff.SetPIN(4);

// //     EXPECT_EQ(onoff.getCurrentMeasurementByID(), -1);
// //     releaseArduinoMock();
// // }

// // TEST_F(OnOffSensorArduinoTest, test_requestCurrentMeasurement_withoutInit_Fails)
// // {
// //     OnOffSensorArduino onoff;
// //     onoff.requestCurrentMeasurement();
// //     EXPECT_EQ(onoff.getCurrentMeasurementByID(), -1);

// //     onoff.SetPIN(13);
// //     EXPECT_EQ(onoff.getCurrentMeasurementByID(), -1);
// // }

// // TEST_F(OnOffSensorArduinoTest, test_getCurrentMeasurementByID_withoutRequestCurrentMeasurement_ReturnsMinusOne)
// // {
// //     ArduinoMock *arduinoMock = arduinoMockInstance();
// //     OnOffSensorArduino onoff(2);
// //     EXPECT_CALL(*arduinoMock, pinMode(2, INPUT_PULLUP)).Times(1);
// //     EXPECT_TRUE(onoff.init("sensorName"));

// //     EXPECT_CALL(*arduinoMock, digitalRead(0)).Times(0);
// //     EXPECT_EQ(onoff.getCurrentMeasurementByID(), -1);

// //     releaseArduinoMock();
// // }

// // TEST_F(OnOffSensorArduinoTest, test_requestCurrentMeasurement_withinTimeInterval_SavesArrayOfMeasurements)
// // {
// //     ArduinoMock *arduinoMock = arduinoMockInstance();
// //     OnOffSensorArduino onoff(2);
// //     EXPECT_CALL(*arduinoMock, pinMode(2, INPUT_PULLUP)).Times(1);
// //     EXPECT_TRUE(onoff.init("sensorName"));

// //     EXPECT_CALL(*arduinoMock, digitalRead(2)).Times(4).WillOnce(Return(HIGH)).WillOnce(Return(LOW)).WillOnce(Return(HIGH)).WillOnce(Return(LOW));
// //     uint32_t startIntervalMillis = 8500;
// //     uint32_t incrementIntervalMillis = 0;
// //     uint16_t nextLoopMillis = 500;
// //     for (int i = 0; i < 4; i++)
// //     {
// //         EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
// //         onoff.requestCurrentMeasurement();
// //         incrementIntervalMillis += nextLoopMillis;
// //     }
// //     EXPECT_EQ(1, onoff.getCurrentMeasurementByID());

// //     incrementIntervalMillis = 0;
// //     nextLoopMillis = 1000;
// //     EXPECT_CALL(*arduinoMock, digitalRead(2)).Times(3).WillOnce(Return(LOW)).WillOnce(Return(HIGH)).WillOnce(Return(LOW));
// //     for (int i = 0; i < 3; i++)
// //     {
// //         EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
// //         onoff.requestCurrentMeasurement();
// //         incrementIntervalMillis += nextLoopMillis;
// //     }
// //     EXPECT_EQ(0, onoff.getCurrentMeasurementByID());

// //     releaseArduinoMock();
// // }

// // TEST_F(OnOffSensorArduinoTest, test_requestCurrentMeasurement_withDifferentMeasurementIntervals_SavesArrayOfMeasurements)
// // {
// //     ArduinoMock *arduinoMock = arduinoMockInstance();
// //     OnOffSensorArduino onoff(2);
// //     EXPECT_CALL(*arduinoMock, pinMode(2, INPUT_PULLUP)).Times(1);
// //     EXPECT_TRUE(onoff.init("sensorName", 100));

// //     EXPECT_CALL(*arduinoMock, digitalRead(2)).Times(4).WillOnce(Return(LOW)).WillOnce(Return(HIGH)).WillOnce(Return(LOW)).WillOnce(Return(HIGH));
// //     uint32_t startIntervalMillis = 8500;
// //     uint32_t incrementIntervalMillis = 0;
// //     uint16_t nextLoopMillis = 50;
// //     for (int i = 0; i < 7; i++)
// //     {
// //         EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
// //         onoff.requestCurrentMeasurement();
// //         incrementIntervalMillis += nextLoopMillis;
// //     }
// //     EXPECT_EQ(0, onoff.getCurrentMeasurementByID());

// //     onoff.SetPIN(4);
// //     EXPECT_CALL(*arduinoMock, pinMode(4, INPUT_PULLUP)).Times(1);
// //     EXPECT_TRUE(onoff.init("sensorName", 10));
// //     incrementIntervalMillis = 0;
// //     nextLoopMillis = 30;
// //     EXPECT_CALL(*arduinoMock, digitalRead(4)).Times(4).WillOnce(Return(LOW)).WillOnce(Return(HIGH)).WillOnce(Return(LOW)).WillOnce(Return(LOW));
// //     for (int i = 0; i < 4; i++)
// //     {
// //         EXPECT_CALL(*arduinoMock, millis).Times(AtLeast(0)).WillRepeatedly(Return(startIntervalMillis + incrementIntervalMillis));
// //         onoff.requestCurrentMeasurement();
// //         incrementIntervalMillis += nextLoopMillis;
// //     }
// //     EXPECT_EQ(0, onoff.getCurrentMeasurementByID());

// //     releaseArduinoMock();
// // }

// // // TEST_F(OnOffSensorArduinoTest, DISABLED_test_requestCurrentMeasurement_WhenToShortReadingInterval_ReturnErrorValue)
// // // {
// // //     FAIL();
// // // }