#include "pch.hpp"

std::array<std::string, 3> userData = { "moka", "password", "100000" };

void *start() {
   std::cout << "\t---WELCOME TO THE BANK---" << std::endl;
   std::cout << "first you need to login into an account: " << std::endl;
   std::cout << "1- LOGIN (if you already have an account). " << std::endl;
   std::cout << "2- SIGN-UP (if this is your first time here). " << std::endl;
   std::cout << "0- If user want to quit the app. " << std::endl;

   int answer = 0;
   while (!(std::cin >> answer)) {

      std::cout << "Please enter a valid input [0/1/2]: " << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

   }
   if (answer == 0) {
      return NULL;
   }

   Account user(answer);

}
std::string username = "moka";

int main(int argc, char **argv) {
   (void)argc;
   (void)argv;
   //hello
   DB::SearchUser(DB::Login(), username);
   return 0;

   return 0;
}