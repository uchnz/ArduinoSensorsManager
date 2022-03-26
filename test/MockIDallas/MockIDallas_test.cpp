#include <gtest/gtest.h>
#include <SensorManager/SensorManager_test.h>

using ::testing::_;
using ::testing::Return;

class IDallasTest : public SensorManagerTest
{
};

TEST_F(IDallasTest, test_initSensors_InitArrayOfTemperaturesToErrorValue_Success)
{
    EXPECT_CALL(_dallas, getNumberOfConnectedSensors).Times(1).WillOnce(Return(5));
    _mgr.scanConnectedTemperatureSensors();

    uint8_t num = _mgr.getSavedNumberSensors();
    ASSERT_EQ(num, 5);

    float actual = _mgr.getCurrentTemperatureByID(0);
    EXPECT_FLOAT_EQ(-128, actual);

    actual = _mgr.getCurrentTemperatureByID(2);
    EXPECT_FLOAT_EQ(-128, actual);

    actual = _mgr.getCurrentTemperatureByID(4);
    EXPECT_FLOAT_EQ(-128, actual);
}

TEST_F(IDallasTest, test_initSensors_WithoutAnySensorsAttached_NothingCreated)
{
    EXPECT_CALL(_dallas, getNumberOfConnectedSensors).Times(1).WillOnce(Return(0));
    _mgr.scanConnectedTemperatureSensors();

    uint8_t zero = _mgr.getSavedNumberSensors();
    ASSERT_EQ(zero, 0);

    float actual = _mgr.getCurrentTemperatureByID(0);
    EXPECT_FLOAT_EQ(-128, actual);
    actual = _mgr.getCurrentTemperatureByID(1);
    EXPECT_FLOAT_EQ(-128, actual);
}

TEST_F(IDallasTest, test_requestSensorsDataBeforeInit_ReturnsZeroAndErrorTemperature)
{
    EXPECT_CALL(_dallas, getNumberOfConnectedSensors).Times(0);

    uint8_t actual = _mgr.getSavedNumberSensors();
    ASSERT_EQ(actual, 0);

    float nothing = _mgr.getCurrentTemperatureByID(0);
    EXPECT_FLOAT_EQ(-128, nothing);
}

TEST_F(IDallasTest, test_refreshSensorsData_Success)
{
    EXPECT_CALL(_dallas, requestCurrentTemperatures).Times(1);
    EXPECT_CALL(_dallas, getNumberOfConnectedSensors).Times(1).WillOnce(Return(3));
    EXPECT_CALL(_dallas, getTemperatureByID).Times(3).WillOnce(Return(11.1)).WillOnce(Return(-0.55)).WillOnce(Return(22));

    _mgr.scanConnectedTemperatureSensors();

    uint8_t num = _mgr.getSavedNumberSensors();
    ASSERT_EQ(num, 3);

    EXPECT_TRUE(_mgr.refreshSensorsData());

    float actual = _mgr.getCurrentTemperatureByID(0);
    EXPECT_FLOAT_EQ(11.1, actual);

    actual = _mgr.getCurrentTemperatureByID(1);
    EXPECT_FLOAT_EQ(-0.55, actual);

    actual = _mgr.getCurrentTemperatureByID(2);
    EXPECT_FLOAT_EQ(22, actual);
}

TEST_F(IDallasTest, test_refreshSensorsData_BeforeInit_ReturnsBeforeCallingRequestFunction)
{
    EXPECT_CALL(_dallas, requestCurrentTemperatures).Times(0);

    uint8_t num = _mgr.getSavedNumberSensors();
    ASSERT_EQ(num, 0);

    EXPECT_FALSE(_mgr.refreshSensorsData());

    float actual = _mgr.getCurrentTemperatureByID(0);
    EXPECT_FLOAT_EQ(-128, actual);
}

// TEST_F(IDallasTest, test_getStringAddressByID_ReturnsValidAddress)
// {
//     //    char *expected = new char[10];
//     char expected[] = "0x020203dfe1";
//     EXPECT_CALL(_dallas, getStringAddressByID(_, _)).Times(1).WillOnce(::testing::SaveArgPointee<1>(expected));
//     //    EXPECT_CALL(_dallas, getStringAddressByID(_, _)).Times(1).WillOnce(::testing::SaveArgPointee<1>("qq"));
//     char actual[20] = "";
//     _mgr.getStringAddressByID(0, actual);

//     EXPECT_EQ(expected, actual);

//     // int expected = 17;
//     // EXPECT_CALL(_dallas, getStringAddressByID(_, _)).Times(1).WillOnce(::testing::SetArgPointee<1>(expected));
//     // //    EXPECT_CALL(_dallas, getStringAddressByID(_, _)).Times(1).WillOnce(::testing::SaveArgPointee<1>("qq"));
//     // int *actual = new int[1];
//     // _mgr.getStringAddressByID(0, actual);

//     // EXPECT_EQ(expected, *actual);
//     //    ASSERT_STREQ("0x020203dfe", actual);
// }

// TEST_F(IDallasTest, test_test_getStringAddressByIDWithIncorrectID_ReturnsEmptyString)
// {
//     // char expected[] = "";
//     // EXPECT_CALL(_dallas, getStringAddressByID(_, _)).Times(1);

//     // char actual[20];
//     // _mgr.getStringAddressByID(0, actual);

//     // ASSERT_STREQ(expected, actual);
// }

// TEST_F(IDallasTest, test_getTemperatureByID_BeforeFirstUpdate_ReturnsErrorTemperature)
// {
//     EXPECT_CALL(_dallas, getNumberOfConnectedSensors).Times(1).WillOnce(Return(0));
//     _mgr.scanConnectedTemperatureSensors();

//     float actual = _mgr.GetCurrentTemperatureByID(0);
//     ASSERT_FLOAT_EQ(-128, actual);
//     actual = _mgr.GetCurrentTemperatureByID(1);
//     ASSERT_FLOAT_EQ(-128, actual);
// }

// TEST_F(IDallasTest, test_getTemperatureByID_AfterUpdate_ReturnsTemperature)
// {
//     EXPECT_CALL(_dallas, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));
//     EXPECT_CALL(_dallas, getTemperatureByID).Times(2).WillOnce(Return(11.1)).WillOnce(Return(-0.5));
//     _mgr.scanConnectedTemperatureSensors();
//     _mgr.refreshSensorsData();

//     float actual = _mgr.GetCurrentTemperatureByID(0);
//     ASSERT_FLOAT_EQ(11.1, actual);
//     actual = _mgr.GetCurrentTemperatureByID(1);
//     ASSERT_FLOAT_EQ(-0.5, actual);
// }

// TEST_F(IDallasTest, test_getTemperatureByID_WithIncorrectID_ReturnsInvalidTemperature)
// {
//     EXPECT_CALL(_dallas, getNumberOfConnectedSensors).Times(1).WillOnce(Return(2));
//     EXPECT_CALL(_dallas, getTemperatureByID(_)).Times(0);
//     _mgr.scanConnectedTemperatureSensors();

//     float actual = _mgr.GetCurrentTemperatureByID(5);

//     ASSERT_FLOAT_EQ(-128, actual);
// }

// TEST_F(IDallasTest, test_requestCurrentTemperatures_ReturnsTemperature)
// {
//     EXPECT_CALL(_dallas, requestCurrentTemperatures).Times(1);

//     _mgr.refreshSensorsData();
// }
