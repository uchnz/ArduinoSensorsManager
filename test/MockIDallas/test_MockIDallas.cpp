#include <gtest/gtest.h>

#include <../test/SensorManager_test.h>

using ::testing::_;
using ::testing::Return;

class IDallasTest : public SensorManagerTest
{
};

TEST_F(IDallasTest, test_InitDallasSensors_Successful)
{
    EXPECT_CALL(_dallas, init).Times(1);

    _mgr.initDallasSensors();
}

TEST_F(IDallasTest, test_setSensorPrecision_Successful)
{
    EXPECT_CALL(_dallas, setSensorsPrecision(_)).Times(1);

    _mgr.setSensorsPrecision(9);
}

TEST_F(IDallasTest, test_requestCurrentTemperatures_Successful)
{
    EXPECT_CALL(_dallas, requestCurrentTemperatures).Times(1);

    _mgr.requestCurrentTemperatures();
}

TEST_F(IDallasTest, test_GetNumberOfSensors_ReturnsValidNumber)
{
    uint8_t expected = 2;
    EXPECT_CALL(_dallas, getNumberOfSensors).Times(1).WillOnce(Return(expected));

    uint8_t actual = _mgr.getNumberOfSensors();

    ASSERT_EQ(expected, actual);
}

TEST_F(IDallasTest, test_getTemperatureByID_ReturnsValidTemperature)
{
    float expected = 25.5;
    EXPECT_CALL(_dallas, getTemperatureByID(_)).Times(1).WillOnce(Return(expected));

    float actual = _mgr.getTemperatureByID(0);

    ASSERT_FLOAT_EQ(expected, actual);
}

TEST_F(IDallasTest, test_getAddressByID_ReturnsValidAddress)
{
    char *expected = "0x020203dfe";
    EXPECT_CALL(_dallas, getStringAddressByID(_)).Times(1).WillOnce(Return(expected));

    char *actual = _mgr.getStringAddressByID(0);

    ASSERT_STREQ(expected, actual);
}