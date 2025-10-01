#pragma once

#include "Logger.h"
#include "Result.h"
#include "md5.h"
#include "csv_reader.h"
#include "ThreadPool.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>
#include <future>   
#include <iostream>


class Scanner {
public:
    Scanner(const std::string& baseFilePath, const std::string& logFilePath);
    void scanDirectory(const std::string& dirPath, Result& result);

private:
    void checkFile(const std::string& filePath, Result& result);

    std::unordered_map<std::string, std::string> hashMap;
    Logger log;
    std::mutex mutex_;
};
