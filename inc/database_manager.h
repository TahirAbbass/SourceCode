#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <iostream>
#include <sqlite3.h>

class DatabaseManager
{
private:
    sqlite3 *db;

public:
    DatabaseManager(const std::string &dbName);
    ~DatabaseManager();

    void createTable();
    void insertMessage(const std::string &message, const std::string& timestamp);
    
    //for testing
    bool tableExists(const std::string &tableName);
    bool doesMessageExist(const std::string &message);
};

#endif