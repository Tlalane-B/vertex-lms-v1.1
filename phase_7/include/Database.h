#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>

/**
 * @class Database
 * @brief Manages low-level SQLite database connection lifecycle and schema initialization.
 */
class Database {
private:
    sqlite3* db;            ///< Raw pointer to active SQLite database connection handle
    std::string dbPath;     ///< Filepath to the SQLite database file (.db)

public:
    explicit Database(const std::string& path = "vertex_lms.db");
    ~Database();

    bool open();
    void close();
    sqlite3* getConnection() const;
    bool execute(const std::string& sql);
    bool initializeSchema();
};

#endif // DATABASE_H
