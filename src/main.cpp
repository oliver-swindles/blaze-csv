#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

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

Results processFile(ifstream& file, int columnIndex, const string& operation) {
    Results results;
    bool needsParsing = (operation == "sum" || operation == "average");

    string fileDataLine;

    while (getline (file, fileDataLine)) {
        results.totalCount++;

        if (!needsParsing) {
            continue;
        }

        stringstream lineStream(fileDataLine);
        string dataChunk;
        int currentColumn = 0;


        while (getline(lineStream, dataChunk, ',')) {
            if (currentColumn == columnIndex) {
                try {
                    results.totalSum += stod(dataChunk);
                    results.validDataCount++;
                }
                catch (const invalid_argument& e) {
                }
                catch (out_of_range& e) {
                }
                break;
            }
            currentColumn++;
        }
    }
    return results;
};

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
    ifstream file(filename);

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

    // Print rest of file
    string fileDataLine;
    double totalSum = 0;
    long validDataCount = 0;
    long totalCount = 0;

    cout << "Total Count: " << fileResults.totalCount << endl;
    cout << "Valid Count: " << fileResults.validDataCount << endl;
    cout << "Sum: " << fileResults.totalSum << endl;
    cout << "Average: " << (fileResults.totalSum / fileResults.validDataCount) << endl;
    file.close();
    return 0;
}