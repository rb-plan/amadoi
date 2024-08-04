// mysql_connector.cc
#include "mysql_connector.h"

MySQLConnector::MySQLConnector(const std::string& host, const std::string& user, const std::string& password, const std::string& db) {
    conn = mysql_init(nullptr);
    if (conn == nullptr) {
        std::cerr << "mysql_init() failed\n";
        exit(1);
    }

    if (mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(), db.c_str(), 0, nullptr, 0) == nullptr) {
        std::cerr << "mysql_real_connect() failed\n";
        mysql_close(conn);
        exit(1);
    }
}

MySQLConnector::~MySQLConnector() {
    if (conn != nullptr) {
        mysql_close(conn);
    }
}

bool MySQLConnector::insertData(int temp, int hum, int status, int type) {
    std::string query = "INSERT INTO t_sensors (temp, hum, status, type, ctime, utime) VALUES (" +
                        std::to_string(temp) + ", " + std::to_string(hum) + ", " + std::to_string(status) + 
                        ", " + std::to_string(type) + ", NOW(), NOW())";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "INSERT failed. Error: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}
