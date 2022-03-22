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

TEST_F(IDallasTest, test_getStringAddressByID_ReturnsValidAddress)
{
    //    char *expected = new char[10];
    char expected[] = "0x020203dfe1";
    EXPECT_CALL(_dallas, getStringAddressByID(_, _)).Times(1).WillOnce(::testing::SaveArgPointee<1>(expected));
    //    EXPECT_CALL(_dallas, getStringAddressByID(_, _)).Times(1).WillOnce(::testing::SaveArgPointee<1>("qq"));
    char actual[20] = "";
    _mgr.getStringAddressByID(0, actual);

    EXPECT_EQ(expected, actual);

    // int expected = 17;
    // EXPECT_CALL(_dallas, getStringAddressByID(_, _)).Times(1).WillOnce(::testing::SetArgPointee<1>(expected));
    // //    EXPECT_CALL(_dallas, getStringAddressByID(_, _)).Times(1).WillOnce(::testing::SaveArgPointee<1>("qq"));
    // int *actual = new int[1];
    // _mgr.getStringAddressByID(0, actual);

    // EXPECT_EQ(expected, *actual);
    //    ASSERT_STREQ("0x020203dfe", actual);
}

TEST_F(IDallasTest, test_test_getStringAddressByIDWithIncorrectID_ReturnsEmptyString)
{
    // char expected[] = "";
    // EXPECT_CALL(_dallas, getStringAddressByID(_, _)).Times(1);

    // char actual[20];
    // _mgr.getStringAddressByID(0, actual);

    // ASSERT_STREQ(expected, actual);
}

TEST_F(IDallasTest, test_getSavedNumberOfSensors_ReturnsValidNumber)
{
    EXPECT_CALL(_dallas, getNumberOfSensors).Times(1).WillOnce(Return(7));
    _mgr.initSensors();
    uint8_t actual = _mgr.getSavedNumberSensors();

    ASSERT_EQ(actual, 7);
}

TEST_F(IDallasTest, test_requestSensorsDataBeforeInit_ReturnsZeroAndErrorTemperature)
{
    EXPECT_CALL(_dallas, getNumberOfSensors).Times(0);

    uint8_t actual = _mgr.getSavedNumberSensors();

    ASSERT_EQ(actual, 0);
    float nothing = _mgr.GetCurrentTemperatureByID(0);
    EXPECT_FLOAT_EQ(-128, nothing);
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

TEST_F(IDallasTest, test_initSensorsWithoutAnySensorsAttached_NothingCreated)
{
    EXPECT_CALL(_dallas, getNumberOfSensors).Times(1).WillOnce(Return(0));
    _mgr.initSensors();

    uint8_t zero = _mgr.getSavedNumberSensors();
    ASSERT_EQ(zero, 0);

    float actual = _mgr.GetCurrentTemperatureByID(0);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.GetCurrentTemperatureByID(1);
    EXPECT_FLOAT_EQ(-128, actual);
}

TEST_F(IDallasTest, test_getTemperatureByIDBeforeFirstUpdate_ReturnsErrorTemperature)
{
    EXPECT_CALL(_dallas, getNumberOfSensors).Times(1).WillOnce(Return(2));
    EXPECT_CALL(_dallas, getTemperatureByID).Times(0);
    _mgr.initSensors();

    float actual = _mgr.GetCurrentTemperatureByID(0);
    ASSERT_FLOAT_EQ(-128, actual);
    actual = _mgr.GetCurrentTemperatureByID(1);
    ASSERT_FLOAT_EQ(-128, actual);
}

TEST_F(IDallasTest, test_getTemperatureByIDAfterUpdate_ReturnsTemperature)
{
    EXPECT_CALL(_dallas, getNumberOfSensors).Times(1).WillOnce(Return(2));
    EXPECT_CALL(_dallas, getTemperatureByID).Times(2).WillOnce(Return(11.1)).WillOnce(Return(-0.5));
    _mgr.initSensors();
    _mgr.requestCurrentTemperatures();

    float actual = _mgr.GetCurrentTemperatureByID(0);
    ASSERT_FLOAT_EQ(11.1, actual);
    actual = _mgr.GetCurrentTemperatureByID(1);
    ASSERT_FLOAT_EQ(-0.5, actual);
}

TEST_F(IDallasTest, test_getTemperatureByIDWithIncorrectID_ReturnsInvalidTemperature)
{
    EXPECT_CALL(_dallas, getNumberOfSensors).Times(1).WillOnce(Return(2));
    EXPECT_CALL(_dallas, getTemperatureByID(_)).Times(0);
    _mgr.initSensors();

    float actual = _mgr.GetCurrentTemperatureByID(5);

    ASSERT_FLOAT_EQ(-128, actual);
}