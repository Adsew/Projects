#pragma once


#include <string>


namespace sql {
    class Connection;
}


class SQLInjector {

private:

    sql::Connection *con;
    
public:

    // Names of your current sql service pieces
    std::string database;
    std::string table;

    SQLInjector();

    ~SQLInjector();

    // Establish a connection to the desired MySQL server
    // Return true on success, false otherwise
    bool connect(std::string &ipString, std::string &user, std::string &password);
    bool connect(const char *ipString, const char *user, const char *password);

    // Set the database that will be worked with
    // Returns false if database can not be made
    bool setActiveDatabase(std::string &databaseName);
    bool setActiveDatabase(const char *databaseName);

    // Set the table that will be worked with
    // Returns false if table does not exist
    bool setActiveTable(std::string &tableName);
    bool setActiveTable(const char *tableName);

    // Launch a statement into the current sql server
    // Returns false on failure
    bool statement(std::string &statementStr);
    bool statement(const char *statementStr);

    // Send a point to the sql server
	void sendPoint(float x, float y, float time);
};

