#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <regex>

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

bool isValidDateFormat(const string& dateStr) {
    static const regex pattern(R"(^\d{4}-\d{2}-\d{2}$)");
    return regex_match(dateStr, pattern);
}

bool isValidEmail(const string& email) {
    if (email.empty())
        return false;

    const regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex_match(email, pattern);
}

bool isValidNZMobileNumber(const string& number) {
    regex localFormat("^02[0-9]{7,8}$");
    regex internationalFormat("^\\+642[0-9]{7,8}$");

    return regex_match(number, localFormat) || regex_match(number, internationalFormat);
}

string getTodayDate() {
    time_t t = time(nullptr);
    tm tm_buf{};
    localtime_s(&tm_buf, &t);

    ostringstream oss;
    oss << put_time(&tm_buf, "%Y-%m-%d");
    return oss.str();
}

bool checkCommandValidity(string command, vector<vector<string>> defCommands) {
    int i = 0;
    for (i = 0; i < defCommands.size(); i++) {
        if (defCommands[i][0] == command || command == "") {
            break;
        }
    }

    if (i >= defCommands.size()) {
        return false;
    }

    return true;
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
    if (table.empty()) return;

    size_t numCols = 0;
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
    for (const auto& row : sortedTable)
        for (size_t i = 0; i < row.size(); ++i)
            colWidths[i] = max(colWidths[i], row[i].length());

    auto printBorder = [&]() {
        cout << "+";
        for (auto width : colWidths)
            cout << string(width + 2, '-') << "+";
        cout << '\n';
        }; 

    auto printRow = [&](const vector<string>& row) {
        cout << "|";
        for (size_t i = 0; i < numCols; ++i) {
            string cell = (i < row.size()) ? row[i] : "";
            cout << " " << left << setw(colWidths[i]) << cell << " |";
        }
        cout << '\n';
        }; 

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

vector<vector<string>> readTxtFile(
    const string& filename,
    int colNumber,
    const string& colValue
) {
    vector<vector<string>> data;
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Error: Unable to find the file." << endl;
        return data;
    }

    string line;
    while (getline(inputFile, line)) {
        vector<string> row;
        stringstream ss(line);
        string field;

        while (getline(ss, field, '|')) {
            row.push_back(field);
        }

        if (colNumber == -1 ||
            (colNumber >= 0 && colNumber < row.size() && row[colNumber] == colValue)) {
            data.push_back(row);
        }
    }

    inputFile.close();
    return data;
}

bool writeTxtFile(const string& filename, const vector<vector<string>>& data, bool append) {
    ofstream outputFile;

    if (append) {
        outputFile.open(filename, ios::out | ios::app);
    }
    else {
        outputFile.open(filename, ios::out | ios::trunc);
    }

    if (outputFile.is_open()) {
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                outputFile << row[i];
                if (i < row.size() - 1) {
                    outputFile << "|";
                }
            }
            outputFile << "\n";
        }
        outputFile.close();

        return true;
    }
    else 
        return false;
}

bool updateTxtFile(
    const string& filename,
    size_t matchColumnIndex,
    const string& matchValue,
    const vector<string>& newRow
) {
    auto data = readTxtFile(filename);
    bool found = false;

    for (auto& row : data) {
        if (row.size() > matchColumnIndex && row[matchColumnIndex] == matchValue) {
            if (row.size() < newRow.size()) {
                row.resize(newRow.size());
            }

            for (size_t i = 0; i < newRow.size(); ++i) {
                if (newRow[i] != "")
                    row[i] = newRow[i];
            }

            found = true;
            break;
        }
    }

    if (found) {
        return writeTxtFile(filename, data, false);
    }
    else {
        return false;
    }
}

bool deleteRowTxtFile(
    const string& filename,
    size_t matchColumnIndex,
    const string& matchValue
) {
    auto data = readTxtFile(filename);

    bool found = false;

    vector<vector<string>> updatedData;

    for (const auto& row : data) {
        if (row.size() > matchColumnIndex && row[matchColumnIndex] == matchValue) {
            found = true; 
            continue;     
        }

        updatedData.push_back(row);
    }

    if (found) {
        return writeTxtFile(filename, updatedData, false);
    }
    else {
        return false;
    }
}


int countRowsInFile(
    const string& filename,
    int colNumber,
    const string& colValue,
    char delimiter
) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return -1;
    }

    string line;
    int count = 0;

    while (getline(file, line)) {
        if (colNumber == -1) {
            count++;
        }
        else {
            stringstream ss(line);
            string cell;
            int currentCol = 0;
            bool matched = false;

            while (getline(ss, cell, delimiter)) {
                if (currentCol == colNumber) {
                    if (cell == colValue) {
                        matched = true;
                    }
                    break;
                }
                currentCol++;
            }

            if (matched) count++;
        }
    }

    file.close();
    return count;
}
