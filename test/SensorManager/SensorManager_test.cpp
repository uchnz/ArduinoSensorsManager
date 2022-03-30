#include <gtest/gtest.h>
#include <SensorManager/SensorManager_test.h>

using ::testing::Return;

TEST_F(SensorManagerTest, test_fillAddressArray_WithNull_ReturnsFalse)
{
    const char **addresses = nullptr;
    bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 5);

    EXPECT_FALSE(result);
}

TEST_F(SensorManagerTest, test_fillAddressArray_WithZeroQuantity_ReturnsFalse)
{
    const char *addresses[] = {"/UZV1/temp1", "/UZV2/temp1\n"};
    bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 0);

    EXPECT_FALSE(result);
}

TEST_F(SensorManagerTest, test_fillAddressArray_WithNumberOfAddressesGreaterTotalAddresses_ReturnsFalse)
{
    // Can't check for: Out Of Boundary!!
    // Need to refactor code of the function to type safe pointers like string, vector, etc.
    // But in this project adding STL library to Arduino yeilds to a conflict with MQTT librarary
    // and code doesn't compile.
}

TEST_F(SensorManagerTest, test_AddressArray_IsFilledCorrectly)
{
    const char *addresses[] = {"/UZV1/temp1", "/UZV2/temp1", "Third Topic, the long one"};
    bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 3);

    EXPECT_TRUE(result);

    char actual[100];
    _mgr.getAddressToSendToByID(0, actual);
    EXPECT_STREQ(actual, addresses[0]);

    _mgr.getAddressToSendToByID(1, actual);
    EXPECT_STREQ(actual, addresses[1]);

    _mgr.getAddressToSendToByID(2, actual);
    EXPECT_STREQ(actual, addresses[2]);
}

TEST_F(SensorManagerTest, test_GetAddress_ByFirstAndLastID_ReturnsFirstAndLastAddresses)
{
    const char *addresses[] = {"/UZV1/someaddr", "/Some topic/ in /the middle///", "more in /the middle", "/UZV2/temp1/topichere"};
    bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 4);

    EXPECT_TRUE(result);

    char actual[100];
    _mgr.getAddressToSendToByID(0, actual);
    EXPECT_STREQ(actual, addresses[0]);
    _mgr.getAddressToSendToByID(3, actual);
    EXPECT_STREQ(actual, addresses[3]);
}

TEST_F(SensorManagerTest, test_GetAddressAboveUpperlimit_ReturnsEmptyString)
{
    const char *addresses[] = {"/UZV1/someaddr", "/UZV2/temp1/topichere"};
    bool result = _mgr.setAddressesToSendMeasurementsTo(addresses, 2);

    EXPECT_TRUE(result);

    char actual[100] = "";
    _mgr.getAddressToSendToByID(5, actual);
    EXPECT_STREQ(actual, "");
}

TEST_F(SensorManagerTest, test_GetAddress_WhenArrayIsNotFilled_ReturnsNull)
{
    char actual[100] = "";
    _mgr.getAddressToSendToByID(0, actual);
    EXPECT_STREQ(actual, "");

    _mgr.getAddressToSendToByID(3, actual);
    EXPECT_STREQ(actual, "");
}

TEST_F(SMTest2, test_getTotalNumberOfOccupiedPINs_BeforeInit_ReturnsZero)
{
    uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
    EXPECT_EQ(0, num);
}

TEST_F(SMTest2, test_initMeasurementsArray2D_WithOneSensorOnEveryPIN)
{
    MockIDallas d1;
    MockIDallas d2;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
    EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));

    IDallas *array[2] = {&d1, &d2};
    EXPECT_TRUE(_mgr.initSenorsOnAllPINs(array, 2));

    uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
    EXPECT_EQ(2, num);
    float actual = _mgr.getCurrentMeasurementOfOneSenorByID(0);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.getCurrentMeasurementOfOneSenorByID(1, 0);
    EXPECT_FLOAT_EQ(-128, actual);
}

TEST_F(SMTest2, test_initMeasurementsArray2D_WithManySensorsOnSomePINs)
{
    MockIDallas d1;
    MockIDallas d2;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
    EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));

    IDallas *array[2] = {&d1, &d2};
    EXPECT_TRUE(_mgr.initSenorsOnAllPINs(array, 2));

    uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
    EXPECT_EQ(2, num);
    float actual = _mgr.getCurrentMeasurementOfOneSenorByID(0);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.getCurrentMeasurementOfOneSenorByID(1);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.getCurrentMeasurementOfOneSenorByID(1, 1);
    EXPECT_FLOAT_EQ(-128, actual);
}

TEST_F(SMTest2, test_initMeasurementsArray2D_WithOneOfISensorObjectsIsNull)
{
    MockIDallas d1;
    MockIDallas d2;
    MockIDallas d3;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
    EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(0);
    EXPECT_CALL(d3, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));

    IDallas *array[3] = {&d1, nullptr, &d3};
    EXPECT_TRUE(_mgr.initSenorsOnAllPINs(array, 3));

    uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
    EXPECT_EQ(3, num);
    float actual = _mgr.getCurrentMeasurementOfOneSenorByID(0);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.getCurrentMeasurementOfOneSenorByID(1, 0);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.getCurrentMeasurementOfOneSenorByID(2, 1);
    EXPECT_FLOAT_EQ(-128, actual);
}

TEST_F(SMTest2, test_initMeasurementsArray2D_WithOneOfObjectsIsNotNull_ReturnsTrue)
{
    MockIDallas d1;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));
    IDallas *array[4] = {nullptr, nullptr, nullptr, &d1};
    EXPECT_TRUE(_mgr.initSenorsOnAllPINs(array, 4));

    uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
    EXPECT_EQ(4, num);
    float actual = _mgr.getCurrentMeasurementOfOneSenorByID(0);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.getCurrentMeasurementOfOneSenorByID(1);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.getCurrentMeasurementOfOneSenorByID(3);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.getCurrentMeasurementOfOneSenorByID(3, 1);
    EXPECT_FLOAT_EQ(-128, actual);
}

TEST_F(SMTest2, test_initMeasurementsArray2D_WithSingleObjectIsNull_ReturnsFalse)
{
    MockIDallas d1;
    IDallas *array[1] = {nullptr};

    EXPECT_FALSE(_mgr.initSenorsOnAllPINs(array, 1));
    uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
    EXPECT_EQ(0, num);
}

TEST_F(SMTest2, test_initMeasurementsArray2D_WithSensorIDAboveRealQuantity)
{
    MockIDallas d1;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
    IDallas *array[1] = {&d1};
    _mgr.initSenorsOnAllPINs(array, 1);

    float actual = _mgr.getCurrentMeasurementOfOneSenorByID(0);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.getCurrentMeasurementOfOneSenorByID(0, 3);
    EXPECT_FLOAT_EQ(-128, actual);
}

TEST_F(SMTest2, test_initMeasurementsArray2D_WithNullObject_ReturnsFalse)
{
    EXPECT_FALSE(_mgr.initSenorsOnAllPINs(nullptr, 1));
    uint8_t num = _mgr.getTotalNumberOfOccupiedPINs();
    EXPECT_EQ(0, num);
}

TEST_F(SMTest2, test_initMeasurementsArray2D_WithZeroObjects_ReturnsFalse)
{
    MockIDallas d1;
    IDallas *array[1] = {&d1};

    EXPECT_FALSE(_mgr.initSenorsOnAllPINs(array, 0));
    EXPECT_FALSE(_mgr.initSenorsOnAllPINs(nullptr, 0));
    EXPECT_FALSE(_mgr.initSenorsOnAllPINs({nullptr}, 0));
}

TEST_F(SMTest2, test_getNumberOfSensorsOnPINByID_ReturnsNumberOfConnectedSensorsOnPIN)
{
    MockIDallas d1;
    MockIDallas d2;
    MockIDallas d3;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
    EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(3));
    EXPECT_CALL(d3, getNumberOfConnectedSensors).Times(1).WillOnce(Return(7));

    IDallas *array[3] = {&d1, &d2, &d3};
    _mgr.initSenorsOnAllPINs(array, 3);

    uint8_t num = _mgr.getNumberOfSensorsOnPINByID(0);
    EXPECT_EQ(1, num);
    num = _mgr.getNumberOfSensorsOnPINByID(1);
    EXPECT_EQ(3, num);
    num = _mgr.getNumberOfSensorsOnPINByID(2);
    EXPECT_EQ(7, num);
}

TEST_F(SMTest2, test_getNumberOfSensorsOnPINByID_WithoutInit_ReturnsZero)
{
    MockIDallas d1;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(0);

    uint8_t num = _mgr.getNumberOfSensorsOnPINByID(0);
    EXPECT_EQ(0, num);
}

TEST_F(SMTest2, test_getNumberOfSensorsOnPINByID_WithOneNullObject_ReturnsZero)
{
    MockIDallas d1;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(0);
    _mgr.initSenorsOnAllPINs({nullptr}, 1);

    uint8_t num = _mgr.getNumberOfSensorsOnPINByID(0);
    EXPECT_EQ(0, num);
}

TEST_F(SMTest2, test_getNumberOfSensorsOnPINByID_WithNonExistItem_ReturnsZero)
{
    MockIDallas d1;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(0);

    uint8_t num = _mgr.getNumberOfSensorsOnPINByID(3);
    EXPECT_EQ(0, num);
}

//--------------------------
//--------------------------
// MORE TESTS TO CHECK BELOW
//--------------------------
//--------------------------

TEST_F(SMTest2, test_refreshSensorsData2D_WithoutErrors_ReturnsTrue)
{
    MockIDallas d1;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
    EXPECT_CALL(d1, requestCurrentTemperatures).Times(1);
    EXPECT_CALL(d1, getTemperatureByID).Times(1).WillOnce(Return(11.11));
    MockIDallas d2;
    EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));
    EXPECT_CALL(d2, requestCurrentTemperatures).Times(1);
    EXPECT_CALL(d2, getTemperatureByID).Times(2).WillOnce(Return(-07.11)).WillOnce(Return(10.55));
    MockIDallas d3;
    EXPECT_CALL(d3, getNumberOfConnectedSensors).Times(1).WillOnce(Return(3));
    EXPECT_CALL(d3, requestCurrentTemperatures).Times(1);
    EXPECT_CALL(d3, getTemperatureByID).Times(3).WillOnce(Return(0)).WillOnce(Return(-74.55)).WillOnce(Return(10));

    IDallas *array[3] = {&d1, &d2, &d3};
    _mgr.initSenorsOnAllPINs(array, 3);

    EXPECT_TRUE(_mgr.refreshSensorsData2D());
}

TEST_F(SMTest2, test_refreshSensorsData2D_WithoutInit_ReturnsFalse)
{
    EXPECT_FALSE(_mgr.refreshSensorsData2D());
}

TEST_F(SMTest2, test_refreshSensorsData2D_WithZeroArray_ReturnsFalse)
{
    MockIDallas d1;
    IDallas *array[1] = {&d1};
    _mgr.initSenorsOnAllPINs(array, 0);

    EXPECT_FALSE(_mgr.refreshSensorsData2D());
}

TEST_F(SMTest2, test_sendSensorsData2D_WithCorrectData_ReturnsTrue)
{
    EXPECT_CALL(_mqtt, send(_, _)).Times(6).WillRepeatedly(Return(true));

    MockIDallas d1;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
    EXPECT_CALL(d1, requestCurrentTemperatures).Times(1);
    EXPECT_CALL(d1, getTemperatureByID).Times(1).WillOnce(Return(11.11));
    MockIDallas d2;
    EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));
    EXPECT_CALL(d2, requestCurrentTemperatures).Times(1);
    EXPECT_CALL(d2, getTemperatureByID).Times(2).WillOnce(Return(-07.11)).WillOnce(Return(10.55));
    MockIDallas d3;
    EXPECT_CALL(d3, getNumberOfConnectedSensors).Times(1).WillOnce(Return(3));
    EXPECT_CALL(d3, requestCurrentTemperatures).Times(1);
    EXPECT_CALL(d3, getTemperatureByID).Times(3).WillOnce(Return(0)).WillOnce(Return(-74.55)).WillOnce(Return(10));

    IDallas *array[3] = {&d1, &d2, &d3};
    _mgr.initSenorsOnAllPINs(array, 3);

    const char *topics[] = {"/UZV1/temp1", "/UZV2/temp1", "Third Topic, the long one"};
    bool result = _mgr.setAddressesToSendMeasurementsTo(topics, 3);

    _mgr.refreshSensorsData2D();

    EXPECT_TRUE(_mgr.sendSensorsData2D());
}

// TEST_F(SensorManagerTest, test_processDataWithInterval_WithErrors_ReturnsFalse)
// {
//     EXPECT_CALL(_dallas, getNumberOfConnectedSensors).Times(1).WillOnce(Return(0));

//     _mgr.scanConnectedTemperatureSensors();

//     EXPECT_FALSE(_mgr.processDataWithInterval());
// }

// TEST_F(SensorManagerTest, test_processDataWithInterval_WithoutErrors_ReturnsTrue)
// {
//     EXPECT_CALL(_dallas, getNumberOfConnectedSensors).Times(1).WillOnce(Return(3));
//     EXPECT_CALL(_dallas, getTemperatureByID).Times(3).WillOnce(Return(111.11)).WillOnce(Return(-10.55)).WillOnce(Return(0));
//     EXPECT_CALL(_dallas, requestCurrentTemperatures).Times(1);
//     EXPECT_CALL(_mqtt, send(_, _)).Times(3).WillRepeatedly(Return(true));
//     const char *topics[] = {"/UZV1/temp1", "/UZV2/temp1", "Third Topic, the long one"};
//     bool result = _mgr.setAddressesToSendMeasurementsTo(topics, 3);
//     _mgr.scanConnectedTemperatureSensors();

//     EXPECT_TRUE(_mgr.processDataWithInterval());
// }