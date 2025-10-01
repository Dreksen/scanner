#pragma once

#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <syncstream>

class Logger {
public:
    explicit Logger(const std::string& logFilePath);
    ~Logger();

    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);

private:
    std::ofstream logFile;
    static std::string timestamp();
};
