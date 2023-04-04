#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <fstream>
#include <openssl/evp.h>
#include <sstream>
#include <iomanip>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

namespace util {
    bool getFileMd5(const std::string& fileName, std::string& hash);

    char* mmapFileToMem(const std::string& filename, long fileSize);

    bool mumapFileFromMem(char* data, long fileSize);

} // namespace util

#endif
