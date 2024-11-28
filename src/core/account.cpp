#include "pch.hpp"

const size_t MAX_INPUT_SIZE = 64;


Account::Account(int answer) {

   if (answer == 1) {
      login();
   } else {
      createAccount();
   }

}

void Account::login() {
   std::cout << "\n\t\t Hello To Bank Account Creation Please Follow The Instructions:\n"
      << "Please input your name: " << std::endl;

   char *username = getInput();

   std::cout << "Hello " << username << std::endl;
   std::cout << "Please enter your password: " << std::endl;

   char *password = getInput();

   std::array<std::string, 2> UserData = { username, hashString(password) };

   if (DB::AddUser(connection, UserData) == 1062) {
      std::cout << "User already exists!" << std::endl;
      delete[] username;
      delete[] password;
   }
   this->username = username;
   this->password = password;
   delete[] username;
   delete[] password;
   std::cout << "Account created succ." << std::endl;


}


