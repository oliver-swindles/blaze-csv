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

int main(int argc, char** argv) {
    // Read args
    // In format [program] [filename.csv] [column name] [operation]
    Timer timer;
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

    // Print rest of file
    string tempText;
    int count = 0;
    while (getline (file, tempText)) {
        count++;
    }

    cout << "Count: " << count << endl;
    file.close();
    return 0;
}