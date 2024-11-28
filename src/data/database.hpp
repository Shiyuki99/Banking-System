#ifndef DATABASE_HPP
#define DATABASE_HPP


#include "../core/include.hpp"
#include "SQL.hpp"

namespace DB {
   MYSQL *Login();
   unsigned int AddUser(MYSQL *connection, const std::array<std::string, 2> &in_UserData);
   bool SearchUser(MYSQL *connection, const std::string &username);
}


#endif