// mysql_connector.h
#ifndef MYSQL_CONNECTOR_H
#define MYSQL_CONNECTOR_H

#include <mariadb/mysql.h>
#include <iostream>
#include <string>

class MySQLConnector {
public:
    MySQLConnector(const std::string& host, const uint16_t& port, const std::string& user, const std::string& password, const std::string& db);
    ~MySQLConnector();
    bool insertData(int temp, int hum, int status, int type);
    int getConstant(const std::string& param, int defaultValue);

private:
    MYSQL* conn;
};

#endif // MYSQL_CONNECTOR_H
