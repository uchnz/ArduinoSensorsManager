#include <gmock/gmock.h>
#include <stdint.h>
#include <IMQTT.h>


namespace sensor_manager
{
    class MockIMQTT : public IMQTT
    {
        public:
            MOCK_METHOD(bool, begin, (char *srvIP), (override));
            MOCK_METHOD(bool, onMessage, (MQTTClientCallback cb), (override));
            MOCK_METHOD(void, setKeepAlive, (uint16_t keepAlive), (override));
            MOCK_METHOD(bool, connect, (), (override));
            MOCK_METHOD(bool, connected, (), (override));
            MOCK_METHOD(bool, loop, (), (override));
            MOCK_METHOD(bool, publish, (char *topic, char *data), (override));
    };
}