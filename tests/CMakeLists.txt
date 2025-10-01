find_package(GTest REQUIRED)

add_executable(kasper_tests
    test_main.cpp
    test_scanner.cpp
)

target_link_libraries(kasper_tests
    scanner
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(kasper_tests
    EXTRA_ARGS "--gtest_output=xml:test_results.xml"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
