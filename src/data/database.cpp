#include "database.hpp"
struct DataPair {
   std::string data;
   enum_field_types type;

   DataPair(const std::string &data, enum_field_types type)
      : data(data), type(type) {
   }
};
namespace DB {


   /**
    * @brief Login into mariaDB
    *
    * @return MYSQL*  return MYSQL pointer to the connection established nullptr if not
    */
   MYSQL *Login() {
      bool succ = false;
      MYSQL *connection = nullptr;

      SQLConnection ConnectionDetails("localhost", "root", "sara", "MyBankDB");
      std::tie(succ, connection) = SQLSetupInit(ConnectionDetails);

      if (!succ) {
         std::cerr << "Connection established successfully. ";
         return nullptr;
      }
      return connection;
   }



   void bindDataPair(MYSQL_BIND *bind, const DataPair &datapair) {

      bind->is_null = 0;
      bind->length = 0;

      if (datapair.type == MYSQL_TYPE_STRING) {
         bind->buffer_type = datapair.type;
         bind->buffer = (char *)datapair.data.c_str();
         bind->buffer_length = datapair.data.length();

      } else {
         std::shared_ptr<double> data = std::make_shared<double>(std::stod(datapair.data));

         bind->buffer_type = datapair.type;
         bind->buffer = data.get();
         bind->buffer_length = sizeof(data);
         std::cout << "here ?" << *data << " " << bind->buffer_type << std::endl;
      }
      return;
   }

   /**
  * @brief Executes a prepared SQL statement with user-provided data and optionally retrieves the result set.
  *
  * @param connection A pointer to the active MySQL database connection.
  * @param preparedQuery The SQL statement with parameter placeholders (e.g., "SELECT * FROM users WHERE id = ?").
  * @param userData A vector of DataPair structs containing the values and data types to bind to the statement parameters.
  * @param[out] RESULT A pointer to a MYSQL_RES struct that will hold the result set metadata (optional).
  * @return true if the statement executed successfully, false otherwise.
  */
   unsigned int preparedQueryExecute(MYSQL *connection, const std::string &preparedQuery, const std::vector<DataPair> &userData, MYSQL_RES *RESULT = nullptr) {

      if (connection == nullptr) {
         std::cerr << "No connection to MYSQL server please try again later. " << std::endl;
         return 1;
      }

      MYSQL_STMT *stmt = mysql_stmt_init(connection);
      if (!stmt) {
         std::cerr << "Failed to initialize MYSQL_STMT. " << std::endl;
         return 1;
      }

      if (mysql_stmt_prepare(stmt, preparedQuery.c_str(), preparedQuery.length())) {
         std::cerr << "Failed to prepare statement: " << mysql_stmt_error(stmt) << std::endl;
         unsigned int error = mysql_stmt_errno(stmt);
         mysql_stmt_close(stmt);
         return error;
      }


      size_t size = userData.size();

      std::unique_ptr<MYSQL_BIND[]> bind(new MYSQL_BIND[size]);
      memset(bind.get(), 0, size * sizeof(MYSQL_BIND));

      for (size_t i = 0; i < size; i++) {
         bindDataPair(bind.get() + i, userData[i]);
      }



      // bind userdata with the statement and return false of error occurred.
      if (mysql_stmt_bind_param(stmt, bind.get())) {
         std::cerr << "Failed to bind parameters: " << mysql_stmt_error(stmt) << std::endl;
         unsigned int error = mysql_stmt_errno(stmt);
         mysql_stmt_close(stmt);
         return error;
      }


      // execute prepared statement and return false if error occurred.
      if (mysql_stmt_execute(stmt)) {
         std::cerr << "Failed to execute statement: " << mysql_stmt_error(stmt) << std::endl;
         unsigned int error = mysql_stmt_errno(stmt);
         mysql_stmt_close(stmt);
         return error;
      }

      if (RESULT != nullptr) {
         RESULT = mysql_stmt_result_metadata(stmt);
      }

      mysql_stmt_close(stmt);
      std::cout << "statement executed succ. " << std::endl;
      return 0;

   }

   /**
    * @brief Insert user into the SQL database.
    *
    * @param connection Connection Pointer<MYSQL*> to SQL Database
    * @param userData User data as array<std::string> formatted as [username, password, balance]
    * @return true If successfully added user data, false otherwise.
    */
   bool AddUser(MYSQL *connection, const std::array<std::string, 3> &in_UserData) {

      const char *insertQuery = "INSERT INTO Users VALUES(?, ?, ?)";//Prepared SQL query

      std::vector<DataPair> UserData;
      UserData.reserve(3);

      UserData.emplace_back(in_UserData[0], MYSQL_TYPE_STRING);
      UserData.emplace_back(in_UserData[1], MYSQL_TYPE_STRING);
      UserData.emplace_back(in_UserData[2], MYSQL_TYPE_DOUBLE);

      if (unsigned int error = preparedQueryExecute(connection, insertQuery, UserData)) {
         std::cerr << error << std::endl;
      }
      return true;

   }

   /**
    * @brief search for a user in SQL Database.
    *
    * @param connection Connection Pointer<MYSQL*> to SQL Database.
    * @param username username want to find.
    * @return true If successfully find user, false otherwise.
    */
   bool SearchUser(MYSQL *connection, const std::string &username) {

      const char *searchQuery = "SELECT * FROM Users WHERE Username = ?;";

      std::vector<DataPair> UserData;
      UserData.emplace_back(username, MYSQL_TYPE_STRING);

      MYSQL_RES *result;
      if (!preparedQueryExecute(connection, searchQuery, UserData, result)) {
         return false;
      }

      int numColumns = mysql_num_fields(result);
      MYSQL_FIELD *fields = mysql_fetch_fields(result);
      MYSQL_ROW row;


      while ((row = mysql_fetch_row(result))) {

         for (size_t i = 0; i < numColumns; i++) {
            std::cout << fields[i].name << ": " << row[i] << std::endl;
         }
      }
      mysql_free_result(result);

      return true;

   }

}