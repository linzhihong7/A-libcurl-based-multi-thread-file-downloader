#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <fstream>
#include "util.h"
#include "noncopyable.h"


template<class SpecificDownLoader>
class DownLoader : public noncopyable {
public:
    DownLoader(int thread_num, const std::string& downLoadFilePath)
    :  _thead_num(thread_num), _downLoadFilePath(downLoadFilePath) {
    }


    void threadFunc(int id, char* fileData, long range_start, long range_end) {
        bool res = static_cast<SpecificDownLoader*>(this)->DownLoadAndWriteToFile(fileData, range_start, range_end);
        if (!res) {
            std::cout << "thread id: %d download range_start: " << range_start << ", range end: " << range_end << " failed" << std::endl;
        }
        std::cout << id << "range_start: " << range_start << ", range_end: " << range_end << std::endl;
    }

    /* use file to save part file */ 
    // bool start() {
    //     if (!createPartFile()) {
    //         std::cout << "createPartFile failed." << std::endl;
    //         return false;
    //     }
    //     std::vector<std::thread> _threads;
    //     long fileSize = static_cast<SpecificDownLoader*>(this)->getFileSize();
    //     long range_start = 0;
    //     long range_end = 0;
    //     for (int i = 0; i < _thead_num; i++) {
    //         range_start = i * (fileSize/(double)_thead_num);
    //         if (i == _thead_num - 1) {
    //             range_end = fileSize - 1;
    //         }
    //         else {
    //             range_end = (i+1) * (fileSize/(double)_thead_num) - 1;
    //         }
    //         _threads.emplace_back(&DownLoader::threadFunc, this, i, range_start, range_end);
    //     }
    //     for (int i = 0; i < _thead_num; i++) {
    //         _threads[i].join();
    //     }

    //     // combineFile();
    //     std::cout << "file: " << _downLoadFilePath << "download success." << std::endl;
    //     return true;
    // }

    // void combineFile() {
    //     std::ofstream outfile(_downLoadFilePath, std::ios::binary | std::ios_base::out);
    //     for (int i = 0; i < _thead_num; i++) {
    //         std::ifstream infile(_partFiles[i], std::ios::binary);
    //         outfile << infile.rdbuf();
    //         infile.close();
    //         std::cout << _partFiles[i] << std::endl;
    //         std::remove(_partFiles[i].c_str());
    //     }
    //     outfile.close();
    // }

    // bool createPartFile() {
    //     for (int i = 0; i < _thead_num; i++) {
    //         std::string partFileName = _downLoadFilePath + "_part_" + std::to_string(i);
    //         std::ofstream file(partFileName);
    //         if (file.is_open()) {
    //             file.close();
    //             _partFiles.push_back(partFileName);
    //         }
    //         else {
    //             std::cout << "failed to create part file" << std::endl;
    //             return false;
    //         }
    //     }
    //       return true;
    // }

    bool start() {
        long fileSize = static_cast<SpecificDownLoader*>(this)->getFileSize();
        char* fileData = util::mmapFileToMem(_downLoadFilePath, fileSize);
        if (fileData == nullptr) {
            std::cout << "mmapFileToMem file " << _downLoadFilePath << " failed." << std::endl;
            return false;
        }
        std::vector<std::thread> _threads;
        long range_start = 0;
        long range_end = 0;
        _thead_num = 1;
        for (int i = 0; i < _thead_num; i++) {
            range_start = i * (fileSize/(double)_thead_num);
            if (i == _thead_num - 1) {
                range_end = fileSize - 1;
            }
            else {
                range_end = (i+1) * (fileSize/(double)_thead_num) - 1;
            }
            _threads.emplace_back(&DownLoader::threadFunc, this, i, fileData, range_start, range_end);
        }

        for (int i = 0; i < _thead_num; i++) {
            _threads[i].join();
        }

        if (!util::mumapFileFromMem(fileData, fileSize)) {
            std::cout << "mumapFileFromMem file " << _downLoadFilePath << " failed." << std::endl;
            return false;
        }

        std::cout << "file: " << _downLoadFilePath << "download success." << std::endl;
        return true;
    }


protected:
    int _thead_num;
    std::string _downLoadFilePath;
    std::vector<std::string> _partFiles;
};

#endif