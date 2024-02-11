#include "../inc/database_manager.h"

DatabaseManager::DatabaseManager(const std::string& dbName) {
    int rc = sqlite3_open(dbName.c_str(), &db);
    if (rc) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        exit(EXIT_FAILURE);
    }
}

DatabaseManager::~DatabaseManager() {
    sqlite3_close(db);
}

void DatabaseManager::createTable() {
    const char* query = "CREATE TABLE IF NOT EXISTS chat_messages (id INTEGER PRIMARY KEY AUTOINCREMENT, message TEXT, timestamp TEXT);";
    
    char* errMsg;
    int rc = sqlite3_exec(db, query, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void DatabaseManager::insertMessage(const std::string& message, const std::string& timestamp) {
    //std::string timestamp = "2024-02-11 12:34:56";  // Replace with your timestamp logic
    
    std::string query = "INSERT INTO chat_messages (message, timestamp) VALUES ('" + message + "', '" + timestamp + "');";
    
    char* errMsg;
    int rc = sqlite3_exec(db, query.c_str(), 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Error inserting message: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

bool DatabaseManager::tableExists(const std::string &tableName)
{
    std::string query = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + tableName + "';";
    sqlite3_stmt *statement;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr) == SQLITE_OK)
    {
        int result = sqlite3_step(statement);
        sqlite3_finalize(statement);

        return result == SQLITE_ROW;
    }

    return false;
}

bool DatabaseManager::doesMessageExist(const std::string &message)
{
    std::string query = "SELECT COUNT(*) FROM chat_messages WHERE message = '" + message + "';";
    char *errMsg;

    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Error executing query: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true; // If the query is executed successfully, the message exists
}