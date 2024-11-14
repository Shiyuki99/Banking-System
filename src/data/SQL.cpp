#include "SQL.hpp"

/**
 * @brief
 * Function to initialize connection with mysql database.
 * @param sqlLogin Login information for your sql.
 * @return std::tuple<bool, MYSQL *>
 */
std::tuple<bool, MYSQL *> SQLSetupInit(SQLConnection sqlLogin)
{
   MYSQL *connection = mysql_init(NULL);
   bool succ = true;
   if (!mysql_real_connect(connection, sqlLogin.server.c_str(), sqlLogin.user.c_str(), sqlLogin.password.c_str(), sqlLogin.database.c_str(), 0, NULL, 0)) {
      succ = false;
      std::cout << "Connection error: " << mysql_error(connection) << std::endl;
   }

   return std::make_tuple(succ, connection);
}


/**
 * @brief Function to execute SQL Queries
 *
 * @param conn MYSQL* connection
 * @param query String of query
 * @return SQLQueryOutput with success bool value and SQL result
 */
SQLQueryOutput SQLQuery(MYSQL *conn, const std::string &query) {



   bool success = true;

   if (mysql_query(conn, query.c_str())) {
      std::cout << "failed to get query, ERROR: " << mysql_error(conn) << std::endl;
      success = false;
   }

   return {
      success,
      mysql_use_result(conn)
   };

}
