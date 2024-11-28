#include <gtest/gtest.h>
#include "../src/data/database.hpp"
#include "../src/core/myfunctions.hpp"

std::array<std::string, 2> userData = { "Nasro",  "123" };





TEST(SearchUserTest, one) {
   EXPECT_EQ(DB::AddUser(DB::Login(), userData), true);
}

int main(int argc, char **argv) {
   (void)argc;
   (void)argv;
   /* ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS(); */
   MYSQL *conn = DB::Login();
   DB::AddUser(conn, userData);

   mysql_close(conn);
   return 0;
}