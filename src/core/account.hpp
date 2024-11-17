#include "include.hpp"

class Account {
   std::string username = "";
   std::string password = "";
   double balance = 0;

public:
   Account(int answer);
   void login();
   void createAccount();
};

