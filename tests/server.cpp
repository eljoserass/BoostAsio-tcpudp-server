#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <gtest/gtest.h>

bool validateLogLine(const std::string &line, const std::regex pattern) {
    return std::regex_match(line, pattern);
}

int validateLogFile(const std::string &filename) {
    const std::regex pattern("Average bytes per second: (\\d+) \\| Number of messages sent: (\\d+) \\| Total of bytes sent: (\\d+)");
    int errors = 0;
    std::ifstream file(filename);
    std::string line;
    int line_number = 0;

    if (!file) {
        std::cout << "Error opening file\n";
        errors++;
    }
    while (std::getline(file, line)) {
        line_number++;
        if (!validateLogLine(line, pattern)) {
            std::cout << "Invalid log file line: " << line_number << " - " << line << std::endl;
            errors++;
        }
    }
    file.close();
    return errors;
}

TEST(LogValidationTest, ValidateLogFile) {
    std::string filename = "messages.log";
    int numErrors = validateLogFile(filename);

    EXPECT_EQ(numErrors, 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
