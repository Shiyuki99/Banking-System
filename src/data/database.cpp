#include "database.hpp"
struct DataPair {
   char *data = nullptr;
   enum_field_types type;

   DataPair(const std::string &data, enum_field_types type)
      : type(type) {
      this->data = (char *)malloc(sizeof(char) * (data.size() + 1));
      strcpy(this->data, data.c_str());
   }

   ~DataPair() {
      free(this->data);
   }

   size_t length() const {
      return strlen(this->data);
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
         std::cerr << "Connection did not established successfully. ";
         return nullptr;
      }
      return connection;
   }



   void bindDataPair(MYSQL_BIND *bind, const DataPair &datapair) {

      bind->is_null = 0;
      bind->length = 0;

      if (datapair.type == MYSQL_TYPE_STRING) {
         bind->buffer_type = datapair.type;
         bind->buffer = datapair.data;
         bind->buffer_length = datapair.length();

      } else {
         double *data = (double *)malloc(sizeof(double));
         *data = std::stod(datapair.data);

         bind->buffer_type = datapair.type;
         bind->buffer = data;
         bind->buffer_length = sizeof(double);
      }
      return;
   }


   /**
    * @brief Fetch prepared statement result after execute
    *
    * @param stmt
    * @param metadata
    * @param is_null
    * @param type MYSQL_TYPE of data
    * @return char*
    */
   const char *FetchMySQLMetadataRow(MYSQL_STMT *stmt, bool is_null = 0, enum_field_types type = MYSQL_TYPE_STRING) {

      // Allocate memory for bind array
      MYSQL_BIND bind;
      char *row = new char[65];

      unsigned long length = 0;

      // Allocate buffer for each column (adjust size as needed)

      bind.buffer_type = type;
      bind.buffer = row;
      bind.buffer_length = 65;
      bind.length = &length;
      bind.is_null = (char *)&is_null;


      if (mysql_stmt_bind_result(stmt, &bind)) {
         std::cerr << "mysql_stmt_bind_result() failed" << mysql_stmt_error(stmt) << std::endl;
         return nullptr;
      }
      // Fetch and print the row
      if (!mysql_stmt_fetch(stmt)) {
         if (length > 0) {
            std::cout << row << std::endl;
            return row;
         } else {
            return "NULL";
         }
      }
      return nullptr;

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
   unsigned int preparedQueryExecute(MYSQL *connection, MYSQL_STMT *stmt, const std::string &preparedQuery, const std::vector<DataPair> &userData) {

      if (connection == nullptr) {
         std::cerr << "No connection to MYSQL server please try again later. " << std::endl;
         return 1;
      }

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
      int count = 0;
      for (size_t i = 0; i < preparedQuery.length(); i++) {
         if (preparedQuery[i] == '?')
            count++;
      }
      if (count != size) {
         std::cerr << "data miss match pram count" << std::endl;
         return 1;
      }

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
         unsigned int error = mysql_stmt_errno(stmt);
         mysql_stmt_close(stmt);
         return error;
      }

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
   unsigned int AddUser(MYSQL *connection, const std::array<std::string, 2> &in_UserData) {

      const char *insertQuery = "INSERT INTO Users VALUES(?, ?, ?)";//Prepared SQL query

      std::vector<DataPair> UserData;

      UserData.reserve(3);

      UserData.emplace_back(in_UserData[0], MYSQL_TYPE_STRING);
      UserData.emplace_back(in_UserData[1], MYSQL_TYPE_STRING);

      UserData.emplace_back("0", MYSQL_TYPE_DOUBLE);

      MYSQL_STMT *stmt = mysql_stmt_init(connection);

      if (unsigned int error = preparedQueryExecute(connection, stmt, insertQuery, UserData)) {
         std::cerr << error << std::endl;
         return error;
      }

      mysql_stmt_close(stmt);
      return 0;

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


      MYSQL_STMT *stmt = mysql_stmt_init(connection);
      if (preparedQueryExecute(connection, stmt, searchQuery, UserData)) {
         return false;
      }


      const char *row = FetchMySQLMetadataRow(stmt);
      while (strlen(row) != 0) {
         //check if user dont exist then return false
      }

      mysql_stmt_close(stmt);

      return true;

   }

}