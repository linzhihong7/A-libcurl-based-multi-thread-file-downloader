#include <iostream>
#include "httpDownLoader.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "./download_tool threadNum downloadUrl downloadPathAndFilename" << std::endl;
        return 1;
    }

    uint32_t threadNum = std::stoul(argv[1]);
    std::string downloadUrl(argv[2]);
    std::string downloadPathAndFilename(argv[3]);
    HttpDownLoader httpDownLoader(threadNum, downloadPathAndFilename, downloadUrl);
    httpDownLoader.start();
    return 0;
}