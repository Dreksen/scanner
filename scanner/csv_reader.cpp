#include "csv_reader.h"

namespace fs = std::filesystem;

std::unordered_map<std::string, std::string> readCSV(const std::string& filename) {
    std::unordered_map<std::string, std::string> map;
    std::ifstream file(filename);
    if (!fs::exists(filename)) {
        throw std::runtime_error("Base file " + filename + " does not exist");
    }
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open base file: " + filename);
    }

    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string hash, name;

        if (std::getline(ss, hash, ';') && std::getline(ss, name, ';')) {
            map.emplace(std::move(hash), std::move(name)); 
        }
    }

    return map;
}
