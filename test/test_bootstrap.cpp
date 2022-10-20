#include <gtest/gtest.h>


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
    ;

    return 0;
} 



class SMT_test : public ::testing::Test
{
protected:
   void SetUp() override
   {
   }

};

TEST_F(SMT_test, test_one)
{
    EXPECT_FALSE(false);
    
}
