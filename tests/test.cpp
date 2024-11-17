#include <gtest/gtest.h>
#include "../src/data/database.hpp"

std::array<std::string, 3> userData = { "Nasro", "1234", "99999999.99" };





TEST(SearchUserTest, one) {
   EXPECT_EQ(DB::AddUser(DB::Login(), userData), true);
}

int main(int argc, char **argv) {
   (void)argc;
   (void)argv;
   /* ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS(); */
   DB::AddUser(DB::Login(), userData);
   return 0;
}