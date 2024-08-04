CREATE DATABASE IF NOT EXISTS amadoi CHARACTER SET utf8mb4;

USE amadoi;

CREATE TABLE IF NOT EXISTS t_sensors (
    id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    temp TINYINT NOT NULL,         -- temperature in Celsius
    hum TINYINT NOT NULL,          -- relative humidity in percentage
    status TINYINT NOT NULL,       -- data status, 0 for normal
    type TINYINT NOT NULL,         -- sensor type, 1 for DHT11, 2 for DHT22
    ctime TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- creation time
    utime TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, -- update time
    PRIMARY KEY (id)
);
