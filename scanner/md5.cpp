#include "md5.h"


std::string fileToMD5(const std::string& filename) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_md5(), nullptr);
    std::ifstream file(filename, std::ios::binary);
    std::streamsize fileSize = file.tellg();

    if (!file.is_open()) {
        throw std::runtime_error("Can not open a file");
    }

    size_t bufferSize;
    if (fileSize < 1024 * 1024) {               // < 1MB
        bufferSize = 64 * 1024;                 // 64KB
    } else if (fileSize < 100 * 1024 * 1024) {  // < 100MB
        bufferSize = 256 * 1024;                // 256KB
    } else {                                    // ≥ 100MB 
        bufferSize = 1024 * 1024;               // 1MB
    }

    std::vector<char> buffer(bufferSize); 
    while (file) {
        file.read(buffer.data(), buffer.size());
        std::streamsize s = file.gcount();
        if (s > 0) {
            EVP_DigestUpdate(ctx, buffer.data(), s);
        }
    }

    unsigned char md[EVP_MAX_MD_SIZE];
    unsigned int md_len;
    EVP_DigestFinal_ex(ctx, md, &md_len);
    EVP_MD_CTX_free(ctx);
    std::stringstream ss;
    for (unsigned int i = 0; i < md_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)md[i];
    }
    return ss.str();
}
