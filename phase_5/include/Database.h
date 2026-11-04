#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>

class Database {
private:
    sqlite3* db;
    std::string dbPath;

public:
    explicit Database(const std::string& path = "vertex_lms.db");
    ~Database();

    bool open();
    void close();
    sqlite3* getConnection() const;
    bool execute(const std::string& sql);
    bool initializeSchema();
    bool backupDatabase(const std::string& backupPath);
};

#endif // DATABASE_H
