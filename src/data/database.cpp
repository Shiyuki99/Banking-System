#include "database.hpp"

namespace DB {

   MYSQL *Login() {
      bool succ = false;
      MYSQL *connection = nullptr;

      SQLConnection ConnectionDetails("localhost", "root", "sara", "MyBankDB");
      std::tie(succ, connection) = SQLSetupInit(ConnectionDetails);

      if (!succ) {
         std::cout << "Connection established successfully. ";
         return nullptr;
      }
      return connection;
   }

   /**
    * @brief Insert user into the SQL database.
    *
    * @param connection Connection Pointer<MYSQL*> to SQL Database
    * @param userData User data as array<std::string> formatted as [username, password, balance]
    * @return true If successfully added user data.
    * @return false if not
    */
   bool AddUser(MYSQL *connection, const std::array<std::string, 3> userData) {

      if (connection == nullptr) {
         std::cerr << "No connection is established! " << std::endl;
         return false;
      }

      const char *insertQuery = "INSERT INTO Users VALUES(?, ?, ?)";//Prepared SQL query

      MYSQL_STMT *stmt = mysql_stmt_init(connection);

      if (!stmt) {
         std::cerr << "Failed to initialize MYSQL_STMT. " << std::endl;
         return false;
      }

      // set prepared insert statement if not set successfully return false.
      if (mysql_stmt_prepare(stmt, insertQuery, strlen(insertQuery))) {
         std::cerr << "Failed to prepare statement: " << mysql_stmt_error(stmt) << std::endl;
         mysql_stmt_close(stmt);
         return false;
      }


      // initialize MYSQL bind with userData.
      MYSQL_BIND bind[3];
      memset(bind, 0, sizeof(bind));

      bind[0].buffer_type = MYSQL_TYPE_STRING;
      bind[0].buffer = (char *)userData[0].c_str();
      bind[0].buffer_length = userData[0].length() - 1;

      bind[1].buffer_type = MYSQL_TYPE_STRING;
      bind[1].buffer = (char *)userData[1].c_str();
      bind[1].buffer_length = userData[1].length() - 1;

      bind[2].buffer_type = MYSQL_TYPE_DOUBLE;
      bind[2].buffer = (char *)userData[2].c_str();
      bind[2].buffer_length = userData[2].length() - 1;


      // bind userdata with the statement and return false of error occurred.
      if (mysql_stmt_bind_param(stmt, bind)) {
         std::cerr << "Failed to bind parameters: " << mysql_stmt_error(stmt) << std::endl;
         mysql_stmt_close(stmt);
         return false;
      }


      // execute prepared statement and return false if error occurred.
      if (mysql_stmt_execute(stmt)) {
         std::cerr << "Failed to execute statement: " << mysql_stmt_errno(stmt) << std::endl;
         mysql_stmt_close(stmt);
         return false;
      }


      // close the statement and exit with success.
      mysql_stmt_close(stmt);
      std::cout << "User added successfully. " << std::endl;
      return true;

   }

}