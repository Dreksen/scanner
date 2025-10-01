#include "scanner/Scanner.h"

#include <iostream>
#include <chrono>


int main(int argc, char *argv[]) {
    std::string baseFile, logFile, folderPath;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--base" && i + 1 < argc) {
            baseFile = argv[++i];
        } else if (arg == "--log" && i + 1 < argc) {
            logFile = argv[++i];
        } else if (arg == "--path" && i + 1 < argc) {
            folderPath = argv[++i];
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    }

    if (baseFile.empty() || logFile.empty() || folderPath.empty()) {
        std::cerr << "Usage: --base <file> --log <file> --path <directory>" << std::endl;
        return 1;
    }


    Result result;
    try {
        Scanner scanner(baseFile, logFile);
        scanner.scanDirectory(folderPath, result);

        std::cout << "Total checked files: " << result.filesCount << "\n";
        std::cout << "Virus files: " << result.virusFilesCount << "\n";
        std::cout << "Could not check: " << result.errorCount << "\n";
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "Execution time: " << result.duration.count() << " ms\n";

    return 0;
}
