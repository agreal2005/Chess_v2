#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// Function to check if a string starts with a given prefix
bool startsWith(const std::string& str, const std::string& prefix) {
    return str.compare(0, prefix.size(), prefix) == 0;
}

void processFile(const fs::path &inputFilePath, const fs::path &outputDir) {
    std::ifstream inputFile(inputFilePath);
    // Create the output file path in the specified output directory
    fs::path outputFilePath = outputDir / inputFilePath.filename();

    std::ofstream outputFile(outputFilePath); // Create a new filtered file

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error: Could not open " << inputFilePath << " for reading or creating filtered file.\n";
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        // Check if the line starts with "#"
        if (startsWith(line, "#")) {
            if (startsWith(line, "#+")) {
                line = "20000";  // Replace lines starting with "#+" with "20000"
            } else if (startsWith(line, "#-")) {
                line = "-20000"; // Replace lines starting with "#-" with "-20000"
            }
        }
        outputFile << line << "\n"; // Write the (possibly modified) line to the output file
    }

    inputFile.close();
    outputFile.close();
    std::cout << "Processed file: " << inputFilePath << " -> " << outputFilePath << "\n";
}

void processDirectory(const std::string &inputDir, const std::string &outputDir) {
    fs::path outputDirPath(outputDir);
    if (!fs::exists(outputDirPath)) {
        fs::create_directories(outputDirPath); // Create output directory if it doesn't exist
    }

    for (const auto &entry : fs::directory_iterator(inputDir)) {
        if (entry.is_regular_file()) {
            processFile(entry.path(), outputDirPath);
        }
    }
}

int main() {
    std::string inputDirectoryPath = "./evals";  // Set your input directory here
    std::string outputDirectoryPath = "./results"; // Set your output directory here

    processDirectory(inputDirectoryPath, outputDirectoryPath);

    return 0;
}
