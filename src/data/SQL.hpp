#ifndef SQL_hpp
#define SQL_hpp

#include "../core/include.hpp"


typedef struct SQLQueryOutput {
   bool success;
   MYSQL_RES *result;
}SQLQueryOutput;


typedef struct SQLConnection {
   std::string server, user, password, database;
   SQLConnection(const std::string &server, const std::string &user, const std::string &password, const std::string &database) {
      this->server = server;
      this->user = user;
      this->password = password;
      this->database = database;
   }


}SQLConnection;


std::tuple<bool, MYSQL *> SQLSetupInit(SQLConnection sqlLogin);
SQLQueryOutput SQLQuery(MYSQL *connection, const std::string &query);

#endif