// api_uploader.cc
#include "api/api_uploader.h"
#include <iostream>
#include <sstream>
#include <memory>
#include <jsoncpp/json/json.h>

ApiUploader::ApiUploader(const std::string& apiUrl, const std::string& deviceCode) 
    : apiUrl(apiUrl), deviceCode(deviceCode) {
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        exit(1);
    }
}

ApiUploader::~ApiUploader() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

std::string ApiUploader::createJsonPayload(int temp, int hum) const {
    Json::Value root;
    root["device_code"] = deviceCode;
    root["temp"] = temp;
    root["hum"] = hum;
    
    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ostringstream os;
    writer->write(root, &os);
    return os.str();
}

size_t ApiUploader::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool ApiUploader::uploadSensorData(int temp, int hum, int status) {
    if (status != 0) {
        // Skip upload if sensor data is not valid
        std::cout << "Skipping upload due to invalid sensor data (status: " << status << ")" << std::endl;
        return false;
    }
    
    std::string jsonPayload = createJsonPayload(temp, hum);
    std::string responseString;
    
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    
    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    
    if (res != CURLE_OK) {
        std::cerr << "CURL failed: " << curl_easy_strerror(res) << std::endl;
        lastResponse.code = -1;
        lastResponse.msg = "CURL error";
        lastResponse.data = "";
        return false;
    }
    
    // Parse JSON response
    Json::Value responseJson;
    Json::Reader reader;
    if (reader.parse(responseString, responseJson)) {
        lastResponse.code = responseJson.get("code", -1).asInt();
        lastResponse.msg = responseJson.get("msg", "").asString();
        lastResponse.data = responseJson.get("data", Json::Value()).toStyledString();
        
        if (lastResponse.code == 0) {
            std::cout << "Data uploaded successfully. Response: " << responseString << std::endl;
            return true;
        } else {
            std::cerr << "API returned error: " << lastResponse.msg << std::endl;
            return false;
        }
    } else {
        std::cerr << "Failed to parse API response: " << responseString << std::endl;
        lastResponse.code = -1;
        lastResponse.msg = "JSON parse error";
        lastResponse.data = "";
        return false;
    }
}

ApiResponse ApiUploader::getLastResponse() const {
    return lastResponse;
}
