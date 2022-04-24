#include <gtest/gtest.h>
#include <ArduinoJson.h>

TEST(JSON, simple)
{
    StaticJsonDocument<200> doc;
    doc["sensor"] = "sname";
    doc["id"] = 0;
    doc["value"] = "1.1";
    char messageToSend[200];
    serializeJsonPretty(doc, messageToSend);
    printf("\n\n");
    printf(messageToSend);
    printf("\n\n\n");
}

TEST(JSON, two_nested)
{
    StaticJsonDocument<300> doc;
    doc[0]["sensor"] = "sname";
    doc[0]["id"] = 0;
    doc[0]["value"] = "1.1";
    doc[1]["sensor"] = "sn2";
    doc[1]["id"] = 1;
    doc[1]["value"] = "-44.1";
    char messageToSend[150];
    serializeJsonPretty(doc, messageToSend);
    printf("\n\n");
    printf(messageToSend);
    printf("\n\n\n");
}

TEST(JSON, nested_FOR_simple)
{
    StaticJsonDocument<128> doc;
    for (int i = 0; i < 3; i++)
    {
        doc[i]["sensor"] = "sname";
        doc[i]["id"] = i;
        doc[i]["value"] = i * 5;
    }
    char messageToSend[200];
    serializeJsonPretty(doc, messageToSend);
    printf("\n\n");
    printf(messageToSend);
    printf("\n\n\n");
}

TEST(JSON, nested_FOR_objects)
{
    const size_t capacity = JSON_ARRAY_SIZE(3) + 3 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(1);
    StaticJsonDocument<capacity> doc;
    doc["sensor"] = "static sensor";
    JsonArray measurements = doc.createNestedArray("measurements");
    for (int i = 0; i < 3; i++)
    {
        JsonObject data = measurements.createNestedObject();
        data["id"] = "23fsfs saf sdff as";
        data["value"] = "sdfasdas sfasdf asdf";
    }
    char messageToSend[352];
    serializeJsonPretty(doc, messageToSend);

    printf("\n\n-----size of StaticJSON: %d----", capacity);
    printf("\nmessage PRETTY:\n%s\r---SIZE: %d---\n\n\n", messageToSend, strlen(messageToSend));
}

TEST(JSON, dynamicObjects)
{
    size_t capacity = JSON_ARRAY_SIZE(4) + 4 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(2);
    DynamicJsonDocument doc(capacity);
    doc["sensor"] = "dynamic ojbect";
    JsonArray measurements = doc.createNestedArray("measurements");
    for (int i = 0; i < 4; i++)
    {
        JsonObject data = measurements.createNestedObject();
        data["id"] = i;
        data["value"] = i * 2;
    }
    char messageToSend[280];
    serializeJsonPretty(doc, messageToSend);

    printf("\n\n-----size of DynamicJSON: %d----", capacity);
    printf("\nmessage PRETTY:\n%s\r---SIZE: %d---\n\n\n", messageToSend, strlen(messageToSend));
}

TEST(JSON, dynamicArray)
{
    size_t capacity = JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(1);
    DynamicJsonDocument doc(capacity);
    doc["name"] = "dynamic_array";
    JsonArray data = doc.createNestedArray("data");
    for (int i = 0; i < 4; i++)
        data[i] = i;
    char messageToSend[50];
    serializeJson(doc, messageToSend);

    printf("\n\n-----size of DynamicJSON: %d----", capacity);
    printf("\nmessage MIN:\n%s\r---SIZE: %d----\n\n\n", messageToSend, strlen(messageToSend));
}
