#ifndef DATABASE_HPP
#define DATABASE_HPP

#include<../core/pch.hpp>
namespace DB {
   bool Login();
   bool AddUser(MYSQL *connection, std::string Query);
}


#endif