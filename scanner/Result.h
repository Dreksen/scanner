#pragma once

#include <atomic>
#include <chrono>

struct Result {
    std::atomic<int> filesCount{0}, virusFilesCount{0}, errorCount{0};
    std::chrono::milliseconds duration;
};
