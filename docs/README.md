# amadoi

树莓派温湿度传感器数据采集程序，支持通过API上传传感器数据。

## 功能特性

- 支持DHT11/DHT22温湿度传感器
- 通过配置文件管理参数
- 通过HTTP API上传传感器数据
- 支持温度校准
- 可配置采样率

## 依赖安装

```bash
sudo apt update
sudo apt install -y libcurl4-openssl-dev libjsoncpp-dev libmariadb-dev wiringpi
```

## 编译

```bash
make
```

## 配置

程序使用 `amadoi.yml` 配置文件，示例配置：

```yaml
# API配置
api_url: "http://localhost:8080/habitat/raw/add"
device_code: "SENS-FARM01"

# 采样配置
sampling_rate: 5000

# 传感器配置
sensor_pin: 7
sensor_type: "dht11"

# 校准
temp_offset: -2
hum_offset: 0
```

## 运行

```bash
./amadoi
```

## API接口格式

上传数据格式：
```json
{
    "device_code": "SENS-FARM01",
    "temp": 24.5,
    "hum": 60.2
}
```

API响应格式：
```json
{
    "code": 0,
    "msg": "success",
    "data": {
        "created_at": "2025-09-21 14:28:08",
        "device_code": "SENS-FARM01",
        "id": 9
    }
}
```

## 安装为系统服务

```bash
sudo make install
sudo cp amadoi.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable amadoi
sudo systemctl start amadoi
```
