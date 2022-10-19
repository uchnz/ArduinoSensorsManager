#include <gtest/gtest.h>

#include "MockIEth.h"
#include "../MockSensorManager/MockSensorManager_test.h"


using ::testing::_;
using ::testing::Return;

class IEthTest : public SensorManagerTest
{
};

TEST_F(IEthTest, test_ConnectWithDHCP_IsCalledOnce)
{
    EXPECT_CALL(_eth, connect).Times(1);
    uint8_t mac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

    _mgr.connectWithDHCP(mac);
}

TEST_F(IEthTest, test_ConnectWithDHCP_CalledWithZeroMAC_ReturnsFalse)
{
    uint8_t mac[6] = {0};
    ASSERT_FALSE(_mgr.connectWithDHCP(mac));
}

TEST_F(IEthTest, test_ConnectWithDHCP_CalledWithNonZeroMACLeft_ReturnsTrue)
{
    EXPECT_CALL(_eth, connect(_)).WillOnce(Return(true));
    uint8_t mac[6] = {1, 0, 0, 0, 0, 0};

    ASSERT_TRUE(_mgr.connectWithDHCP(mac));
}

TEST_F(IEthTest, test_ConnectWithDHCP_CalledWithNonZeroMACRight_ReturnsTrue)
{
    EXPECT_CALL(_eth, connect(_)).WillOnce(Return(true));
    uint8_t mac[6] = {0, 0, 0, 0, 0, 1};

    ASSERT_TRUE(_mgr.connectWithDHCP(mac));
}

TEST_F(IEthTest, test_ConnectWithDHCP_CalledWithNonZeroMACMiddle_ReturnsTrue)
{
    EXPECT_CALL(_eth, connect(_)).WillOnce(Return(true));
    uint8_t mac[6] = {0, 0, 1, 0, 0, 0};

    ASSERT_TRUE(_mgr.connectWithDHCP(mac));
}
