#include <gtest/gtest.h>
#include <SimpleActuator_test.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

TEST_F(SimpleActuatorTest, test_nameIsSavedCorrectly)
{
    char name[30];
    MockIOActuator io;
    SimpleActuator sa2("namezzzzz", io);
    EXPECT_EQ(9, sa2.getName(name));
    EXPECT_STREQ("namezzzzz", name);

    SimpleActuator sa("testname", io);
    EXPECT_EQ(8, sa.getName(name));
    EXPECT_STREQ("testname", name);
}

TEST_F(SimpleActuatorTest, test_emptyName_ReturnsZeroAndEmptyString)
{
    char name[30];
    MockIOActuator io;
    SimpleActuator sa2("", io);
    EXPECT_EQ(0, sa2.getName(name));
    EXPECT_STREQ("", name);
}

TEST_F(SimpleActuatorTest, test_nullName_ReturnsZeroAndEmptyString)
{
    char name[30];
    MockIOActuator io;
    SimpleActuator sa2(nullptr, io);
    EXPECT_EQ(0, sa2.getName(name));
    EXPECT_STREQ("", name);
}

TEST_F(SimpleActuatorTest, test_toBigName_TruncatesToMaxSize)
{
    char name[30];
    MockIOActuator io;
    SimpleActuator sa2("Name is to big to fit into member var", io);
    EXPECT_EQ(29, sa2.getName(name));
    EXPECT_STREQ("Name is to big to fit into me", name);
}

TEST_F(SimpleActuatorTest, test_getName_withNullptr)
{
    char *name = nullptr;
    MockIOActuator io;
    SimpleActuator sa2("Name", io);
    EXPECT_EQ(0, sa2.getName(name));
    EXPECT_EQ(nullptr, name);
}

TEST_F(SimpleActuatorTest, test_setName_savesCorrectName)
{
    char name[30];
    MockIOActuator io;
    SimpleActuator sa("old name", io);
    EXPECT_EQ(8, sa.getName(name));
    EXPECT_STREQ("old name", name);

    EXPECT_TRUE(sa.setName("correct name"));

    EXPECT_EQ(12, sa.getName(name));
    EXPECT_STREQ("correct name", name);
}

TEST_F(SimpleActuatorTest, test_setName_withNull_changesNothing)
{
    char name[30];
    MockIOActuator io;
    SimpleActuator sa("prev name", io);
    EXPECT_FALSE(sa.setName(nullptr));
    EXPECT_EQ(9, sa.getName(name));
    EXPECT_STREQ("prev name", name);
}

TEST_F(SimpleActuatorTest, test_setName_withEmptyString_changesNothing)
{
    char name[30];
    MockIOActuator io;
    SimpleActuator sa("prev name", io);
    EXPECT_FALSE(sa.setName(""));
    EXPECT_EQ(9, sa.getName(name));
    EXPECT_STREQ("prev name", name);
}

TEST_F(SimpleActuatorTest, test_setName_withToBigName_TruncatesToMaxSize)
{
    char name[30];
    MockIOActuator io;
    SimpleActuator sa("s", io);
    EXPECT_EQ(1, sa.getName(name));

    EXPECT_TRUE(sa.setName("Name is to big to fit into member var"));
    EXPECT_EQ(29, sa.getName(name));
    EXPECT_STREQ("Name is to big to fit into me", name);
}

TEST_F(SimpleActuatorTest, test_init_whenNoname_Fails)
{
    MockIOActuator io;
    SimpleActuator sa("", io);

    EXPECT_FALSE(sa.init());

    SimpleActuator sa2(nullptr, io);
    EXPECT_FALSE(sa2.init());
}

TEST_F(SimpleActuatorTest, test_init_whenSensorHasName_Successful)
{
    MockIOActuator io;
    EXPECT_CALL(io, init()).Times(1).WillOnce(Return(true));
    SimpleActuator sa("with name", io);

    EXPECT_TRUE(sa.init());
}

TEST_F(SimpleActuatorTest, test_executeActuator_withoutInit_ReturnsFalse)
{
    MockIOActuator io;
    EXPECT_CALL(io, init()).Times(0);
    SimpleActuator sa("with name", io);

    char payload[] = "ON";
    EXPECT_CALL(io, write(1)).Times(0);
    EXPECT_FALSE(sa.execute(payload));
}

TEST_F(SimpleActuatorTest, test_executeActuator_withPayload_ReturnsTrue)
{
    MockIOActuator io;
    EXPECT_CALL(io, init()).Times(1).WillOnce(Return(true));
    SimpleActuator sa("with name", io);
    EXPECT_TRUE(sa.init());

    char payload[] = "ON";
    EXPECT_CALL(io, write(1)).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(sa.execute(payload));

    char payload2[] = "OFF";
    EXPECT_CALL(io, write(0)).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(sa.execute(payload2));
}

TEST_F(SimpleActuatorTest, test_executeActuator_withBadPayload_ReturnsFalse)
{
    MockIOActuator io;
    EXPECT_CALL(io, init()).Times(1).WillOnce(Return(true));
    SimpleActuator sa("with name", io);
    EXPECT_TRUE(sa.init());

    char payload[] = "";
    EXPECT_CALL(io, write(_)).Times(0);
    EXPECT_FALSE(sa.execute(payload));

    char payload2[] = "bad data";
    EXPECT_CALL(io, write(_)).Times(0);
    EXPECT_FALSE(sa.execute(payload2));

    EXPECT_CALL(io, write(_)).Times(0);
    EXPECT_FALSE(sa.execute(nullptr));
}

TEST_F(SimpleActuatorTest, test_getActuatorState_ReturnsCurrentState)
{
    MockIOActuator io;
    EXPECT_CALL(io, init()).Times(1).WillOnce(Return(true));
    SimpleActuator sa("with name", io);
    EXPECT_TRUE(sa.init());

    char state[10];
    EXPECT_EQ(3, sa.getState(state));
    EXPECT_STREQ("OFF", state);

    char payload[] = "ON";
    EXPECT_CALL(io, write(1)).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(sa.execute(payload));
    EXPECT_EQ(2, sa.getState(state));
    EXPECT_STREQ("ON", state);

    char payload2[] = "OFF";
    EXPECT_CALL(io, write(0)).Times(1).WillOnce(Return(true));
    EXPECT_TRUE(sa.execute(payload2));
    EXPECT_EQ(3, sa.getState(state));
    EXPECT_STREQ("OFF", state);
}

TEST_F(SimpleActuatorTest, test_getActuatorState_withoutInit_ReturnsUnknownState)
{
    MockIOActuator io;
    EXPECT_CALL(io, init()).Times(0);
    SimpleActuator sa("with name", io);

    char state[10];
    EXPECT_EQ(7, sa.getState(state));
    EXPECT_STREQ("UNKNOWN", state);
}

TEST_F(SimpleActuatorTest, test_getActuatorState_withNullptr_ReturnsZero)
{
    MockIOActuator io;
    EXPECT_CALL(io, init()).Times(0);
    SimpleActuator sa("with name", io);

    EXPECT_EQ(0, sa.getState(nullptr));
}