#include <gtest/gtest.h>
#include "../src/data/database.hpp"

std::array<std::string, 3> userData = { "moka", "password", "100000" };




TEST(ExampleTestCase, one) {
   EXPECT_EQ(DB::AddUser(DB::Login(), userData), true);
}

int main(int argc, char **argv) {
   (void)argc;
   (void)argv;
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}