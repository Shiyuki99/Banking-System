#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "../core/include.hpp"
#include "SQL.hpp"

namespace DB {
   MYSQL *Login();
   bool AddUser(MYSQL *connection, const std::array<std::string, 3> userData);
}


#endif