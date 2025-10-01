#include <gtest/gtest.h>
#include "../scanner/Scanner.h"
#include <filesystem>
#include <fstream>
#include <thread>

namespace fs = std::filesystem;

class ScannerTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::create_directories("test_dir");
        
        std::ofstream base("test_base.csv");
        base << "d41d8cd98f00b204e9800998ecf8427e;test_virus1\n";
        base << "5d41402abc4b2a76b9719d911017c592;test_virus2\n";
        base.close();
        
        std::ofstream clean("test_dir/clean_file.txt");
        clean << "This is a clean file";
        clean.close();
        
        std::ofstream virus("test_dir/virus_file.txt");
        virus << "hello";
        virus.close();
    }
    
    void TearDown() override {
        fs::remove_all("test_dir");
        fs::remove("test_base.csv");
        fs::remove("test_log.txt");
        if (fs::exists("empty_base.csv")) fs::remove("empty_base.csv");
        if (fs::exists("invalid_base.csv")) fs::remove("invalid_base.csv");
        if (fs::exists("large_file.bin")) fs::remove("large_file.bin");
    }
};

TEST_F(ScannerTest, ScannerCreation) {
    EXPECT_NO_THROW({
        Scanner scanner("test_base.csv", "test_log.txt");
    });
}

TEST_F(ScannerTest, ScanDirectory) {
    Scanner scanner("test_base.csv", "test_log.txt");
    Result result;
    
    scanner.scanDirectory("test_dir", result);
    
    EXPECT_EQ(result.filesCount, 2);
    EXPECT_EQ(result.virusFilesCount, 1);
}

TEST_F(ScannerTest, ScanNonExistentDirectory) {
    Scanner scanner("test_base.csv", "test_log.txt");
    Result result;
    
    EXPECT_THROW({
        scanner.scanDirectory("non_existent_dir", result);
    }, std::runtime_error);
}

TEST_F(ScannerTest, ErrorCounting) {
    Scanner scanner("test_base.csv", "test_log.txt");
    Result result;
    
    scanner.scanDirectory("test_dir", result);
    
    EXPECT_EQ(result.errorCount, 0);
}

TEST_F(ScannerTest, EmptyDirectory) {
    fs::create_directories("empty_test_dir");
    
    Scanner scanner("test_base.csv", "test_log.txt");
    Result result;
    
    scanner.scanDirectory("empty_test_dir", result);
    
    EXPECT_EQ(result.filesCount, 0);
    EXPECT_EQ(result.virusFilesCount, 0);
    EXPECT_EQ(result.errorCount, 0);
    
    fs::remove_all("empty_test_dir");
}

TEST_F(ScannerTest, NonExistentBaseFile) {
    EXPECT_THROW({
        Scanner scanner("non_existent_base.csv", "test_log.txt");
    }, std::runtime_error);
}

TEST_F(ScannerTest, EmptyBaseFile) {
    std::ofstream empty("empty_base.csv");
    empty.close();
    
    EXPECT_NO_THROW({
        Scanner scanner("empty_base.csv", "test_log.txt");
    });
}

TEST_F(ScannerTest, InvalidBaseFileFormat) {
    std::ofstream invalid("invalid_base.csv");
    invalid << "invalid_format_no_separator\n";
    invalid << "hash1;description1;extra_field\n";
    invalid.close();
    
    EXPECT_NO_THROW({
        Scanner scanner("invalid_base.csv", "test_log.txt");
    });
}

TEST_F(ScannerTest, LargeNumberOfFiles) {
    for (int i = 0; i < 100; i++) {
        std::ofstream file("test_dir/file" + std::to_string(i) + ".txt");
        file << "content" << i;
        file.close();
    }
    
    Scanner scanner("test_base.csv", "test_log.txt");
    Result result;
    
    scanner.scanDirectory("test_dir", result);
    
    EXPECT_EQ(result.filesCount, 102);
    EXPECT_EQ(result.virusFilesCount, 1);
}

TEST_F(ScannerTest, NestedDirectories) {
    fs::create_directories("test_dir/subdir1/subdir2");
    
    std::ofstream file1("test_dir/subdir1/file1.txt");
    file1 << "file1 content";
    file1.close();
    
    std::ofstream file2("test_dir/subdir1/subdir2/file2.txt");
    file2 << "file2 content";
    file2.close();
    
    Scanner scanner("test_base.csv", "test_log.txt");
    Result result;
    
    scanner.scanDirectory("test_dir", result);
    
    EXPECT_EQ(result.filesCount, 4);
}

TEST_F(ScannerTest, SpecialCharactersInFilenames) {
    std::ofstream file1("test_dir/file with spaces.txt");
    file1 << "content";
    file1.close();
    
    std::ofstream file2("test_dir/file-with-dashes.txt");
    file2 << "content";
    file2.close();
    
    Scanner scanner("test_base.csv", "test_log.txt");
    Result result;
    
    scanner.scanDirectory("test_dir", result);
    
    EXPECT_EQ(result.filesCount, 4);
}


TEST_F(ScannerTest, MultipleVirusMatches) {
    std::ofstream virus1("test_dir/virus1.txt");
    virus1 << "hello";
    virus1.close();
    
    std::ofstream virus2("test_dir/virus2.txt");
    virus2.close();
    
    Scanner scanner("test_base.csv", "test_log.txt");
    Result result;
    
    scanner.scanDirectory("test_dir", result);
    
    EXPECT_EQ(result.virusFilesCount, 3);
}

TEST_F(ScannerTest, VeryLongFileName) {
    std::string longName(200, 'a');
    std::ofstream file("test_dir/" + longName + ".txt");
    file << "content";
    file.close();
    
    Scanner scanner("test_base.csv", "test_log.txt");
    Result result;
    
    scanner.scanDirectory("test_dir", result);
    
    EXPECT_EQ(result.filesCount, 3);
}

TEST_F(ScannerTest, BinaryFiles) {
    std::ofstream binary("test_dir/binary.bin", std::ios::binary);
    for (int i = 0; i < 1000; i++) {
        binary.put(static_cast<char>(i % 256));
    }
    binary.close();
    
    Scanner scanner("test_base.csv", "test_log.txt");
    Result result;
    
    scanner.scanDirectory("test_dir", result);
    
    EXPECT_EQ(result.filesCount, 3);
}

TEST_F(ScannerTest, EmptyFiles) {
    std::ofstream empty1("test_dir/empty1.txt");
    empty1.close();
    
    std::ofstream empty2("test_dir/empty2.txt");
    empty2.close();
    
    Scanner scanner("test_base.csv", "test_log.txt");
    Result result;
    
    scanner.scanDirectory("test_dir", result);
    
    EXPECT_EQ(result.filesCount, 4);
    EXPECT_EQ(result.virusFilesCount, 3);
}