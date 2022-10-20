#pragma once

#include <gtest/gtest.h>
#include <SensorManager.h>
#include <test_MockIEth/MockIEth.h>
#include <test_MockIMQTT/MockIMQTT_test.h>
#include <test_MockIDallas/MockIDallas.h>

// using ::testing::_;

class SensorManagerTest : public ::testing::Test
{
protected:
   void SetUp() override
   {
   }

   sensor_manager::MockIEth _eth;
   sensor_manager::MockIMQTT _mqtt;
   sensor_manager::MockIDallas _dallas;
   sensor_manager::SensorManager _mgr{_eth, _mqtt, _dallas};
};
