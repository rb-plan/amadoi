// api_uploader.h
#ifndef API_UPLOADER_H
#define API_UPLOADER_H

#include <string>
#include <curl/curl.h>

struct ApiResponse {
    int code;
    std::string msg;
    std::string data;
};

class ApiUploader {
public:
    ApiUploader(const std::string& apiUrl, const std::string& deviceCode);
    ~ApiUploader();
    bool uploadSensorData(int temp, int hum, int status);
    ApiResponse getLastResponse() const;

private:
    std::string apiUrl;
    std::string deviceCode;
    CURL* curl;
    ApiResponse lastResponse;
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    std::string createJsonPayload(int temp, int hum) const;
};

#endif // API_UPLOADER_H
