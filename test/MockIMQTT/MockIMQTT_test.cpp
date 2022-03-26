#include <gtest/gtest.h>

#include <SensorManager/SensorManager_test.h>

using ::testing::_;
using ::testing::Invoke;
using ::testing::MockFunction;
using ::testing::Return;

class IMQTTTest : public SensorManagerTest
{
};

TEST_F(IMQTTTest, test_sendSensorsData_WithNoData_ReturnsFalse)
{
    EXPECT_FALSE(_mgr.sendSensorsData());
}

TEST_F(IMQTTTest, test_sendSensorDataByID_WithNoData_ReturnsFalse)
{
    EXPECT_FALSE(_mgr.sendSensorDataByID(0));
}

// TEST_F(IMQTTTest, test_sendSensorData_CallsPublishWithTopicAndMessage_ReturnsTrue)
// {
//     char message[] = "11.3";
//     char topic[] = "/topic1/channel1";
//     EXPECT_CALL(_mqtt, send(message, topic)).Times(1).WillOnce(Return(true));

//     ASSERT_TRUE(_mgr.sendSensorsData(message, topic));
// }

// TEST_F(IMQTTTest, test_sendSensorData_WithNullPointersDoestCallSend_ReturnsFalse)
// {
//     EXPECT_CALL(_mqtt, send(_, _)).Times(0);

//     ASSERT_FALSE(_mgr.sendSensorsData(nullptr, nullptr));
//     ASSERT_FALSE(_mgr.sendSensorsData("some data", nullptr));
//     ASSERT_FALSE(_mgr.sendSensorsData(nullptr, "some address"));
// }

// TEST_F(IMQTTTest, test_sendSensorData_WithVadidDataOnFailure_ReturnsFalse)
// {
//     char message[] = "11.3";
//     char topic[] = "/topic1/channel1";
//     EXPECT_CALL(_mqtt, send(message, topic)).Times(1).WillOnce(Return(false));

//     ASSERT_FALSE(_mgr.sendSensorsData(message, topic));
// }

TEST_F(IMQTTTest, test_receiveManagingData_CallsCallbackWithValidData_BADTEST)
{
    // char message[] = "11.3";
    // char topic[] = "/topic1/channel1";
    // MockFunction<void(char *, char *)> mock_function;
    // EXPECT_CALL(mock_function, Call(message, topic)).Times(1);
    // ON_CALL(*this, receive).WillByDefault([this]() {
    //     real_.sensor_manager::SensorManager::callbackIncommingMessages(message, topic);
    // sensor_manager::SensorManager::callbackIncommingMessages(char *topic, char *payload)

    // WARNING: Currently this test does nothing.
    // I need to find a way to make receive() call my callback function.
    // Don't know how to do that yet.
    EXPECT_CALL(_mqtt, receive()).Times(1).WillOnce(Return(true));

    ASSERT_TRUE(_mgr.receiveManagingData());
}

// TEST_F(IMQTTTest, test_InitMQTT_IsCalledOnce)
// {
//     EXPECT_CALL(_mqtt, begin(_)).Times(1);
//     char MQTTserverIP[] = "10.62.202.14";

//     _mgr.initMQTT(MQTTserverIP);
// }

// TEST_F(IMQTTTest, test_InitMQTT_CalledWithNull_ReturnsFalse)
// {
//     char *MQTTserverIP = nullptr;

//     ASSERT_FALSE(_mgr.initMQTT(MQTTserverIP));
// }

// TEST_F(IMQTTTest, test_InitMQTT_CalledWithCorrectIP_ReturnsTrue)
// {
//     EXPECT_CALL(_mqtt, begin(_)).WillOnce(Return(true));
//     char MQTTserverIP[] = "192.168.1.1";

//     ASSERT_TRUE(_mgr.initMQTT(MQTTserverIP));
// }

// TEST_F(IMQTTTest, test_InitMQTT_CalledWithIncorrectIP_ReturnsFalse)
// {
//     EXPECT_CALL(_mqtt, begin(_)).WillOnce(Return(false));
//     char MQTTserverIP[] = "not ip";

//     ASSERT_FALSE(_mgr.initMQTT(MQTTserverIP));
// }

// TEST_F(IMQTTTest, test_installCallback_IsCalledWithNull_ReturnsFalse)
// {
//     EXPECT_CALL(_mqtt, onMessage(_)).Times(0);
//     sensor_manager::MQTTClientCallback cb = nullptr;

//     _mgr.installCallback(cb);
// }

// TEST_F(IMQTTTest, test_installCallback_IsCalledCorrectly_ReturnsTrue)
// {
//     EXPECT_CALL(_mqtt, onMessage(_)).Times(1);
//     sensor_manager::MQTTClientCallback cb = _mgr.callbackIncommingMessages;
//     _mgr.installCallback(cb);
// }

// TEST_F(IMQTTTest, test_setKeepAlive_IsSet)
// {
//     uint16_t keepAlive = 653;
//     EXPECT_CALL(_mqtt, setKeepAlive(::testing::Eq(keepAlive)));

//     _mgr.setKeepAliveClient(keepAlive);
// }

// TEST_F(IMQTTTest, test_setKeepAlive_IsCalledWithNegativeValue)
// {
//     uint16_t keepAlive = -50;
//     EXPECT_CALL(_mqtt, setKeepAlive(::testing::Eq(keepAlive)));

//     _mgr.setKeepAliveClient(keepAlive);
// }

// TEST_F(IMQTTTest, test_setKeepAlive_IsCalledWithZeroValue)
// {
//     uint16_t keepAlive = 0;
//     EXPECT_CALL(_mqtt, setKeepAlive(::testing::Eq(keepAlive)));

//     _mgr.setKeepAliveClient(keepAlive);
// }

// Should I test for bigger that uint16_t type, or it's redundant? If redundant - please delete
// TEST_F(IMQTTTest, test_setKeepAlive_IsCalledValueGreaterThan65k) {
//    uint16_t keepAlive = 70000;
//    EXPECT_CALL(_mqtt, setKeepAlive(::testing::Eq(keepAlive)));

//    _mgr.setKeepAliveClient(keepAlive);
// }

// TEST_F(IMQTTTest, test_connect_WhenSucceeds_ReturnsTrue)
// {
//     EXPECT_CALL(_mqtt, connect()).Times(1).WillOnce(Return(true));
//     EXPECT_CALL(_mqtt, connected()).Times(1).WillOnce(Return(false));

//     ASSERT_TRUE(_mgr.connectToMQTT());
// }

// TEST_F(IMQTTTest, test_connect_WhenFails_ReturnsFalse)
// {
//     EXPECT_CALL(_mqtt, connect()).Times(1).WillOnce(Return(false));
//     EXPECT_CALL(_mqtt, connected()).Times(1).WillOnce(Return(false));

//     ASSERT_FALSE(_mgr.connectToMQTT());
// }

// TEST_F(IMQTTTest, test_connect_WhenPreviouslyConnected_ReturnsTrue)
// {
//     EXPECT_CALL(_mqtt, connect()).Times(0);
//     EXPECT_CALL(_mqtt, connected()).Times(1).WillOnce(Return(true));

//     ASSERT_TRUE(_mgr.connectToMQTT());
// }

// TEST_F(IMQTTTest, test_connected_WhenConnected_ReturnsTrue)
// {
//     EXPECT_CALL(_mqtt, connected()).Times(1).WillOnce(Return(true));

//     ASSERT_TRUE(_mgr.connectedToMQTT());
// }

// TEST_F(IMQTTTest, test_connected_WhenDisconnected_ReturnsFalse)
// {
//     EXPECT_CALL(_mqtt, connected()).Times(1).WillOnce(Return(false));

//     ASSERT_FALSE(_mgr.connectedToMQTT());
// }

// TEST_F(IMQTTTest, test_checkForIncomingMessages_IfNoErrors_ReturnsTrue)
// {
//     EXPECT_CALL(_mqtt, loop()).Times(1).WillOnce(Return(true));

//     ASSERT_TRUE(_mgr.checkForIncomingMessages());
// }

// TEST_F(IMQTTTest, test_checkForIncomingMessages_IfErrors_ReturnsFalse)
// {
//     EXPECT_CALL(_mqtt, loop()).Times(1).WillOnce(Return(false));

//     ASSERT_FALSE(_mgr.checkForIncomingMessages());
// }

// TEST_F(IMQTTTest, test_publishMessageToBroker_OnSucceed_ReturnsTrue)
// {
//     EXPECT_CALL(_mqtt, publish(_, _)).Times(1).WillOnce(Return(true));
//     char topic[] = "/topic1/channel1";
//     char message[] = "1";

//     ASSERT_TRUE(_mgr.publishMessageToBroker(topic, message));
// }

// TEST_F(IMQTTTest, test_publishMessageToBroker_OnFailure_ReturnsFalse)
// {
//     EXPECT_CALL(_mqtt, publish(_, _)).Times(1).WillOnce(Return(false));
//     char topic[] = "/topic1/channel1";
//     char message[] = "1";

//     ASSERT_FALSE(_mgr.publishMessageToBroker(topic, message));
// }

// TEST_F(IMQTTTest, test_publishMessageToBroker_IfNullPassed_ReturnsFalse)
// {
//     EXPECT_CALL(_mqtt, publish(_, _)).Times(0);
//     char *topic = nullptr;
//     char *message = nullptr;

//     ASSERT_FALSE(_mgr.publishMessageToBroker(topic, message));
// }

// TEST_F(IMQTTTest, test_subscribeToTopic_IfNullPassed_ReturnsFalse)
// {
//     // EXPECT_CALL(_mqtt, publish(_, _)).Times(0);
//     // char *topic = nullptr;
//     // char *message = nullptr;

//     // ASSERT_FALSE(_mgr.publishMessageToBroker(topic, message));
// }
