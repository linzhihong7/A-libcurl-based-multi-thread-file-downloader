#ifndef HTTPDOWNLOADER_H
#define HTTPDOWNLOADER_H
#include "downloader.h"
#include <curl/curl.h>

struct CurlData {
    CURL* curl;
    char* writeBuffer;
};

class HttpDownLoader : public DownLoader<HttpDownLoader> {
public:
    HttpDownLoader(int threadNum, 
                   const std::string& downLoadFilePath,
                   const std::string& fileUrl)
    : DownLoader<HttpDownLoader>(threadNum,  downLoadFilePath), _fileUrl(fileUrl), _filseSize(0)
    {
        curl_global_init(CURL_GLOBAL_ALL);//初始化curl
        if (!httpGetFileSize()) {
            std::cout << "httpGetFileSize failed." << std::endl;
        }
    }

    ~HttpDownLoader() {
        curl_global_cleanup();
    }

    bool httpGetFileSize();

    long getFileSize() { return _filseSize; }

    bool DownLoadAndWriteToFile(char* fileData, long range_start, long range_end);
private:
    std::string _fileUrl;
    long _filseSize;
};


#endif