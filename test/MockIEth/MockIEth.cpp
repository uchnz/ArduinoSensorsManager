#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdint.h>

#include <./../../test/MockIEth/MockIEth.hpp>

using ::testing::_;
using ::testing::Return;

// class IEthTest : public SensorManagerTest
// {
// };
// class IMQTTTest : public SensorManagerTest
// {
// };
// class IDallasTest : public SensorManagerTest
// {
// };

// IEth tests
TEST_F(IEthTest, test_ConnectWithDHCP_IsCalledOnce)
{
   EXPECT_CALL(_eth, connect).Times(1);
   uint8_t mac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

   _mgr.connectWithDHCP(mac);
}

// TEST_F(IEthTest, test_ConnectWithDHCP_CalledWithZeroMAC_ReturnsFalse)
// {
//    uint8_t mac[6] = {0};
//    ASSERT_FALSE(_mgr.connectWithDHCP(mac));
// }

// TEST_F(IEthTest, test_ConnectWithDHCP_CalledWithNonZeroMACLeft_ReturnsTrue)
// {
//    EXPECT_CALL(_eth, connect(_)).WillOnce(Return(true));
//    uint8_t mac[6] = {1, 0, 0, 0, 0, 0};

//    ASSERT_TRUE(_mgr.connectWithDHCP(mac));
// }

// TEST_F(IEthTest, test_ConnectWithDHCP_CalledWithNonZeroMACRight_ReturnsTrue)
// {
//    EXPECT_CALL(_eth, connect(_)).WillOnce(Return(true));
//    uint8_t mac[6] = {0, 0, 0, 0, 0, 1};

//    ASSERT_TRUE(_mgr.connectWithDHCP(mac));
// }

// TEST_F(IEthTest, test_ConnectWithDHCP_CalledWithNonZeroMACMiddle_ReturnsTrue)
// {
//    EXPECT_CALL(_eth, connect(_)).WillOnce(Return(true));
//    uint8_t mac[6] = {0, 0, 1, 0, 0, 0};

//    ASSERT_TRUE(_mgr.connectWithDHCP(mac));
// }

// // IMQTT tests
// TEST_F(IMQTTTest, test_InitMQTT_IsCalledOnce)
// {
//    EXPECT_CALL(_mqtt, begin(_)).Times(1);
//    char MQTTserverIP[] = "10.62.202.14";

//    _mgr.initMQTT(MQTTserverIP);
// }

// TEST_F(IMQTTTest, test_InitMQTT_CalledWithNull_ReturnsFalse)
// {
//    char *MQTTserverIP = nullptr;

//    ASSERT_FALSE(_mgr.initMQTT(MQTTserverIP));
// }

// TEST_F(IMQTTTest, test_InitMQTT_CalledWithCorrectIP_ReturnsTrue)
// {
//    EXPECT_CALL(_mqtt, begin(_)).WillOnce(Return(true));
//    char MQTTserverIP[] = "192.168.1.1";

//    ASSERT_TRUE(_mgr.initMQTT(MQTTserverIP));
// }

// TEST_F(IMQTTTest, test_InitMQTT_CalledWithIncorrectIP_ReturnsFalse)
// {
//    EXPECT_CALL(_mqtt, begin(_)).WillOnce(Return(false));
//    char MQTTserverIP[] = "not ip";

//    ASSERT_FALSE(_mgr.initMQTT(MQTTserverIP));
// }

// TEST_F(IMQTTTest, test_installCallback_IsCalledWithNull_ReturnsFalse)
// {
//    EXPECT_CALL(_mqtt, onMessage(_)).Times(0);
//    sensor_manager::MQTTClientCallback cb = nullptr;

//    ASSERT_FALSE(_mgr.installCallback(cb));
// }

// TEST_F(IMQTTTest, test_installCallback_IsCalledCorrectly_ReturnsTrue)
// {
//    EXPECT_CALL(_mqtt, onMessage(_)).Times(1);
//    sensor_manager::MQTTClientCallback cb = _mgr.callbackIncommingMessages;
//    ASSERT_TRUE(_mgr.installCallback(cb));
// }

// TEST_F(IMQTTTest, test_setKeepAlive_IsSet)
// {
//    uint16_t keepAlive = 653;
//    EXPECT_CALL(_mqtt, setKeepAlive(::testing::Eq(keepAlive)));

//    _mgr.setKeepAliveClient(keepAlive);
// }

// TEST_F(IMQTTTest, test_setKeepAlive_IsCalledWithNegativeValue)
// {
//    uint16_t keepAlive = -50;
//    EXPECT_CALL(_mqtt, setKeepAlive(::testing::Eq(keepAlive)));

//    _mgr.setKeepAliveClient(keepAlive);
// }

// TEST_F(IMQTTTest, test_setKeepAlive_IsCalledWithZeroValue)
// {
//    uint16_t keepAlive = 0;
//    EXPECT_CALL(_mqtt, setKeepAlive(::testing::Eq(keepAlive)));

//    _mgr.setKeepAliveClient(keepAlive);
// }

// // Should I test for bigger that uint16_t type, or it's redundant? If redundant - please delete
// // TEST_F(IMQTTTest, test_setKeepAlive_IsCalledValueGreaterThan65k) {
// //    uint16_t keepAlive = 70000;
// //    EXPECT_CALL(_mqtt, setKeepAlive(::testing::Eq(keepAlive)));

// //    _mgr.setKeepAliveClient(keepAlive);
// // }

// TEST_F(IMQTTTest, test_connect_WhenSucceeds_ReturnsTrue)
// {
//    EXPECT_CALL(_mqtt, connect()).Times(1).WillOnce(Return(true));
//    EXPECT_CALL(_mqtt, connected()).Times(1).WillOnce(Return(false));

//    ASSERT_TRUE(_mgr.connectToMQTT());
// }

// TEST_F(IMQTTTest, test_connect_WhenFails_ReturnsFalse)
// {
//    EXPECT_CALL(_mqtt, connect()).Times(1).WillOnce(Return(false));
//    EXPECT_CALL(_mqtt, connected()).Times(1).WillOnce(Return(false));

//    ASSERT_FALSE(_mgr.connectToMQTT());
// }

// TEST_F(IMQTTTest, test_connect_WhenPreviouslyConnected_ReturnsTrue)
// {
//    EXPECT_CALL(_mqtt, connect()).Times(0);
//    EXPECT_CALL(_mqtt, connected()).Times(1).WillOnce(Return(true));

//    ASSERT_TRUE(_mgr.connectToMQTT());
// }

// TEST_F(IMQTTTest, test_connected_WhenConnected_ReturnsTrue)
// {
//    EXPECT_CALL(_mqtt, connected()).Times(1).WillOnce(Return(true));

//    ASSERT_TRUE(_mgr.connectedToMQTT());
// }

// TEST_F(IMQTTTest, test_connected_WhenDisconnected_ReturnsFalse)
// {
//    EXPECT_CALL(_mqtt, connected()).Times(1).WillOnce(Return(false));

//    ASSERT_FALSE(_mgr.connectedToMQTT());
// }

// TEST_F(IMQTTTest, test_checkForIncomingMessages_IfNoErrors_ReturnsTrue)
// {
//    EXPECT_CALL(_mqtt, loop()).Times(1).WillOnce(Return(true));

//    ASSERT_TRUE(_mgr.checkForIncomingMessages());
// }

// TEST_F(IMQTTTest, test_checkForIncomingMessages_IfErrors_ReturnsFalse)
// {
//    EXPECT_CALL(_mqtt, loop()).Times(1).WillOnce(Return(false));

//    ASSERT_FALSE(_mgr.checkForIncomingMessages());
// }

// TEST_F(IMQTTTest, test_publishMessageToBroker_OnSucceed_ReturnsTrue)
// {
//    EXPECT_CALL(_mqtt, publish(_, _)).Times(1).WillOnce(Return(true));
//    char topic[] = "/topic1/channel1";
//    char message[] = "1";

//    ASSERT_TRUE(_mgr.publishMessageToBroker(topic, message));
// }

// TEST_F(IMQTTTest, test_publishMessageToBroker_OnFailure_ReturnsFalse)
// {
//    EXPECT_CALL(_mqtt, publish(_, _)).Times(1).WillOnce(Return(false));
//    char topic[] = "/topic1/channel1";
//    char message[] = "1";

//    ASSERT_FALSE(_mgr.publishMessageToBroker(topic, message));
// }

// TEST_F(IMQTTTest, test_publishMessageToBroker_IfNullPassed_ReturnsFalse)
// {
//    EXPECT_CALL(_mqtt, publish(_, _)).Times(0);
//    char *topic = nullptr;
//    char *message = nullptr;

//    ASSERT_FALSE(_mgr.publishMessageToBroker(topic, message));
// }

// // IDallas tests
// TEST_F(IDallasTest, test_InitDallasSensors_Successful)
// {
//    EXPECT_CALL(_dallas, initSensors).Times(1);

//    _mgr.initDallasSensors();
// }

// TEST_F(IDallasTest, test_setSensorPrecision_Successful)
// {
//    EXPECT_CALL(_dallas, setSensorsPrecision(_)).Times(1);

//    _mgr.setSensorsPrecision(9);
// }

// TEST_F(IDallasTest, test_requestCurrentTemperatures_Successful)
// {
//    EXPECT_CALL(_dallas, requestCurrentTemperatures).Times(1);

//    _mgr.requestCurrentTemperatures();
// }

// TEST_F(IDallasTest, test_GetNumberOfSensors_ReturnsValidNumber)
// {
//    uint8_t expected = 2;
//    EXPECT_CALL(_dallas, getNumberOfSensors).Times(1).WillOnce(Return(expected));

//    uint8_t actual = _mgr.getNumberOfSensors();

//    ASSERT_EQ(expected, actual);
// }

// TEST_F(IDallasTest, test_getTemperatureByID_ReturnsValidTemperature)
// {
//    float expected = 25.5;
//    EXPECT_CALL(_dallas, getTemperatureByID(_)).Times(1).WillOnce(Return(expected));

//    float actual = _mgr.getTemperatureByID(0);

//    ASSERT_FLOAT_EQ(expected, actual);
// }

// TEST_F(IDallasTest, test_getAddressByID_ReturnsValidAddress)
// {
//    char *expected = "0x020203dfe";
//    EXPECT_CALL(_dallas, getStringAddressByID(_)).Times(1).WillOnce(Return(expected));

//    char *actual = _mgr.getStringAddressByID(0);

//    ASSERT_STREQ(expected, actual);
// }

/////////////////////////////
///////END OF Tests//////////
/////////////////////////////

int main(int argc, char **argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}