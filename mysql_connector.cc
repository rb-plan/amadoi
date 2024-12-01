// mysql_connector.cc
#include "mysql_connector.h"
#include <mysql/mysql.h>
#include <iostream>

MySQLConnector::MySQLConnector(const std::string& host, const std::string& user, const std::string& password, const std::string& db) {
    conn = mysql_init(nullptr);
    if (conn == nullptr) {
        std::cerr << "mysql_init() failed" << std::endl;
        exit(1);
    }

    if (mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(), db.c_str(), 0, nullptr, 0) == nullptr) {
        std::cerr << "mysql_real_connect() failed" << std::endl;
        mysql_close(conn);
        exit(1);
    }
}

MySQLConnector::~MySQLConnector() {
    mysql_close(conn);
}

bool MySQLConnector::insertData(int temp, int hum, int status, int type) {
    std::string query = "INSERT INTO t_sensors (temp, hum, status, type, ctime, utime) VALUES (" +
                        std::to_string(temp) + ", " +
                        std::to_string(hum) + ", " +
                        std::to_string(status) + ", " +
                        std::to_string(type) + ", NOW(), NOW())";

    if (mysql_query(conn, query.c_str())) {
        std::cerr << "INSERT failed: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

int MySQLConnector::getConstant(const std::string& param, int defaultValue) {
    std::string query = "SELECT value FROM t_constant WHERE param = '" + param + "'";

    if (mysql_query(conn, query.c_str())) {
        std::cerr << "SELECT failed: " << mysql_error(conn) << std::endl;
        return defaultValue;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == nullptr) {
        std::cerr << "mysql_store_result() failed: " << mysql_error(conn) << std::endl;
        return defaultValue;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row && row[0]) {
        int value = std::stoi(row[0]);
        mysql_free_result(result);
        return value;
    }

    mysql_free_result(result);
    return defaultValue;
}
