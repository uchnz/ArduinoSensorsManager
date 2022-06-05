#include <gtest/gtest.h>
#include <SensorManager_test.h>

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::HasSubstr;
using ::testing::Return;
using ::testing::SetArrayArgument;

// TEST_F(SensorManagerTest, test_fillAddressArray_WithNull_ReturnsFalse)
// {
//     const char **addresses = nullptr;
//     bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 5);

//     EXPECT_FALSE(result);
// }

// TEST_F(SensorManagerTest, test_fillAddressArray_WithZeroQuantity_ReturnsFalse)
// {
//     const char *addresses[] = {"/UZV1/temp1", "/UZV2/temp1\n"};
//     bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 0);

//     EXPECT_FALSE(result);
// }

// TEST_F(SensorManagerTest, test_AddressArray_IsFilledCorrectly)
// {
//     const char *addresses[] = {"/UZV1/temp1", "/UZV2/temp1", "Third Topic, the long one"};
//     bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 3);

//     EXPECT_TRUE(result);

//     char actual[100];
//     _mgr.getAddressToSendToByID(0, actual);
//     EXPECT_STREQ(actual, addresses[0]);

//     _mgr.getAddressToSendToByID(1, actual);
//     EXPECT_STREQ(actual, addresses[1]);

//     _mgr.getAddressToSendToByID(2, actual);
//     EXPECT_STREQ(actual, addresses[2]);
// }

// TEST_F(SensorManagerTest, test_GetAddress_ByFirstAndLastID_ReturnsFirstAndLastAddresses)
// {
//     const char *addresses[] = {"/UZV1/someaddr", "/Some topic/ in /the middle///", "more in /the middle", "/UZV2/temp1/topichere"};
//     bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 4);

//     EXPECT_TRUE(result);

//     char actual[100];
//     _mgr.getAddressToSendToByID(0, actual);
//     EXPECT_STREQ(actual, addresses[0]);
//     _mgr.getAddressToSendToByID(3, actual);
//     EXPECT_STREQ(actual, addresses[3]);
// }

// TEST_F(SensorManagerTest, test_GetAddressAboveUpperlimit_ReturnsEmptyString)
// {
//     const char *addresses[] = {"/UZV1/someaddr", "/UZV2/temp1/topichere"};
//     bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 2);

//     EXPECT_TRUE(result);

//     char actual[100] = "";
//     _mgr.getAddressToSendToByID(5, actual);
//     EXPECT_STREQ(actual, "");
// }

// TEST_F(SensorManagerTest, test_GetAddress_WhenArrayIsNotFilled_ReturnsNull)
// {
//     char actual[100] = "";
//     _mgr.getAddressToSendToByID(0, actual);
//     EXPECT_STREQ(actual, "");

//     _mgr.getAddressToSendToByID(3, actual);
//     EXPECT_STREQ(actual, "");
// }

// TEST_F(SensorManagerTest, test_getTotalNumberOfOccupiedPINs_BeforeInit_ReturnsZero)
// {
//     uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
//     EXPECT_EQ(0, num);
// }

// TEST_F(SensorManagerTest, test_initMeasurementsArray2D_WithOutOfBoundaryQuantity_ReturnsFalse)
// {
//     // Can't check for: Out Of Boundary!!
//     // Need to refactor code of the function to type safe pointers like string, vector, etc.
//     // But in this project adding STL library to Arduino yeilds to a conflict with MQTT librarary
//     // and code doesn't compile:
//     // MockIDallas d1;
//     // EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
//     // IDallas *array[1] = {&d1};
//     // EXPECT_TRUE(_mgr.init(array, 2));
// }

// // TEST_F(SensorManagerTest, test_initMeasurementsArray2D_WithOneSensorOnEveryPIN)
// // {
// //     MockSensor d1;
// //     MockSensor d2;
// //     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
// //     EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
// //     char sensorName1[] = "IDS1_temp";
// //     // EXPECT_CALL(d1, getName(_)).WillOnce(DoAll(SetArgReferee<0>(sensorName1), Return(9)));
// //     EXPECT_CALL(d1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName1, sensorName1 + 10), Return(9)));
// //     char sensorName2[] = "ID2_pressure";
// //     EXPECT_CALL(d2, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName2, sensorName2 + 13), Return(12)));

// //     ISensor *array[2] = {&d1, &d2};
// //     EXPECT_TRUE(_mgr.init(array, 2));

// //     uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
// //     EXPECT_EQ(2, num);
// //     double actual = _mgr.getCurrentMeasurementOfOneSenorByID(0);
// //     EXPECT_DOUBLE_EQ(-128, actual);
// //     actual = _mgr.getCurrentMeasurementOfOneSenorByID(1, 0);
// //     EXPECT_DOUBLE_EQ(-128, actual);
// // }

// // TEST_F(SensorManagerTest, test_initMeasurementsArray2D_WithManySensorsOnSomePINs)
// // {
// //     MockSensor d1;
// //     MockSensor d2;
// //     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
// //     EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));
// //     char sensorName1[] = "temp1";
// //     EXPECT_CALL(d1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName1, sensorName1 + 6), Return(5)));
// //     char sensorName2[] = "ID2/pressure/moisure";
// //     EXPECT_CALL(d2, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName2, sensorName2 + 21), Return(20)));

// //     ISensor *array[2] = {&d1, &d2};
// //     EXPECT_TRUE(_mgr.init(array, 2));

// //     uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
// //     EXPECT_EQ(2, num);
// //     double actual = _mgr.getCurrentMeasurementOfOneSenorByID(0);
// //     EXPECT_DOUBLE_EQ(-128, actual);
// //     actual = _mgr.getCurrentMeasurementOfOneSenorByID(1);
// //     EXPECT_DOUBLE_EQ(-128, actual);
// //     actual = _mgr.getCurrentMeasurementOfOneSenorByID(1, 1);
// //     EXPECT_DOUBLE_EQ(-128, actual);
// // }

// TEST_F(SensorManagerTest, test_initMeasurementsArray2D_WithOneOfISensorObjectsIsNull)
// {
//     MockSensor d1;
//     MockSensor d2;
//     MockSensor d3;
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
//     EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(0);
//     EXPECT_CALL(d3, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));
//     char sensorName1[] = "s1";
//     EXPECT_CALL(d1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName1, sensorName1 + 3), Return(2)));
//     char sensorName3[] = "s3";
//     EXPECT_CALL(d3, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName3, sensorName3 + 3), Return(2)));

//     ISensor *array[3] = {&d1, nullptr, &d3};
//     EXPECT_TRUE(_mgr.init(array, 3));

//     uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
//     EXPECT_EQ(3, num);
//     double actual = _mgr.getCurrentMeasurementOfOneSenorByID(0);
//     EXPECT_DOUBLE_EQ(-128, actual);
//     actual = _mgr.getCurrentMeasurementOfOneSenorByID(1, 0);
//     EXPECT_DOUBLE_EQ(-128, actual);
//     actual = _mgr.getCurrentMeasurementOfOneSenorByID(2, 0);
//     EXPECT_DOUBLE_EQ(-128, actual);
//     actual = _mgr.getCurrentMeasurementOfOneSenorByID(2, 1);
//     EXPECT_DOUBLE_EQ(-128, actual);
// }

// TEST_F(SensorManagerTest, test_initMeasurementsArray2D_WithOneOfObjectsIsNotNull_ReturnsTrue)
// {
//     MockSensor d1;
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));
//     char sensorName[] = "s4";
//     EXPECT_CALL(d1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName, sensorName + 3), Return(2)));
//     ISensor *array[4] = {nullptr, nullptr, nullptr, &d1};

//     EXPECT_TRUE(_mgr.init(array, 4));

//     uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
//     EXPECT_EQ(4, num);
//     double actual = _mgr.getCurrentMeasurementOfOneSenorByID(0);
//     EXPECT_DOUBLE_EQ(-128, actual);
//     actual = _mgr.getCurrentMeasurementOfOneSenorByID(1);
//     EXPECT_DOUBLE_EQ(-128, actual);
//     actual = _mgr.getCurrentMeasurementOfOneSenorByID(3);
//     EXPECT_DOUBLE_EQ(-128, actual);
//     actual = _mgr.getCurrentMeasurementOfOneSenorByID(3, 1);
//     EXPECT_DOUBLE_EQ(-128, actual);
// }

// TEST_F(SensorManagerTest, test_initMeasurementsArray2D_WithSingleObjectIsNull_ReturnsFalse)
// {
//     MockSensor d1;
//     ISensor *array[1] = {nullptr};

//     EXPECT_FALSE(_mgr.init(array, 1));
//     uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
//     EXPECT_EQ(0, num);
// }

// TEST_F(SensorManagerTest, test_initMeasurementsArray2D_WithSensorIDAboveRealQuantity)
// {
//     MockSensor d1;
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
//     char sensorName[] = "s1";
//     EXPECT_CALL(d1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName, sensorName + 3), Return(2)));
//     ISensor *array[1] = {&d1};
//     _mgr.init(array, 1);

//     double actual = _mgr.getCurrentMeasurementOfOneSenorByID(0);
//     EXPECT_DOUBLE_EQ(-128, actual);
//     actual = _mgr.getCurrentMeasurementOfOneSenorByID(0, 3);
//     EXPECT_DOUBLE_EQ(-128, actual);
// }

// TEST_F(SensorManagerTest, test_initMeasurementsArray2D_WithNullObject_ReturnsFalse)
// {
//     EXPECT_FALSE(_mgr.init(nullptr, 1));
//     uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
//     EXPECT_EQ(0, num);
// }

// TEST_F(SensorManagerTest, test_initMeasurementsArray2D_WithZeroObjects_ReturnsFalse)
// {
//     MockSensor d1;
//     ISensor *array[1] = {&d1};

//     EXPECT_FALSE(_mgr.init(array, 0));
//     EXPECT_FALSE(_mgr.init(nullptr, 0));
//     EXPECT_FALSE(_mgr.init({nullptr}, 0));
// }

// TEST_F(SensorManagerTest, test_getNumberOfSensorsOnPINByID_ReturnsNumberOfConnectedSensorsOnPIN)
// {
//     MockSensor d1;
//     MockSensor d2;
//     MockSensor d3;
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
//     EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(3));
//     EXPECT_CALL(d3, getNumberOfConnectedSensors).Times(1).WillOnce(Return(7));
//     char sensorName1[] = "s1";
//     EXPECT_CALL(d1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName1, sensorName1 + 3), Return(2)));
//     char sensorName2[] = "s2";
//     EXPECT_CALL(d2, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName2, sensorName2 + 3), Return(2)));
//     char sensorName3[] = "s3";
//     EXPECT_CALL(d3, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName3, sensorName3 + 3), Return(2)));

//     ISensor *array[3] = {&d1, &d2, &d3};
//     _mgr.init(array, 3);

//     uint8_t num = _mgr.getNumberOfSensorsOnPINByID(0);
//     EXPECT_EQ(1, num);
//     num = _mgr.getNumberOfSensorsOnPINByID(1);
//     EXPECT_EQ(3, num);
//     num = _mgr.getNumberOfSensorsOnPINByID(2);
//     EXPECT_EQ(7, num);
// }

// TEST_F(SensorManagerTest, test_getNumberOfSensorsOnPINByID_WithoutInit_ReturnsZero)
// {
//     MockSensor d1;
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(0);

//     uint8_t num = _mgr.getNumberOfSensorsOnPINByID(0);
//     EXPECT_EQ(0, num);
// }

// TEST_F(SensorManagerTest, test_getNumberOfSensorsOnPINByID_WithOneNullObject_ReturnsZero)
// {
//     MockSensor d1;
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(0);
//     _mgr.init({nullptr}, 1);

//     uint8_t num = _mgr.getNumberOfSensorsOnPINByID(0);
//     EXPECT_EQ(0, num);
// }

// TEST_F(SensorManagerTest, test_getNumberOfSensorsOnPINByID_WithNonExistItem_ReturnsZero)
// {
//     MockSensor d1;
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(0);

//     uint8_t num = _mgr.getNumberOfSensorsOnPINByID(3);
//     EXPECT_EQ(0, num);
// }

// TEST_F(SensorManagerTest, test_refreshSensorsData2D_WithoutErrors_ReturnsTrue)
// {
//     MockSensor d1;
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
//     EXPECT_CALL(d1, requestCurrentMeasurement).Times(1);
//     EXPECT_CALL(d1, getCurrentMeasurementByID).Times(1).WillOnce(Return(11.11));
//     char sensorName1[] = "s1";
//     EXPECT_CALL(d1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName1, sensorName1 + 3), Return(2)));
//     MockSensor d2;
//     EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));
//     EXPECT_CALL(d2, requestCurrentMeasurement).Times(1);
//     EXPECT_CALL(d2, getCurrentMeasurementByID).Times(2).WillOnce(Return(-07.11)).WillOnce(Return(10.55));
//     char sensorName2[] = "s2";
//     EXPECT_CALL(d2, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName2, sensorName2 + 3), Return(2)));
//     MockSensor d3;
//     EXPECT_CALL(d3, getNumberOfConnectedSensors).Times(1).WillOnce(Return(3));
//     EXPECT_CALL(d3, requestCurrentMeasurement).Times(1);
//     EXPECT_CALL(d3, getCurrentMeasurementByID).Times(3).WillOnce(Return(0)).WillOnce(Return(-74.55)).WillOnce(Return(10));
//     char sensorName3[] = "s3";
//     EXPECT_CALL(d3, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName3, sensorName3 + 3), Return(2)));

//     ISensor *array[3] = {&d1, &d2, &d3};
//     _mgr.init(array, 3);

//     EXPECT_TRUE(_mgr.refreshSensorsData2D());
// }

// TEST_F(SensorManagerTest, test_refreshSensorsData2D_WithoutInit_ReturnsFalse)
// {
//     EXPECT_FALSE(_mgr.refreshSensorsData2D());
// }

// TEST_F(SensorManagerTest, test_refreshSensorsData2D_WithZeroQuantity_ReturnsFalse)
// {
//     MockSensor d1;
//     ISensor *array[1] = {&d1};
//     _mgr.init(array, 0);

//     EXPECT_FALSE(_mgr.refreshSensorsData2D());
// }

// TEST_F(SensorManagerTest, test_refreshSensorsData2D_WithArrayOfNulls_ReturnsFalse)
// {
//     ISensor *array[2] = {nullptr, nullptr};
//     _mgr.init(array, 2);

//     EXPECT_FALSE(_mgr.refreshSensorsData2D());
// }

// // TEST_F(SensorManagerTest, test_refreshSensorsData2D_WithSensorObjectWithZeroSensors_ReturnsFalse)
// // {
// //     MockSensor d1;
// //     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(0));
// //     EXPECT_CALL(d1, requestCurrentMeasurement).Times(1);
// //     EXPECT_CALL(d1, getCurrentMeasurementByID).Times(0);
// //     char sensorName1[] = "s1";
// //     EXPECT_CALL(d1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName1, sensorName1 + 3), Return(2)));
// //     // EXPECT_CALL(d1, getName(_)).WillOnce(DoAll(SetArgReferee<0>(sensorName1), Return(2)));
// //     ISensor *array[1] = {&d1};
// //     _mgr.initSenorsOnAllPINs(array, 1);

// //     EXPECT_TRUE(_mgr.refreshSensorsData2D());

// //     _mgr.getCurrentMeasurementOfOneSenorByID(0);
// // }

// TEST_F(SensorManagerTest, DISABLED_test_sendSensorsData2D_WithCorrectData_ReturnsTrue)
// {
//     EXPECT_CALL(_mqtt, send(_, _)).Times(6).WillRepeatedly(Return(true));

//     MockSensor d1;
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
//     EXPECT_CALL(d1, requestCurrentMeasurement).Times(1);
//     EXPECT_CALL(d1, getCurrentMeasurementByID).Times(1).WillOnce(Return(11.11));
//     MockSensor d2;
//     EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));
//     EXPECT_CALL(d2, requestCurrentMeasurement).Times(1);
//     EXPECT_CALL(d2, getCurrentMeasurementByID).Times(2).WillOnce(Return(-07.11)).WillOnce(Return(10.55));
//     MockSensor d3;
//     EXPECT_CALL(d3, getNumberOfConnectedSensors).Times(1).WillOnce(Return(3));
//     EXPECT_CALL(d3, requestCurrentMeasurement).Times(1);
//     EXPECT_CALL(d3, getCurrentMeasurementByID).Times(3).WillOnce(Return(0)).WillOnce(Return(-74.55)).WillOnce(Return(10));

//     ISensor *array[3] = {&d1, &d2, &d3};
//     _mgr.init(array, 3);

//     const char *addresses[] = {"/UZV1/temp1", "/UZV2/temp1", "Third Topic, the long one"};
//     bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 3);

//     _mgr.refreshSensorsData2D();

//     EXPECT_TRUE(_mgr.sendSensorsData2D());
// }

// TEST_F(SensorManagerTest, DISABLED_test_sendSensorsData2D_WithFailureUponSending_ReturnsFalse)
// {
//     EXPECT_CALL(_mqtt, send(_, _)).Times(1).WillRepeatedly(Return(false));

//     MockSensor d1;
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));

//     ISensor *array[1] = {&d1};
//     using ::testing::HasSubstr;
//     _mgr.init(array, 1);

//     const char *addresses[] = {"/UZV1/temp1"};
//     bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 1);

//     EXPECT_FALSE(_mgr.sendSensorsData2D());
// }

// TEST_F(SensorManagerTest, DISABLED_test_sendSensorsData2D_WithNullAddress_ReturnsFalse)
// {
//     EXPECT_CALL(_mqtt, send(_, _)).Times(3).WillOnce(Return(true)).WillOnce(Return(false)).WillOnce(Return(true));

//     MockSensor d1;
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
//     MockSensor d2;
//     EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
//     MockSensor d3;
//     EXPECT_CALL(d3, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));

//     ISensor *array[3] = {&d1, &d2, &d3};
//     _mgr.init(array, 3);

//     const char *addresses[] = {"/UZV1/temp1", nullptr, "3rd addr"};
//     bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 3);

//     EXPECT_FALSE(_mgr.sendSensorsData2D());
// }

// TEST_F(SensorManagerTest, test_sendingJSON_SingleSensor_ReturnsTrues)
// {
//     MockSensor d1;
//     ISensor *array[1] = {&d1};
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
//     char sensorName[] = "ID/1_t+ p";
//     EXPECT_CALL(d1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName, sensorName + 10), Return(9)));
//     _mgr.init(array, 1);
//     const char *addresses[] = {"/UZV1/temp1"};
//     bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 1);
//     EXPECT_CALL(d1, requestCurrentMeasurement).Times(1);
//     EXPECT_CALL(d1, getCurrentMeasurementByID).Times(1).WillOnce(Return(11.11));
//     EXPECT_TRUE(_mgr.refreshSensorsData2D());

//     char message[] = "{\"name\":\"ID/1_t+ p\",\"data\":[11.11]}";

//     EXPECT_CALL(_mqtt, send(HasSubstr(message), HasSubstr("/UZV1/temp1"))).Times(1).WillOnce(Return(true));
//     EXPECT_TRUE(_mgr.sendSensorsData2D());
// }

// TEST_F(SensorManagerTest, test_sendingJSON_MultipleSensors_ReturnsTrue)
// {
//     MockSensor d1;
//     EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
//     EXPECT_CALL(d1, requestCurrentMeasurement).Times(1);
//     EXPECT_CALL(d1, getCurrentMeasurementByID).Times(1).WillOnce(Return(11.11));
//     char sensorName1[] = "s1/temp";
//     EXPECT_CALL(d1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName1, sensorName1 + 8), Return(7)));
//     MockSensor d2;
//     EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));
//     EXPECT_CALL(d2, requestCurrentMeasurement).Times(1);
//     EXPECT_CALL(d2, getCurrentMeasurementByID).Times(2).WillOnce(Return(-07.18)).WillOnce(Return(10.55));
//     char sensorName2[] = "s2/Te p&m+is-re";
//     EXPECT_CALL(d2, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName2, sensorName2 + 16), Return(15)));
//     MockSensor d3;
//     EXPECT_CALL(d3, getNumberOfConnectedSensors).Times(1).WillOnce(Return(3));
//     EXPECT_CALL(d3, requestCurrentMeasurement).Times(1);
//     EXPECT_CALL(d3, getCurrentMeasurementByID).Times(3).WillOnce(Return(0)).WillOnce(Return(745)).WillOnce(Return(1028));
//     char sensorName3[] = "s3/tem#+pre's!r$+UV";
//     EXPECT_CALL(d3, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName3, sensorName3 + 20), Return(19)));

//     ISensor *array[3] = {&d1, &d2, &d3};
//     _mgr.init(array, 3);

//     const char *addresses[] = {"/UZV1/temp1", "/topic2/with2VALS", "Third Topic, the long one"};
//     bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 3);

//     _mgr.refreshSensorsData2D();

//     char message1[] = "{\"name\":\"s1/temp\",\"data\":[11.11]}";
//     EXPECT_CALL(_mqtt, send(HasSubstr(message1), HasSubstr("/UZV1/temp1"))).Times(1).WillOnce(Return(true));
//     char message2[] = "{\"name\":\"s2/Te p&m+is-re\",\"data\":[-7.18,10.55]}";
//     EXPECT_CALL(_mqtt, send(HasSubstr(message2), HasSubstr("/topic2/with2VALS"))).Times(1).WillOnce(Return(true));
//     char message3[] = "{\"name\":\"s3/tem#+pre's!r$+UV\",\"data\":[0,745,1028]}";
//     EXPECT_CALL(_mqtt, send(HasSubstr(message3), HasSubstr("Third Topic, the long one"))).Times(1).WillOnce(Return(true));
//     EXPECT_TRUE(_mgr.sendSensorsData2D());
// }

TEST_F(SensorManagerTest, test_sendingSensorsJSON_withoutInit_fails_returnFalse)
{
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());
}

TEST_F(SensorManagerTest, DISABLED_test_sendingJSON_whenSomeArraysAreLonger_fails_returnFalse)
{
    // Can't check for: Out Of Boundary!!
    // Need to refactor code of the function to type safe pointers like string, vector, etc.
    // But in this project adding STL library to Arduino yeilds to a conflict with MQTT library
    // and code doesn't compile.

    MockSensor s1, s2;
    ISensor *array2[2] = {&s1, &s2};
    const char *addresses[] = {"/UZV1/temp1"};

    EXPECT_FALSE(_mgr.init2(array2, addresses, 2));
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());

    ISensor *array[2] = {&s1};
    const char *addresses2[] = {"/UZV1/temp1", "addr"};

    EXPECT_FALSE(_mgr.init2(array, addresses2, 2));
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());
}

TEST_F(SensorManagerTest, test_sendingJSON_whenArraysHaveSomeNulls_fails_returnFalse)
{
    MockSensor s1, s2;
    ISensor *array[2] = {&s1, &s2};
    const char *addresses[] = {"/UZV1/temp1", nullptr};

    EXPECT_FALSE(_mgr.init2(array, addresses, 2));
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());

    ISensor *array2[2] = {&s1, nullptr};
    const char *addresses2[] = {"/UZV1/temp1", "addr"};

    EXPECT_FALSE(_mgr.init2(array2, addresses2, 2));
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());

    EXPECT_FALSE(_mgr.init2(array2, addresses, 2));
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());
}

TEST_F(SensorManagerTest, test_sendingSensorsJSON_withNullArrays_fails_returnFalse)
{
    MockSensor d1;
    ISensor *array[1] = {&d1};
    EXPECT_FALSE(_mgr.init2(array, nullptr, 1));
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());

    const char *addresses[] = {"/UZV1/temp1"};
    EXPECT_FALSE(_mgr.init2(nullptr, addresses, 1));
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());

    EXPECT_FALSE(_mgr.init2(nullptr, nullptr, 1));
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());
}

TEST_F(SensorManagerTest, test_sendingSensorsJSON_withEmptyArrays_fails_returnFalse)
{
    ISensor *array[1] = {nullptr};
    const char *addresses[] = {"/UZV1/temp1"};
    EXPECT_FALSE(_mgr.init2(array, addresses, 1));
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());

    MockSensor s1;
    ISensor *array2[1] = {&s1};
    const char *addresses2[] = {""};
    EXPECT_FALSE(_mgr.init2(array2, addresses2, 1));
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());

    EXPECT_FALSE(_mgr.init2(array, addresses2, 1));
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());

    EXPECT_FALSE(_mgr.init2(array2, addresses, 0));
    EXPECT_FALSE(_mgr.refreshSensors());
    EXPECT_FALSE(_mgr.sendSensorsData());
}

TEST_F(SensorManagerTest, test_sendingSensorsJSON_SingleSensor_ReturnsTrues)
{
    MockSensor d1;
    ISensor *array[1] = {&d1};
    const char *addresses[] = {"/UZV1/temp1"};
    EXPECT_TRUE(_mgr.init2(array, addresses, 1));

    EXPECT_CALL(d1, requestCurrentMeasurement).Times(1);
    EXPECT_TRUE(_mgr.refreshSensors());

    char message[] = "{\"name\":\"ID/1_t+ p\",\"data\":[11.11]}";
    EXPECT_CALL(_mqtt, send(HasSubstr(message), HasSubstr("/UZV1/temp1"))).Times(1).WillOnce(Return(true));
    EXPECT_CALL(d1, getCurrentMeasurementByID).Times(1).WillOnce(Return(11.11));
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillRepeatedly(Return(1));
    char sensorName[] = "ID/1_t+ p";
    EXPECT_CALL(d1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName, sensorName + 10), Return(9)));
    EXPECT_TRUE(_mgr.sendSensorsData());
}

TEST_F(SensorManagerTest, test_sendingSensorsJSON_MultipleSensors_ReturnsTrue)
{
    MockSensor s1;
    EXPECT_CALL(s1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
    EXPECT_CALL(s1, requestCurrentMeasurement).Times(1);
    EXPECT_CALL(s1, getCurrentMeasurementByID).Times(1).WillOnce(Return(11.11));
    char sensorName1[] = "s1/temp";
    EXPECT_CALL(s1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName1, sensorName1 + 8), Return(7)));
    MockSensor s2;
    EXPECT_CALL(s2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));
    EXPECT_CALL(s2, requestCurrentMeasurement).Times(1);
    EXPECT_CALL(s2, getCurrentMeasurementByID).Times(2).WillOnce(Return(-07.18)).WillOnce(Return(10.55));
    char sensorName2[] = "s2/Te p&m+is-re";
    EXPECT_CALL(s2, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName2, sensorName2 + 16), Return(15)));
    MockSensor s3;
    EXPECT_CALL(s3, getNumberOfConnectedSensors).Times(1).WillOnce(Return(3));
    EXPECT_CALL(s3, requestCurrentMeasurement).Times(1);
    EXPECT_CALL(s3, getCurrentMeasurementByID).Times(3).WillOnce(Return(0)).WillOnce(Return(745)).WillOnce(Return(1028));
    char sensorName3[] = "s3/tem#+pre's!r$+UV";
    EXPECT_CALL(s3, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(sensorName3, sensorName3 + 20), Return(19)));

    ISensor *array[3] = {&s1, &s2, &s3};
    const char *addresses[] = {"/UZV1/temp1", "/topic2/with2VALS", "Third Topic, the long one"};
    EXPECT_TRUE(_mgr.init2(array, addresses, 3));

    EXPECT_TRUE(_mgr.refreshSensors());

    char message1[] = "{\"name\":\"s1/temp\",\"data\":[11.11]}";
    EXPECT_CALL(_mqtt, send(HasSubstr(message1), HasSubstr("/UZV1/temp1"))).Times(1).WillOnce(Return(true));
    char message2[] = "{\"name\":\"s2/Te p&m+is-re\",\"data\":[-7.18,10.55]}";
    EXPECT_CALL(_mqtt, send(HasSubstr(message2), HasSubstr("/topic2/with2VALS"))).Times(1).WillOnce(Return(true));
    char message3[] = "{\"name\":\"s3/tem#+pre's!r$+UV\",\"data\":[0,745,1028]}";
    EXPECT_CALL(_mqtt, send(HasSubstr(message3), HasSubstr("Third Topic, the long one"))).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(_mgr.sendSensorsData());
}

TEST_F(SensorManagerTest, test_sendingSystemJSON_SingleSensor_ReturnsTrues)
{
    MockSensor s1;
    ISensor *array[1] = {&s1};
    const char *addresses[] = {"/addr"};
    EXPECT_TRUE(_mgr.init2(array, addresses, 1));
    char message[] = "{\"DeviceID\":\"VIJ2022\",\"uptime\":1144273222,\"sensors\":1}";

    EXPECT_CALL(_mqtt, send(HasSubstr(message), HasSubstr("/SystemInfo"))).Times(1).WillOnce(Return(true));

    EXPECT_TRUE(_mgr.sendSystemInfo((uint32_t)1144273222));
}

TEST_F(SensorManagerTest, test_executeActuator_withoutInit_fails_returnFalse)
{
    EXPECT_FALSE(_mgr.executeCommand("ON"));
}

TEST_F(SensorManagerTest, test_executeActuator_whenArraysHaveSomeNulls_fails_returnFalse)
{
    MockActuator a1;
    IActuator *array[2] = {&a1, nullptr};
    const char *addresses[] = {"/manage/actuator", "m/1"};
    EXPECT_CALL(_mqtt, subscribeToTopic(_)).Times(0);
    EXPECT_FALSE(_mgr.initActuators(array, addresses, 2));
    EXPECT_FALSE(_mgr.executeCommand("ON"));

    MockActuator a2;
    IActuator *array2[2] = {&a1, &a2};
    const char *addresses2[] = {nullptr, "m/1"};
    EXPECT_CALL(_mqtt, subscribeToTopic(_)).Times(0);
    EXPECT_FALSE(_mgr.initActuators(array2, addresses2, 2));
    EXPECT_FALSE(_mgr.executeCommand("ON"));

    EXPECT_CALL(_mqtt, subscribeToTopic(_)).Times(0);
    EXPECT_FALSE(_mgr.initActuators(array, addresses2, 2));
    EXPECT_FALSE(_mgr.executeCommand("ON"));
}

TEST_F(SensorManagerTest, test_executeActuator_withNullArrays_fails_returnFalse)
{
    MockActuator a1;
    IActuator *array[1] = {&a1};
    EXPECT_CALL(_mqtt, subscribeToTopic(_)).Times(0);
    EXPECT_FALSE(_mgr.initActuators(array, nullptr, 1));
    EXPECT_FALSE(_mgr.executeCommand("ON"));

    const char *addresses[] = {"m/1"};
    EXPECT_CALL(_mqtt, subscribeToTopic(_)).Times(0);
    EXPECT_FALSE(_mgr.initActuators(nullptr, addresses, 1));
    EXPECT_FALSE(_mgr.executeCommand("ON"));

    EXPECT_CALL(_mqtt, subscribeToTopic(_)).Times(0);
    EXPECT_FALSE(_mgr.initActuators(nullptr, nullptr, 1));
    EXPECT_FALSE(_mgr.executeCommand("ON"));
}

TEST_F(SensorManagerTest, test_executeActuator_withEmptyArrays_fails_returnFalse)
{
    IActuator *array[1] = {nullptr};
    const char *addresses[] = {"/topic"};
    EXPECT_FALSE(_mgr.initActuators(array, addresses, 1));
    EXPECT_FALSE(_mgr.executeCommand("ON"));

    MockActuator a1;
    IActuator *array2[1] = {&a1};
    const char *addresses2[] = {""};
    EXPECT_FALSE(_mgr.initActuators(array2, addresses2, 1));
    EXPECT_FALSE(_mgr.executeCommand("ON"));

    EXPECT_FALSE(_mgr.initActuators(array, addresses2, 1));
    EXPECT_FALSE(_mgr.executeCommand("ON"));

    EXPECT_FALSE(_mgr.initActuators(array2, addresses, 0));
    EXPECT_FALSE(_mgr.executeCommand("ON"));
}

TEST_F(SensorManagerTest, test_executeActuator_SingleActuator_ReturnsTrue)
{
    MockActuator a1;
    IActuator *array[1] = {&a1};
    const char *addresses[] = {"/manage/actuator"};
    EXPECT_CALL(_mqtt, subscribeToTopic(_)).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(_mgr.initActuators(array, addresses, 1));

    char payload[] = "{\"name\":\"pump\",\"action\":\"ON\"}";
    char actuatorName[] = "pump";
    EXPECT_CALL(a1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(actuatorName, actuatorName + 5), Return(5)));
    EXPECT_CALL(a1, execute(_)).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(_mgr.executeCommand(payload));

    char payload2[] = "{\"name\":\"pump\",\"action\":\"OFF\"}";
    EXPECT_CALL(a1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(actuatorName, actuatorName + 5), Return(5)));
    EXPECT_CALL(a1, execute(_)).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(_mgr.executeCommand(payload2));

    char payload3[] = "{\"name\":\"non exist pump\",\"action\":\"ON\"}";
    EXPECT_CALL(a1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(actuatorName, actuatorName + 5), Return(5)));
    EXPECT_CALL(a1, execute(_)).Times(0);
    EXPECT_FALSE(_mgr.executeCommand(payload3));
}

TEST_F(SensorManagerTest, test_executeActuator_WrongJSON_ReturnsFalse)
{
    MockActuator a1;
    IActuator *array[1] = {&a1};
    const char *addresses[] = {"/manage/actuator"};
    EXPECT_CALL(_mqtt, subscribeToTopic(_)).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(_mgr.initActuators(array, addresses, 1));

    char payload[] = "{\"name\":\"to big pump name here for buffer overrun\",\"action\":\"ON\"}";
    char actuatorName[] = "pump";
    EXPECT_CALL(a1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(actuatorName, actuatorName + 5), Return(5)));
    EXPECT_CALL(a1, execute(_)).Times(0);
    EXPECT_FALSE(_mgr.executeCommand(payload));

    char payload2[] = "{\"name\":\"pump\",\"action\":\"wrong action\"}";
    EXPECT_CALL(a1, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(actuatorName, actuatorName + 5), Return(5)));
    EXPECT_CALL(a1, execute(_)).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(_mgr.executeCommand(payload2));
}

TEST_F(SensorManagerTest, test_executeActuator_MultipleActuators_ReturnsTrue)
{
    MockActuator a1;
    MockActuator a2;
    IActuator *array[2] = {&a1, &a2};
    const char *addresses[] = {"/manage/actuator", "/manage/2"};
    EXPECT_CALL(_mqtt, subscribeToTopic(_)).Times(2).WillRepeatedly(Return(true));
    EXPECT_TRUE(_mgr.initActuators(array, addresses, 2));

    char payloadA1[] = "{\"name\":\"pump\",\"action\":\"ON\"}";
    char payloadA2[] = "{\"name\":\"p2\",\"action\":\"OFF\"}";
    char actuator1[] = "pump";
    char actuator2[] = "p2";
    EXPECT_CALL(a1, getName(_)).Times(AtLeast(1)).WillRepeatedly(DoAll(SetArrayArgument<0>(actuator1, actuator1 + 5), Return(5)));
    EXPECT_CALL(a2, getName(_)).Times(1).WillOnce(DoAll(SetArrayArgument<0>(actuator2, actuator2 + 3), Return(3)));
    EXPECT_CALL(a1, execute(_)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(a2, execute(_)).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(_mgr.executeCommand(payloadA1));
    EXPECT_TRUE(_mgr.executeCommand(payloadA2));

    char payload3[] = "{\"name\":\"non exist pump\",\"action\":\"ON\"}";
    EXPECT_CALL(a1, getName(_)).Times(AtLeast(1)).WillRepeatedly(DoAll(SetArrayArgument<0>(actuator1, actuator1 + 5), Return(5)));
    EXPECT_CALL(a2, getName(_)).Times(AtLeast(1)).WillRepeatedly(DoAll(SetArrayArgument<0>(actuator2, actuator2 + 3), Return(3)));
    EXPECT_FALSE(_mgr.executeCommand(payload3));
}
