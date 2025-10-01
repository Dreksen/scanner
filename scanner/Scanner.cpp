#include "Scanner.h"

namespace fs = std::filesystem;

Scanner::Scanner(const std::string& baseFilePath, const std::string& logFilePath)
        : log(logFilePath)
{
    hashMap = readCSV(baseFilePath);
}

void Scanner::checkFile(const std::string& filePath, Result& result) {
    std::string md5 = fileToMD5(filePath); // читаем блоками по 1 МБ
    result.filesCount++;

    if (hashMap.contains(md5)) {
        std::unique_lock<std::mutex> ulock(mutex_);
        log.warn(hashMap.at(md5) + " with hash " + md5 + " found in " + filePath);
        result.virusFilesCount++;
    }

}

void Scanner::scanDirectory(const std::string& dirPath, Result& result) {
    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        throw std::runtime_error("Directory does not exist: " + dirPath);
    }

    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::string> files;
    for (auto& entry : fs::recursive_directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().string());
        }
    }

    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;

    ThreadPool pool(numThreads);
    std::vector<std::future<void>> futures;

    for (const auto& filePath : files) {
        pool.enqueue([this, filePath, &result]() {
            try {
                this->checkFile(filePath, result);
            } catch (...) {
                result.errorCount++;
            }
        });
    }

    pool.waitAll();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    result.duration = duration;
}
