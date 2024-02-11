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