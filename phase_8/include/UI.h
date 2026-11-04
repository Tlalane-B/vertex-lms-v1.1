#ifndef UI_H
#define UI_H

#include "Database.h"
#include "Models.h"

class ConsoleUI {
private:
    Database db;
public:
    ConsoleUI() : db("vertex_lms.db") {}

    void run();

private:
    void handleStudentMenu(const User& user);
    void handleLecturerMenu(const User& user);
    void handleHodMenu(const User& user);
    void handleAdminMenu(const User& user);
};

#endif // UI_H
