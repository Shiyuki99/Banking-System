#include <account.hpp>

Account::Account(int answer) {

   if (answer == 1) {
      login();
   } else {
      createAccount();
   }

}

void Account::login() {

}