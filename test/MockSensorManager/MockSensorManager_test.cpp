#include <gtest/gtest.h>

#include "MockSensorManager_test.h"


TEST_F(SensorManagerTest, test_one)
{
    EXPECT_FALSE(false);
    
}


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

TEST_F(SensorManagerTest, test_GetTopicWhenNotFilled_ReturnsNull)
{
    char actual[100] = "";
    _mgr.GetTopicByID(0, actual);
    EXPECT_STREQ(actual, "");

    _mgr.GetTopicByID(3, actual);
    EXPECT_STREQ(actual, "");
}
