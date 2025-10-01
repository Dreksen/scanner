add_library(scanner SHARED
        Scanner.cpp
        md5.cpp
        logger.cpp
        csv_reader.cpp
        ThreadPool.cpp
)

find_package(OpenSSL REQUIRED)
target_link_libraries(scanner OpenSSL::SSL OpenSSL::Crypto)

# чтобы в main можно было писать #include "scanner/scanDir.h"
target_include_directories(scanner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(scanner)