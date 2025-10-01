#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iostream>
#include <filesystem>
#include <string>

std::unordered_map<std::string, std::string> readCSV(const std::string& filename);
