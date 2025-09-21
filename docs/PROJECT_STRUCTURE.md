# Amadoi 项目结构

## 目录结构

```
amadoi/
├── build/                    # 编译输出目录
│   ├── api/
│   ├── config/
│   ├── sensors/
│   └── amadoi               # 可执行文件
├── config/                   # 配置文件
│   ├── amadoi.yml           # 主配置文件
│   └── databases.sql        # 数据库脚本
├── docs/                     # 文档目录
│   ├── README.md
│   ├── LICENSE
│   └── PROJECT_STRUCTURE.md # 本文档
├── include/                  # 头文件目录
│   ├── api/
│   │   └── api_uploader.h
│   ├── config/
│   │   └── config_reader.h
│   └── sensors/
│       ├── dht11.h
│       ├── dht22.h
│       └── sensor.h
├── src/                      # 源代码目录
│   ├── api/
│   │   └── api_uploader.cc
│   ├── config/
│   │   └── config_reader.cc
│   ├── sensors/
│   │   ├── dht11.cc
│   │   └── dht22.cc
│   └── main.cc              # 主程序
├── .git/                     # Git版本控制
├── .gitignore               # Git忽略文件
├── Makefile                 # 构建配置
└── amadoi.service           # 系统服务配置
```

## 模块说明

### API模块 (`src/api/`, `include/api/`)
- **api_uploader.h/cc**: HTTP API上传器
  - 负责将传感器数据上传到远程API
  - 支持JSON格式数据
  - 包含错误处理和重试机制

### 配置模块 (`src/config/`, `include/config/`)
- **config_reader.h/cc**: 配置文件读取器
  - 支持YAML格式配置文件
  - 提供类型安全的配置值获取
  - 支持默认值和错误处理

### 传感器模块 (`src/sensors/`, `include/sensors/`)
- **sensor.h**: 传感器基类接口
- **dht11.h/cc**: DHT11温湿度传感器实现
- **dht22.h/cc**: DHT22温湿度传感器实现

### 主程序 (`src/main.cc`)
- 程序入口点
- 初始化各个模块
- 主循环逻辑

## 构建系统

### Makefile 目标

- `make` 或 `make all`: 编译项目
- `make clean`: 清理编译文件
- `make install`: 安装到系统
- `make install-service`: 安装系统服务
- `make run`: 运行程序（测试用）
- `make debug`: 编译调试版本
- `make release`: 编译优化版本

### 编译选项

- **编译器**: C++2a标准
- **优化**: -O2 (默认), -O3 (release), -g (debug)
- **警告**: -Wall -Wextra
- **包含路径**: -Iinclude
- **链接库**: libcurl, jsoncpp, wiringPi

## 配置文件

### amadoi.yml 配置项

```yaml
# API配置
api_url: "http://10.20.0.26:8000/habitat/raw/add"
device_code: "SENS-FARM01"

# 采样配置
sampling_rate: 5000

# 传感器配置
sensor_pin: 7
sensor_type: "dht11"

# 校准参数
temp_offset: -2
hum_offset: 0
```

## 系统服务

使用systemd管理服务：

```bash
sudo make install-service  # 安装服务
sudo systemctl start amadoi    # 启动服务
sudo systemctl enable amadoi   # 开机自启
sudo systemctl status amadoi   # 查看状态
```

## 开发指南

### 添加新传感器

1. 在 `include/sensors/` 创建头文件
2. 在 `src/sensors/` 创建实现文件
3. 继承 `sensor.h` 接口
4. 更新Makefile依赖

### 添加新配置项

1. 在 `config/amadoi.yml` 添加配置项
2. 在 `config_reader.cc` 中使用相应方法获取值

### 代码规范

- 使用C++2a标准
- 头文件保护宏使用 `#ifndef/#define/#endif`
- 类名使用PascalCase，方法名使用camelCase
- 文件名使用snake_case
- 添加适当的注释和文档
