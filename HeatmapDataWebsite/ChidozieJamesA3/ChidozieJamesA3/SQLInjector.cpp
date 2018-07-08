

#include <MySQL\mysql_connection.h>
#include <MySQL\cppconn\driver.h>
#include <MySQL\cppconn\exception.h>
#include <MySQL\cppconn\resultset.h>
#include <MySQL\cppconn\statement.h>

#include <iostream>

#include "SQLInjector.h"


SQLInjector::SQLInjector() {

    con = NULL;
}

SQLInjector::~SQLInjector() {

    delete con;
}

// Establish a connection to the desired MySQL server
// Return true on success, false otherwise
bool SQLInjector::connect(std::string &ipString, std::string &user, std::string &password) {

    return connect(ipString.c_str(), user.c_str(), password.c_str());
}

bool SQLInjector::connect(const char *ipString, const char *user, const char *password) {

    sql::Driver *driver = NULL;

    try {
        
        driver = get_driver_instance();

        std::cout << "Connecting..." << std::endl;
        
        con = driver->connect(ipString, user, password);

        std::cout << "Connection successful!" << std::endl;
    }
    catch (sql::SQLException &e) {

        std::cout << "Error " << e.getErrorCode() << ": " << e.what() << std::endl;

        return false;
    }

    return true;
}

// Set the database that will be worked with
// Returns false if database can not be made
bool SQLInjector::setActiveDatabase(std::string &databaseName) {

    return setActiveDatabase(databaseName.c_str());
}

bool SQLInjector::setActiveDatabase(const char *databaseName) {

    if (con != NULL) {

        sql::SQLString sqlStr(databaseName);

        try {

            con->setSchema(sqlStr);

            database = databaseName;

            return true;
        }
        catch (sql::SQLException &e) {

            // Unknown database, need to create
            if (e.getErrorCode() == 1049) {
            
                std::cout << "Database \"" << databaseName << "\" does not exist. Creating..." << std::endl;

                statement(std::string("CREATE DATABASE " + std::string(databaseName)).c_str());

                con->setSchema(sqlStr);

                database = databaseName;

                return true;
            }
            else {

                std::cout << "Error " << e.getErrorCode() << ": " << e.what() << std::endl;
            }
        }
    }

    return false;
}

// Set the table that will be worked with
// Returns false if table does not exist
bool SQLInjector::setActiveTable(std::string &tableName) {

    return setActiveTable(tableName.c_str());
}

bool SQLInjector::setActiveTable(const char *tableName) {

    if (con != NULL) {

        // STRING IS INCORRECT, NEED SOME WAY TO DETECT IF TABLE EXISTS, IF NOT MAKE IT
        std::string str(
            "SELECT * FROM " + std::string(tableName) + ""
        );

        try {

            sql::Statement *statement = con->createStatement();
            sql::SQLString stateStr(str.c_str());

            statement->executeQuery(stateStr);
            delete statement;

            table = tableName;

            return true;
        }
        catch (sql::SQLException &e) {

            // Table doesn't exist, create table
            if (e.getErrorCode() == 1146) {

                std::cout << "Table '" << tableName << "' does not exist. Creating..." << std::endl;

                try {

                    str = "CREATE TABLE " + std::string(tableName) + " (x int, y int, time float)";

                    sql::Statement *statement = con->createStatement();
                    sql::SQLString stateStr(str.c_str());

                    statement->execute(stateStr);

                    delete statement;

                    table = tableName;

                    return true;
                }
                catch (sql::SQLException &e) {

                    std::cout << "Error " << e.getErrorCode() << ": " << e.what() << std::endl;
                }
            }
            else {

                std::cout << "Error " << e.getErrorCode() << ": " << e.what() << std::endl;
            }
        }
    }

    return false;
}

// Launch a statement into the current sql server
// Returns false on failure
bool SQLInjector::statement(std::string &statementStr) {

    return statement(statementStr.c_str());
}

bool SQLInjector::statement(const char *statementStr) {

    if (con != NULL) {

        try {

            sql::Statement *statement = con->createStatement();
            sql::SQLString stateStr(statementStr);

            statement->execute(stateStr);
            
            delete statement;
        }
        catch (sql::SQLException &e) {

            std::cout << "Error " << e.getErrorCode() << ": " << e.what() << std::endl;
        }
    }

    return false;
}

// Send a point to the sql server
void SQLInjector::sendPoint(float x, float y, float time) {

    std::string str = "INSERT INTO " + table + " VALUES (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(time) + ")";

    statement(str.c_str());
}
