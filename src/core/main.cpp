#include "pch.hpp"

std::array<std::string, 3> userData = { "moka", "password", "100000" };


int main(int argc, char **argv) {
   (void)argc;
   (void)argv;
   //hello
   DB::AddUser(DB::Login(), userData);
   return 0;

   return 0;
}