// mysql_connector.h
#ifndef MYSQL_CONNECTOR_H
#define MYSQL_CONNECTOR_H

#include <mysql.h>
#include <iostream>
#include <string>

class MySQLConnector {
public:
    MySQLConnector(const std::string& host, const std::string& user, const std::string& password, const std::string& db);
    ~MySQLConnector();
    bool insertData(int temp, int hum, int status, int type);

private:
    MYSQL* conn;
};

#endif // MYSQL_CONNECTOR_H
