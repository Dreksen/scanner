#include "logger.h"


Logger::Logger(const std::string& logFilePath)
        : logFile(logFilePath, std::ios::app)
{
    if (!logFile.is_open()) {
        throw std::runtime_error("Cannot open log file: " + logFilePath);
    }
}

Logger::~Logger() {
    logFile.close();
}

std::string Logger::timestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void Logger::info(const std::string& msg) {
    std::osyncstream(logFile) << "[" << timestamp() << "][INFO] " << msg << "\n";
}

void Logger::warn(const std::string& msg) {
    std::osyncstream(logFile) << "[" << timestamp() << "][WARN] " << msg << "\n";
}

void Logger::error(const std::string& msg) {
    std::osyncstream(logFile) << "[" << timestamp() << "][ERROR] " << msg << "\n"; 
}
