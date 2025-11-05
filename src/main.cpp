#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

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
    string filename = argv[1];
    string columnName = argv[2];
    int columnIndex = -1;
    // string operation = argv[3];

    string tempText;
    ifstream file(filename);

    string columnHeaders;

    // Matching column headers to arguments passed in
    getline(file, columnHeaders);

    auto headers = split(columnHeaders, ',');

    for (int i = 0; i < headers.size(); i++) {
        if (headers[i] == columnName) {
            columnIndex = i;
            break;
        };
    }

    cout << "Index:" << columnIndex << endl;

    // cout << columnHeaders << endl;

    while (getline (file, tempText)) {
        cout << tempText;
    }

    file.close();

    return 0;
}