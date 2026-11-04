#ifndef UI_H
#define UI_H

#include "Database.h"
#include "Models.h"

/**
 * @class ConsoleUI
 * @brief Presentation Layer CLI interface handling user interactions for all four RBAC roles.
 */
class ConsoleUI {
private:
    Database db;  ///< Main SQLite database controller instance
public:
    ConsoleUI() : db("vertex_lms.db") {}

    /// Starts main application loop displaying login prompts.
    void run();

private:
    void handleStudentMenu(const User& user);
    void handleLecturerMenu(const User& user);
    void handleHodMenu(const User& user);
    void handleAdminMenu(const User& user);
};

#endif // UI_H
