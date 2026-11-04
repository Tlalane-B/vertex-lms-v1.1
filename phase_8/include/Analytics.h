#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "Database.h"
#include <string>

class AnalyticsModule {
private:
    Database& db;
public:
    explicit AnalyticsModule(Database& database) : db(database) {}

    void generateSystemReport() const;
    bool exportAuditTrailCSV(const std::string& outputPath) const;
};

#endif // ANALYTICS_H
