#include <gmock/gmock.h>
//#include <stdint.h>
#include <IMQTT.h>

namespace sensor_manager
{
    class MockIMQTT : public IMQTT
    {
    public:
        MOCK_METHOD(bool, begin, (char *), (override));
        MOCK_METHOD(void, onMessage, (MQTTClientCallback), (override));
        MOCK_METHOD(void, setKeepAlive, (uint16_t), (override));
        MOCK_METHOD(bool, connect, (), (override));
        MOCK_METHOD(bool, connected, (), (override));
        MOCK_METHOD(bool, loop, (), (override));
        MOCK_METHOD(bool, publish, (char *, char *), (override));
        MOCK_METHOD(void, subscribeToTopic, (const char *), (override));
        MOCK_METHOD(bool, send, (const char *, const char *), (override));
        MOCK_METHOD(bool, receive, (), (override));
    };
}