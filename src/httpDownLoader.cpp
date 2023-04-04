#include "httpDownLoader.h"
#include "string.h"
//回调函数，将下载内容写入文件
size_t WriteToFileCB(char* ptr, size_t size, size_t nmemb, void* userdata) {
    CurlData* curlData = static_cast<CurlData*>(userdata);
    size_t dataSize = size * nmemb;
    for (size_t i = 0; i < dataSize; i++) {
        *curlData->writeBuffer++ = ptr[i];
    }
    curl_easy_setopt(curlData->curl, CURLOPT_WRITEDATA, static_cast<void*>(curlData));
    std::cout << "WriteToFileCB fileSize： " << dataSize << std::endl;
    return dataSize;
}

bool HttpDownLoader::DownLoadAndWriteToFile(char* fileData, long range_start, long range_end)
{
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cout << "init curl failed." << std::endl;
        return false;
    }

    CurlData CurlData;
    CurlData.curl = curl;
    CurlData.writeBuffer = fileData + range_start;

    //设置URL和回调函数
    curl_easy_setopt(curl, CURLOPT_URL, _fileUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToFileCB);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(&CurlData));

    //设置下载范围
    std::string range = std::to_string(range_start) + "-" + std::to_string(range_end);
    curl_easy_setopt(curl, CURLOPT_RANGE, range.c_str());

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cout << "Failed to download file: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    //清理libcurl
    curl_easy_cleanup(curl);
    
    return true;
}

size_t header_callback(char *buffer, size_t size, size_t nitems, long* length)
{
    const char* prefix = "Content-Length:";
    char* pos = strstr(buffer, prefix);
    if (pos) {
        sscanf(pos, "%*s %ld", length);
    }
    return size * nitems;
}

bool  HttpDownLoader::httpGetFileSize()
{
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cout << "init curl failed." << std::endl;
        return false;
    }

     //设置URL和回调函数
    curl_easy_setopt(curl, CURLOPT_URL, _fileUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_HEADER, 1);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &_filseSize);


    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cout << "Failed to get filesize: " << curl_easy_strerror(res) << std::endl;
        return false;
    }
    //清理libcurl
    curl_easy_cleanup(curl);
    
    return true;

}