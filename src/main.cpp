#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <string_view>

#include "profiler.h"

using namespace std;

// Split strings by a specified delimiter (i.e. ",")
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    istringstream iss(str);
    string token;

    while (end != string::npos) {
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

void parseChunk(string_view chunk, Results& results, int columnIndex, bool needsParsing) {
    int currentColumn = 0;
    string currentField;

    for (char c : chunk) {
        if (c == ',') {
            if (needsParsing && currentColumn == columnIndex) {
                try {
                    results.totalSum += stod(currentField);
                    results.validDataCount++;
                } catch (...) {

                }
            }
            currentColumn++;
            currentField.clear();
        } else if (c == '\n') {
            if (needsParsing && currentColumn == columnIndex) {
                try {
                    results.totalSum += stod(currentField);
                    results.validDataCount++;
                } catch (...) {

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

Results processFile(ifstream& file, int columnIndex, string operation) {
    Timer processFileTimer("Processing File");
    Results results;
    bool needsParsing = (operation == "sum" || operation == "average");

    // Create 8mb buffer
    const size_t CHUNK_SIZE = 8 * 1024 * 1024;
    vector<char> buffer(CHUNK_SIZE);
    string remnant;

    while (file.read(buffer.data(), CHUNK_SIZE) || file.gcount() > 0) {
        size_t bytesRead = file.gcount();

        if (bytesRead == 0) continue;

        string_view dataView(buffer.data(), bytesRead);

        size_t firstNewLinePosition = dataView.find('\n');

        string bridgeLine = remnant + string(dataView.substr(0, firstNewLinePosition));

        parseChunk_String(bridgeLine, results, columnIndex, needsParsing);

        size_t lastNewLinePosition = dataView.find_last_of('\n');

        string_view middleChunk(
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
        cerr << "Incorrect number of arguments passed. Please try again.";
        return 1;
    }

    // Set arguments
    string filename = argv[1];
    string columnName = argv[2];
    int columnIndex = -1;
    string operation = argv[3];

    // Opening file
    ifstream file(filename, ios::binary);

    // Find index of chosen column header
    string columnHeaders;
    getline(file, columnHeaders);
    auto headers = split(columnHeaders, ',');
    for (int i = 0; i < headers.size(); i++) {
        if (headers[i] == columnName) {
            columnIndex = i;
            break;
        };
    }

    if (columnIndex == -1) {
        cerr << "Column '" << columnName << "' not found.";
        return 1;
    }

    Results fileResults = processFile(file, columnIndex, operation);

    cout << "Total Count: " << fileResults.totalCount << endl;
    cout << "Valid Count: " << fileResults.validDataCount << endl;
    cout << "Sum: " << fileResults.totalSum << endl;
    cout << "Average: " << (fileResults.totalSum / fileResults.validDataCount) << endl;
    file.close();
    return 0;
}