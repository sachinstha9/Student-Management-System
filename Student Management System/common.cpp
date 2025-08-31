#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "common.hpp"

using namespace std;

vector<string> splitBySpace(string str) {
    if (str.empty())
        return { "" };

    istringstream iss(str);
    string word;
    vector<string> words;

    while (iss >> word) {
        words.push_back(word);
    }

    return words;
}

void checkCommandValidity(string command, vector<vector<string>> defCommands) {
    int i = 0;
    for (i = 0; i < defCommands.size(); i++) {
        if (defCommands[i][0] == command || command == "") {
            break;
        }
    }

    if (i >= defCommands.size()) {
        cout << "Command invalid. Type 'help' to see valid commands." << endl;
    }
}

string trim(const string& s) {
    // trims whitespace of a string
    if (s.empty())
        return "";

    auto start = find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !isspace(ch);
        });

    auto end = find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !isspace(ch);
        }).base();

    return (start < end) ? string(start, end) : "";
}

void printTable(const vector<vector<string>>& table, int sortByColumn, bool ascending) {
    // display vector<vector<string>> format data in table format
    if (table.empty()) return;

    size_t numCols = 0;
    // gets number of columns
    for (const auto& row : table)
        numCols = max(numCols, row.size());

    vector<vector<string>> sortedTable = table;

    if (sortByColumn >= 0 && sortByColumn < numCols) {
        sort(sortedTable.begin() + 1, sortedTable.end(), [&](const vector<string>& a, const vector<string>& b) {
            string valA = (sortByColumn < a.size()) ? a[sortByColumn] : "";
            string valB = (sortByColumn < b.size()) ? b[sortByColumn] : "";
            return ascending ? (valA < valB) : (valA > valB);
            });
    }

    vector<size_t> colWidths(numCols, 0);
    // gets width of each columns
    for (const auto& row : sortedTable)
        for (size_t i = 0; i < row.size(); ++i)
            colWidths[i] = max(colWidths[i], row[i].length());

    auto printBorder = [&]() {
        cout << "+";
        for (auto width : colWidths)
            cout << string(width + 2, '-') << "+";
        cout << '\n';
        }; // prints border

    auto printRow = [&](const vector<string>& row) {
        cout << "|";
        for (size_t i = 0; i < numCols; ++i) {
            string cell = (i < row.size()) ? row[i] : "";
            cout << " " << left << setw(colWidths[i]) << cell << " |";
        }
        cout << '\n';
        }; // prints row

    printBorder();
    bool isHeader = true;
    for (const auto& row : sortedTable) {
        printRow(row);
        if (isHeader) {
            printBorder();
            isHeader = false;
        }
    }
    printBorder();
}

vector<vector<string>> readTxtFile(const string& filename) {
    vector<::vector<string>> data;
    ifstream inputFile(filename);

    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            vector<string> row;
            stringstream ss(line);
            string field;

            while (getline(ss, field, '|')) {
                row.push_back(field);
            }
            data.push_back(row);
        }
        inputFile.close();

    }
    else {
        cerr << "Error: Unable to find the file." << endl;
    }

    return data;
}