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
    EXPECT_CALL(_mqtt, send(_, _)).Times(3).WillOnce(Return(true));
    const char *topics[] = {"/UZV1/temp1", "/UZV2/temp1", "Third Topic, the long one"};
    bool result = _mgr.fillTopicsStrings(topics, 3);
    _mgr.scanConnectedTemperatureSensors();

    EXPECT_TRUE(_mgr.processDataWithInterval());
}
