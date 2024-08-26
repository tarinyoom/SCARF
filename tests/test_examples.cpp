#include "gtest/gtest.h"

// Example test case for the MOV module
TEST(MOVTest, BasicFunctionality) { EXPECT_EQ(1, 1); }

// Example main function to run all tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
