#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <string_view>
#include <charconv>

#include "profiler.h"

// Split strings by a specified delimiter (i.e. ",")
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    std::istringstream iss(str);
    std::string token;

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    
    tokens.push_back(str.substr(start));
    return tokens;
}

struct Results {
    long totalCount = 0;
    long validDataCount = 0;
    double totalSum = 0;
};

void parseChunk(std::string_view chunk, Results& results, int columnIndex, bool needsParsing) {
    int currentColumn = 0;
    std::string currentField;

    for (char c : chunk) {
        // If new column, & column matches selection, add data
        if (c == ',') {
            if (needsParsing && currentColumn == columnIndex) {
                double value;
                // Parse character in currentField as double, storing result in value
                auto [ptr, ec] = std::from_chars(currentField.data(), currentField.data() + currentField.size(), value);

                // If there is no error code, then add results
                if (ec == std::errc()) {
                    results.totalSum += value;
                    results.validDataCount++;                    
                }

                // try {
                //     results.totalSum += stod(currentField);
                //     results.validDataCount++;
                // } catch (...) {

                // }
            }
            currentColumn++;
            currentField.clear();
        } else if (c == '\n') {
            // If end of row & column matches selection, add data (because final column may be missed with just ',' check)
            if (needsParsing && currentColumn == columnIndex) {
                double value;
                auto [ptr, ec] = std::from_chars(currentField.data(), currentField.data() + currentField.size(), value);

                if (ec == std::errc()) {
                    results.totalSum += value;
                    results.validDataCount++;                    
                }
            }
            results.totalCount++;
            currentField.clear();
            currentColumn = 0;
        } else {
            if (needsParsing && currentColumn == columnIndex) {
                currentField += c;
            }
        }
    }
}

void parseChunk_String(const std::string& chunk, Results& results, int columnIndex, bool needsParsing) {
    parseChunk(chunk, results, columnIndex, needsParsing);
}

Results processFile(std::ifstream& file, int columnIndex, std::string operation) {
    Timer processFileTimer("Processing File"); // Start new timer to measure length of file processing
    Results results;
    bool needsParsing = (operation == "sum" || operation == "average");

    // Create 8mb buffer
    const size_t CHUNK_SIZE = 8 * 1024 * 1024;
    std::vector<char> buffer(CHUNK_SIZE);
    std::string remnant;

    // While there is data to read, create and put it into an 8mb chunk
    while (file.read(buffer.data(), CHUNK_SIZE) || file.gcount() > 0) {
        size_t bytesRead = file.gcount();

        // If no data, skip
        if (bytesRead == 0) continue;

        // Create a reference to the buffer data
        std::string_view dataView(buffer.data(), bytesRead);


        size_t firstNewLinePosition = dataView.find('\n');
        std::string bridgeLine = remnant + std::string(dataView.substr(0, firstNewLinePosition));
        parseChunk_String(bridgeLine, results, columnIndex, needsParsing);
        size_t lastNewLinePosition = dataView.find_last_of('\n');

        std::string_view middleChunk(
            buffer.data() + firstNewLinePosition + 1,
            lastNewLinePosition - (firstNewLinePosition + 1)
        );

        parseChunk(middleChunk, results, columnIndex, needsParsing);

        remnant = dataView.substr(lastNewLinePosition + 1);
    };

    // Check if any remaining remnant to parse after looping
    if (!remnant.empty()) {
        remnant += '\n';
        parseChunk(remnant, results, columnIndex, needsParsing);
    }

    return results;
};

// Original in case of emergency!!
// Results processFile(ifstream& file, int columnIndex, const string& operation) {
//     Results results;
//     bool needsParsing = (operation == "sum" || operation == "average");

//     string fileDataLine;

//     while (getline (file, fileDataLine)) {
//         results.totalCount++;

//         if (!needsParsing) {
//             continue;
//         }

//         stringstream lineStream(fileDataLine);
//         string dataChunk;
//         int currentColumn = 0;


//         while (getline(lineStream, dataChunk, ',')) {
//             if (currentColumn == columnIndex) {
//                 try {
//                     results.totalSum += stod(dataChunk);
//                     results.validDataCount++;
//                 }
//                 catch (const invalid_argument& e) {
//                 }
//                 catch (out_of_range& e) {
//                 }
//                 break;
//             }
//             currentColumn++;
//         }
//     }
//     return results;
// };

int main(int argc, char** argv) {
    // Read args
    // In format [program] [filename.csv] [column name] [operation]
    Timer timer("Entire Program"); // Start profiling Timer
    if (argc != 4) {
        std::cerr << "Incorrect number of arguments passed. Please try again.";
        return 1;
    }

    // Set arguments
    std::string filename = argv[1];
    std::string columnName = argv[2];
    int columnIndex = -1;
    std::string operation = argv[3];

    // Opening file
    std::ifstream file(filename, std::ios::binary);

    // Find index of chosen column header
    std::string columnHeaders;
    std::getline(file, columnHeaders);
    auto headers = split(columnHeaders, ',');
    for (int i = 0; i < headers.size(); i++) {
        if (headers[i] == columnName) {
            columnIndex = i;
            break;
        };
    }

    if (columnIndex == -1) {
        std::cerr << "Column '" << columnName << "' not found.";
        return 1;
    }

    Results fileResults = processFile(file, columnIndex, operation);

    std::cout << "Total Count: " << fileResults.totalCount << "\n";
    std::cout << "Valid Count: " << fileResults.validDataCount << "\n";
    std::cout << "Sum: " << fileResults.totalSum << "\n";
    std::cout << "Average: " << (fileResults.totalSum / fileResults.validDataCount) << std::endl;
    file.close();
    return 0;
}