#include <gtest/gtest.h>

#include <SensorManager/SensorManager_test.h>

using ::testing::_;
using ::testing::Return;

class IDallasTest : public SensorManagerTest
{
};

// TEST_F(IDallasTest, test_InitDallasSensors_Successful)
// {
//     EXPECT_CALL(_dallas, init).Times(1);

//     _mgr.initDallasSensors();
// }

// TEST_F(IDallasTest, test_setSensorPrecision_Successful)
// {
//     EXPECT_CALL(_dallas, setSensorsPrecision(_)).Times(1);

//     _mgr.setSensorsPrecision(9);
// }

TEST_F(IDallasTest, test_getAddressByID_ReturnsValidAddress)
{
    char *expected = "0x020203dfe";
    EXPECT_CALL(_dallas, getStringAddressByID(_)).Times(1).WillOnce(Return(expected));

    char *actual = _mgr.getStringAddressByID(0);

    ASSERT_STREQ(expected, actual);
}

TEST_F(IDallasTest, test_requestCurrentTemperatures_Successful)
{
    EXPECT_CALL(_dallas, requestCurrentTemperatures).Times(1);

    _mgr.requestCurrentTemperatures();
}

TEST_F(IDallasTest, test_GetNumberOfSensors_ReturnsValidNumber)
{
    EXPECT_CALL(_dallas, getNumberOfSensors).Times(1).WillOnce(Return(2));

    uint8_t actual = _mgr.getNumberOfSensors();

    ASSERT_EQ(2, actual);
}

TEST_F(IDallasTest, test_initSensors_InitArrayOfTemperaturesToZero)
{
    EXPECT_CALL(_dallas, getNumberOfSensors).Times(1).WillOnce(Return(5));
    _mgr.initSensors();

    float actual = _mgr.GetCurrentTemperatureByID(0);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.GetCurrentTemperatureByID(2);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.GetCurrentTemperatureByID(4);
    EXPECT_FLOAT_EQ(-128, actual);
}

TEST_F(IDallasTest, test_getTemperatureByIDAfterUpdate_ReturnsTemperature)
{
    EXPECT_CALL(_dallas, getNumberOfSensors).Times(1).WillOnce(Return(2));
    EXPECT_CALL(_dallas, getTemperatureByID).Times(2).WillOnce(Return(11.1)).WillOnce(Return(-0.5));
    _mgr.initSensors();
    _mgr.updateAllTemperatures();

    float actual = _mgr.getTemperatureByID(0);
    ASSERT_FLOAT_EQ(11.1, actual);
    actual = _mgr.getTemperatureByID(1);
    ASSERT_FLOAT_EQ(-0.5, actual);
}

TEST_F(IDallasTest, test_getTemperatureByIDWithIncorrectID_ReturnsInvalidTemperature)
{
    EXPECT_CALL(_dallas, getNumberOfSensors).Times(1).WillOnce(Return(2));
    EXPECT_CALL(_dallas, getTemperatureByID(_)).Times(0);
    _mgr.initSensors();

    float actual = _mgr.getTemperatureByID(3);

    ASSERT_FLOAT_EQ(-128, actual);
}

// TEST_F(IDallasTest, test_UpdateTepmerature_ReturnsTrue)
// {
//     EXPECT_CALL(_dallas, getTemperatureByID(_)).Times(2).WillOnce(Return(12.5)).WillOnce(Return(-10.1));

//     _mgr.updateTemperature(0);
//     float actual = _mgr.GetCurrentTemperatureByID(0);
//     EXPECT_FLOAT_EQ(actual, 12.5);

//     _mgr.updateTemperature(1);
//     actual = _mgr.GetCurrentTemperatureByID(1);
//     EXPECT_FLOAT_EQ(actual, -10.1);
// }

// TEST_F(IDallasTest, test_GetCurrentTemperature_ReturnsTheSameWithoutUpdate)
// {
//     float expected = 22.7;
//     EXPECT_CALL(_dallas, getTemperatureByID(_)).Times(1).WillOnce(Return(expected));
//     _mgr.updateTemperature(0);

//     float actual = _mgr.getCurrentTemperature();
//     EXPECT_FLOAT_EQ(actual, expected);

//     actual = 0;
//     actual = _mgr.getCurrentTemperature();
//     EXPECT_FLOAT_EQ(actual, expected);
// }

// TEST_F(IDallasTest, test_GetCurrentTemperature_ReturnsLowestValueWithoutUpdate)
// {
//     float expected = -128;
//     EXPECT_CALL(_dallas, getTemperatureByID(_)).Times(0);

//     float actual = _mgr.getCurrentTemperature();
//     EXPECT_FLOAT_EQ(actual, expected);
// }

// TEST_F(IDallasTest, test_GetCurrentTemperatureByID_ReturnsTemperature)
// {
//     float expected = 22.7;
//     EXPECT_CALL(_dallas, getTemperatureByID(_)).Times(1).WillOnce(Return(expected));
//     _mgr.updateTemperature(0);

//     float actual = _mgr.GetCurrentTemperatureByID(0);
//     EXPECT_FLOAT_EQ(actual, expected);
// }
