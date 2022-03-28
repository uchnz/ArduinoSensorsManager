#include <gtest/gtest.h>
#include <SensorManager/SensorManager_test.h>

using ::testing::Return;

TEST_F(SensorManagerTest, test_FillTopicsStringsWithNull_ReturnsFalse)
{
    const char **topics = nullptr;
    bool result = _mgr.fillTopicsStrings(topics, 5);

    EXPECT_FALSE(result);
}

TEST_F(SensorManagerTest, test_FillTopicsStringsNumberOfTopicsGreaterThenStrings_ReturnsFalse)
{
    // Can't check for: Out Of Boundary!!
    // Need to refactor code of the function to type safe pointers like string, vector, etc.
    // But in this project adding STL library to Arduino yeilds to a conflict with MQTT librarary
    // and code doesn't compile.
}

TEST_F(SensorManagerTest, test_fillTopicsStringsWithZeroQuantity_ReturnsFalse)
{
    const char *topics[] = {"/UZV1/temp1\n", "/UZV2/temp1\n"};
    bool result = _mgr.fillTopicsStrings(topics, 0);

    EXPECT_FALSE(result);
}

TEST_F(SensorManagerTest, test_GetTopicOnID_returnsTopic)
{
    const char *topics[] = {"/UZV1/temp1", "/UZV2/temp1", "Third Topic, the long one"};
    bool result = _mgr.fillTopicsStrings(topics, 3);

    EXPECT_TRUE(result);

    char actual[100];
    _mgr.GetTopicByID(0, actual);
    EXPECT_STREQ(actual, topics[0]);

    _mgr.GetTopicByID(1, actual);
    EXPECT_STREQ(actual, topics[1]);

    _mgr.GetTopicByID(2, actual);
    EXPECT_STREQ(actual, topics[2]);
}

TEST_F(SensorManagerTest, test_GetTopicOnUpperAndLowerlimits_ReturnsFirstAndLastItems)
{
    const char *topics[] = {"/UZV1/someaddr", "/Some topic/ in /the middle///", "more in /the middle", "/UZV2/temp1/topichere"};
    bool result = _mgr.fillTopicsStrings(topics, 4);

    EXPECT_TRUE(result);

    char actual[100];
    _mgr.GetTopicByID(0, actual);
    EXPECT_STREQ(actual, topics[0]);
    _mgr.GetTopicByID(3, actual);
    EXPECT_STREQ(actual, topics[3]);
}

TEST_F(SensorManagerTest, test_GetTopicAboveUpperlimit_ReturnsNull)
{
    const char *topics[] = {"/UZV1/someaddr", "/UZV2/temp1/topichere"};
    bool result = _mgr.fillTopicsStrings(topics, 2);

    EXPECT_TRUE(result);

    char actual[100] = "";
    _mgr.GetTopicByID(5, actual);
    EXPECT_STREQ(actual, "");
}

TEST_F(SensorManagerTest, test_GetTopic_WhenNotFilled_ReturnsNull)
{
    char actual[100] = "";
    _mgr.GetTopicByID(0, actual);
    EXPECT_STREQ(actual, "");

    _mgr.GetTopicByID(3, actual);
    EXPECT_STREQ(actual, "");
}

TEST_F(SensorManagerTest, test_processDataWithInterval_WithErrors_ReturnsFalse)
{
    EXPECT_CALL(_dallas, getNumberOfConnectedSensors).Times(1).WillOnce(Return(0));

    _mgr.scanConnectedTemperatureSensors();

    EXPECT_FALSE(_mgr.processDataWithInterval());
}

TEST_F(SensorManagerTest, test_processDataWithInterval_WithoutErrors_ReturnsTrue)
{
    EXPECT_CALL(_dallas, getNumberOfConnectedSensors).Times(1).WillOnce(Return(3));
    EXPECT_CALL(_dallas, getTemperatureByID).Times(3).WillOnce(Return(111.11)).WillOnce(Return(-10.55)).WillOnce(Return(0));
    EXPECT_CALL(_dallas, requestCurrentTemperatures).Times(1);
    EXPECT_CALL(_mqtt, send(_, _)).Times(3).WillRepeatedly(Return(true));
    const char *topics[] = {"/UZV1/temp1", "/UZV2/temp1", "Third Topic, the long one"};
    bool result = _mgr.fillTopicsStrings(topics, 3);
    _mgr.scanConnectedTemperatureSensors();

    EXPECT_TRUE(_mgr.processDataWithInterval());
}

TEST_F(SMTest2, test_getNumberOfSensorTypesInArray_BeforeInit_ReturnsZero)
{
    uint8_t num = _mgr.getTotalNumberOfSensorTypesInArray();
    EXPECT_EQ(0, num);
}

TEST_F(SMTest2, test_initSenorsInArray)
{
    MockIDallas d1;
    MockIDallas d2;
    IDallas *array[2] = {&d1, &d2};
    _mgr.initSenorsInArray(array, 2);

    uint8_t num = _mgr.getTotalNumberOfSensorTypesInArray();
    EXPECT_EQ(2, num);
}

TEST_F(SMTest2, test_getTotalNumberOfSensorTypesInArray_WithoutInit_ReturnsZero)
{
    MockIDallas d1;
    MockIDallas d2;
    IDallas *array[2] = {&d1, &d2};

    uint8_t num = _mgr.getTotalNumberOfSensorTypesInArray();
    EXPECT_EQ(0, num);
}

TEST_F(SMTest2, test_getNumberOfSensorsInArrayCellByID_ReturnsNumberOfConnectedSensorsOnConnectedPort)
{
    MockIDallas d1;
    MockIDallas d2;
    MockIDallas d3;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
    EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(3));
    EXPECT_CALL(d3, getNumberOfConnectedSensors).Times(1).WillOnce(Return(7));

    IDallas *array[3] = {&d1, &d2, &d3};
    EXPECT_TRUE(_mgr.initSenorsInArray(array, 3));

    uint8_t num = _mgr.getNumberOfSensorsInArrayCellByID(0);
    EXPECT_EQ(1, num);

    num = _mgr.getNumberOfSensorsInArrayCellByID(1);
    EXPECT_EQ(3, num);

    num = _mgr.getNumberOfSensorsInArrayCellByID(2);
    EXPECT_EQ(7, num);
}

TEST_F(SMTest2, test_getNumberOfSensorsInArrayCellByID_WithoutInit_ReturnsZero)
{
    MockIDallas d1;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(0);

    uint8_t num = _mgr.getNumberOfSensorsInArrayCellByID(0);
    EXPECT_EQ(0, num);

    num = _mgr.getNumberOfSensorsInArrayCellByID(1);
    EXPECT_EQ(0, num);
}

TEST_F(SMTest2, test_init2DTemperatureArray_WithNullPointer_ReturnFalse)
{
    MockIDallas d1;
    IDallas *array[1] = {&d1};

    EXPECT_FALSE(_mgr.initSenorsInArray(array, 0));

    EXPECT_FALSE(_mgr.initSenorsInArray(nullptr, 1));
}

TEST_F(SMTest2, test_init2DTemperatureArray_WithErrorValue_Success)
{
    MockIDallas d1;
    MockIDallas d2;
    EXPECT_CALL(d1, getNumberOfConnectedSensors).Times(1).WillOnce(Return(1));
    EXPECT_CALL(d2, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));

    IDallas *array[2] = {&d1, &d2};
    EXPECT_TRUE(_mgr.initSenorsInArray(array, 2));

    float actual = _mgr.getCurrentTemperatureOfSingleSenorByID(0, 0);
    EXPECT_FLOAT_EQ(-128, actual);

    actual = _mgr.getCurrentTemperatureOfSingleSenorByID(1, 0);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.getCurrentTemperatureOfSingleSenorByID(1, 1);
    EXPECT_FLOAT_EQ(-128, actual);
}

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
    _mgr.initSenorsInArray(array, 3);

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
    _mgr.initSenorsInArray(array, 0);

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
    _mgr.initSenorsInArray(array, 3);

    const char *topics[] = {"/UZV1/temp1", "/UZV2/temp1", "Third Topic, the long one"};
    bool result = _mgr.fillTopicsStrings(topics, 3);

    _mgr.refreshSensorsData2D();

    EXPECT_TRUE(_mgr.sendSensorsData2D());
}