#include "util.h"

namespace util {
    bool getFileMd5(const std::string& fileName, std::string& hash) {
        std::ifstream infile(fileName.c_str(), std::ios::binary | std::ios::in);

        if (!infile.is_open()) {
            std::cout << "Error: could not open file " << fileName << std::endl;
            return false;
        }

        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        if (!ctx) {
            std::cerr << "Error: EVP_MD_CTX_new failed" << std::endl;
            return false;
        }

        if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1) {
            std::cerr << "Error: EVP_DigestInit_ex failed. " << std::endl;
            EVP_MD_CTX_free(ctx);
            return false;
        }

        char buffer[20];
        while (infile.read(buffer, sizeof(buffer)), infile.gcount() > 0) {
            if (EVP_DigestUpdate(ctx, buffer, infile.gcount()) != 1) {
                std::cerr << "Error EVP_DigestUpdate" << std::endl;
                EVP_MD_CTX_free(ctx);
                return false;
            }
        }


        //Finalize the digest and get the MD5 hash
        unsigned char digest[EVP_MAX_MD_SIZE];
        unsigned int digest_length = 0;
        if (EVP_DigestFinal_ex(ctx, digest, &digest_length) != 1) {
            std::cerr << "Error EVP_DigestFinal_ex failed" << std::endl;
            EVP_MD_CTX_free(ctx);
            return 1;
        }

        //Convert the digest to a hex string
        std::ostringstream ss;
        for (unsigned int i = 0; i < digest_length; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(digest[i]);
        }
        hash = ss.str();
        return true;
    }

    char* mmapFileToMem(const std::string& filename, long fileSize) {
        int fd = open(filename.c_str(), O_RDWR | O_CREAT);
        if (fd == -1) {
            std::cout << "mmapFileToMemory: open file " << filename << " failed." << std::endl;
            return nullptr;
        }
        lseek(fd, fileSize - 1, SEEK_END);
        write(fd, "", 1);

        void* data = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) {
            std::cout << "file " << filename << " mmap failed." << std::endl;
            return nullptr;
        }
        close(fd);
        return static_cast<char*>(data);
    }

    bool mumapFileFromMem(char* data, long fileSize) {
         //同步到磁盘
        if (msync(data, fileSize, MS_SYNC) == -1) {
            std::cout << "file  msync failed." << std::endl;
            return false;
        }

        //卸载映射
        if (munmap(data, fileSize) == - 1) {
            std::cout << "file  munmap failed." << std::endl;
            return false;
        }

        return true;
    }
}