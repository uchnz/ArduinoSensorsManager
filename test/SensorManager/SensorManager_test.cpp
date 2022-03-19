#include <gtest/gtest.h>
//#include <stdint.h>
#include <../test/SensorManager/SensorManager_test.h>

TEST_F(SensorManagerTest, test_printTemperatureDebugInfo_ReturnsDebugStringData)
{
    // char expected[] = "Sensor[1] = 21.50, Address: 28e4ac0400fe";
    // std::string actual = _mgr.printTemperatureDebugInfo(1, 21.50);

    // EXPECT_STREQ(actual.c_str(), expected);
}

TEST_F(SensorManagerTest, test_FillTopicsStringsWithNull_ReturnsFalse)
{
    char **topics = nullptr;
    bool expected = _mgr.fillTopicsStrings(topics, 5);

    EXPECT_FALSE(expected);
}

TEST_F(SensorManagerTest, test_fillTopicsStringsWithZeroQuantity_ReturnsFalse)
{
    char *topics[] = {"/UZV1/temp1\n", "/UZV2/temp1\n"};
    bool expected = _mgr.fillTopicsStrings(topics, 0);

    EXPECT_FALSE(expected);
}

TEST_F(SensorManagerTest, test_fillTopicsStrings_returnsTrue)
{
    char *topics[] = {"/UZV1/temp1", "/UZV2/temp1"};
    bool expected = _mgr.fillTopicsStrings(topics, 2);

    EXPECT_TRUE(expected);
}

TEST_F(SensorManagerTest, test_GetTopicOnID_returnsTopic)
{
    char *topics[] = {"/UZV1/temp1", "/UZV2/temp1", "Third Topic, the long one"};
    _mgr.fillTopicsStrings(topics, 3);

    char *actual = _mgr.GetTopicByID(0);
    EXPECT_STREQ(actual, topics[0]);

    actual = _mgr.GetTopicByID(1);
    EXPECT_STREQ(actual, topics[1]);

    actual = _mgr.GetTopicByID(2);
    EXPECT_STREQ(actual, topics[2]);
}

TEST_F(SensorManagerTest, test_GetTopicOnUpperlimit_ReturnsLastItem)
{
    char *topics[] = {"/UZV1/someaddr", "/UZV2/temp1/topichere"};
    _mgr.fillTopicsStrings(topics, 2);

    char *actual = _mgr.GetTopicByID(1);
    EXPECT_STREQ(actual, topics[1]);
}

TEST_F(SensorManagerTest, test_GetTopicAboveUpperlimit_ReturnsNull)
{
    char *topics[] = {"/UZV1/someaddr", "/UZV2/temp1/topichere"};
    _mgr.fillTopicsStrings(topics, 2);

    char *actual = _mgr.GetTopicByID(3);
    EXPECT_STREQ(actual, nullptr);
}

TEST_F(SensorManagerTest, test_GetTopicWhenNotFilled_ReturnsNull)
{
    char *actual = _mgr.GetTopicByID(0);
    EXPECT_STREQ(actual, nullptr);

    actual = _mgr.GetTopicByID(3);
    EXPECT_STREQ(actual, nullptr);
}