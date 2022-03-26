#include <gmock/gmock.h>
//#include <stdint.h>
#include <IMQTT.h>

namespace sensor_manager
{
    class MockIMQTT : public IMQTT
    {
    public:
        MOCK_METHOD(bool, send, (const char *, const char *), (override));
        MOCK_METHOD(bool, receive, (), (override));
    };
}