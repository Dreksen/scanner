#pragma once

#include <openssl/evp.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

std::string fileToMD5(const std::string& filename);
