CREATE DATABASE IF NOT EXISTS amadoi CHARACTER SET utf8mb4;

USE amadoi;

CREATE TABLE t_constant (
    id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    param VARCHAR(255) NOT NULL,
    value VARCHAR(255),
    summary VARCHAR(255),
    PRIMARY KEY (id)
);

INSERT INTO t_constant (param, value, summary) 
VALUES ('dht11_rate', '5000', 'ms');

CREATE TABLE IF NOT EXISTS t_sensors (
    id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    temp TINYINT NOT NULL,         -- temperature in Celsius
    hum TINYINT NOT NULL,          -- relative humidity in percentage
    status TINYINT NOT NULL,       -- data status, 0 for normal 2 for error
    type TINYINT NOT NULL,         -- sensor type, 1 for DHT11, 2 for DHT22
    ctime TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- creation time
    utime TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, -- update time
    PRIMARY KEY (id)
);
