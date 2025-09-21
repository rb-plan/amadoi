# Makefile for amadoi - Modern C++ Project Structure
# Author: AI Assistant
# Description: Raspberry Pi temperature and humidity sensor with API upload

# Compiler and flags
CXX = c++
CXXFLAGS = -std=c++2a -Wall -Wextra -O2
INCLUDES = -Iinclude
LDFLAGS = -lcurl -ljsoncpp -lwiringPi

# Directories
SRC_DIR = src
BUILD_DIR = build
CONFIG_DIR = config
DOCS_DIR = docs

# Source files organized by module
API_SOURCES = $(SRC_DIR)/api/api_uploader.cc
CONFIG_SOURCES = $(SRC_DIR)/config/config_reader.cc
SENSOR_SOURCES = $(SRC_DIR)/sensors/dht11.cc $(SRC_DIR)/sensors/dht22.cc
MAIN_SOURCES = $(SRC_DIR)/main.cc

# All source files
SOURCES = $(API_SOURCES) $(CONFIG_SOURCES) $(SENSOR_SOURCES) $(MAIN_SOURCES)

# Object files (replace .cc with .o and add build directory prefix)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cc=$(BUILD_DIR)/%.o)

# Target executable
TARGET = $(BUILD_DIR)/amadoi
DIAGNOSTIC = sensor_diagnostic

# Default target
all: $(TARGET)

# Create target executable
$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	@echo "Linking $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build completed successfully!"

# Create diagnostic tool
$(DIAGNOSTIC): sensor_diagnostic.cc $(BUILD_DIR)/sensors/dht11.o $(BUILD_DIR)/config/config_reader.o | $(BUILD_DIR)
	@echo "Building diagnostic tool..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ sensor_diagnostic.cc $(BUILD_DIR)/sensors/dht11.o $(BUILD_DIR)/config/config_reader.o -lwiringPi
	@echo "Diagnostic tool built successfully!"

# Create build directory structure
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)/api
	@mkdir -p $(BUILD_DIR)/config
	@mkdir -p $(BUILD_DIR)/sensors

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR) $(DIAGNOSTIC)
	@echo "Clean completed!"

# Install the application
install: $(TARGET)
	@echo "Installing amadoi..."
	sudo mkdir -p /usr/local/amadoi
	sudo cp $(TARGET) /usr/local/amadoi/amadoi
	sudo chmod 755 /usr/local/amadoi/amadoi
	sudo mkdir -p /etc/amadoi
	sudo cp $(CONFIG_DIR)/amadoi.yml /etc/amadoi/
	@echo "Installation completed!"

# Install systemd service
install-service: install
	@echo "Installing systemd service..."
	sudo cp amadoi.service /etc/systemd/system/
	sudo systemctl daemon-reload
	sudo systemctl enable amadoi
	@echo "Service installed successfully!"

# Start the service
start:
	sudo systemctl start amadoi

# Stop the service
stop:
	sudo systemctl stop amadoi

# Show service status
status:
	systemctl status amadoi

# Run the application (for testing)
run: $(TARGET)
	@echo "Running amadoi..."
	cd $(BUILD_DIR) && ./amadoi $(if $(ARGS),$(ARGS),-c ../config/amadoi.yml)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: clean $(TARGET)

# Release build (default)
release: CXXFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all          - Build the application (default)"
	@echo "  clean        - Remove build artifacts"
	@echo "  install      - Install the application"
	@echo "  install-service - Install systemd service"
	@echo "  start        - Start the systemd service"
	@echo "  stop         - Stop the systemd service"
	@echo "  status       - Show service status"
	@echo "  run [ARGS]   - Run the application (for testing)"
	@echo "                 Example: make run ARGS=\"-c /etc/amadoi/config.yml\""
	@echo "  diagnostic   - Build and run sensor diagnostic tool"
	@echo "  debug        - Build debug version"
	@echo "  release      - Build optimized release version"
	@echo "  help         - Show this help message"

# Dependencies
$(BUILD_DIR)/api/api_uploader.o: include/api/api_uploader.h
$(BUILD_DIR)/config/config_reader.o: include/config/config_reader.h
$(BUILD_DIR)/sensors/dht11.o: include/sensors/dht11.h include/sensors/sensor.h
$(BUILD_DIR)/sensors/dht22.o: include/sensors/dht22.h include/sensors/sensor.h
$(BUILD_DIR)/main.o: include/sensors/dht11.h include/api/api_uploader.h include/config/config_reader.h

# Diagnostic tool
diagnostic: $(DIAGNOSTIC)
	@echo "Running sensor diagnostic tool..."
	sudo ./$(DIAGNOSTIC)

# Phony targets
.PHONY: all clean install install-service start stop status run debug release help diagnostic