#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>

#include "common.hpp"

using namespace std;

// Split a string by a given delimiter
vector<string> split(string str, char delimiter) {
    if (str.empty())
        return { "" };

    vector<string> tokens;
    string token;
    stringstream ss(str);

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

// Trim whitespace from start and end of a string
string trim(const string& s) {
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

// Check if email is valid using regex
bool isValidEmail(const string& email) {
    if (email.empty())
        return false;

    const regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex_match(email, pattern);
}

// Get today's date in YYYY-MM-DD format
string getTodayDate() {
    time_t t = time(nullptr);
    tm tm_buf{};
    localtime_s(&tm_buf, &t);

    ostringstream oss;
    oss << put_time(&tm_buf, "%Y-%m-%d");
    return oss.str();
}

// Check if command is valid and user has access
bool checkCommandValidity(string command, vector<vector<string>> defCommands, int userType) {
    int i = 0;
    for (i = 0; i < defCommands.size(); i++) {
        if (defCommands[i][0] == command || command == "") {
            break;
        }
    }

    if (i >= defCommands.size()) {
        cout << "Command invalid. Type 'help' to see valid commands." << endl;
        return false;
    }

    vector<string> validUsers = split(defCommands[i][2]);

    if (find(validUsers.begin(), validUsers.end(), to_string(userType)) == validUsers.end() && validUsers[0] != "0") {
        cout << "You must be ";

        for (size_t i = 0; i < validUsers.size(); ++i) {
            if (validUsers[i] == "1")
                cout << "Admin";
            else if (validUsers[i] == "2")
                cout << "Teacher";
            else if (validUsers[i] == "3")
                cout << "Student";

            if (i != validUsers.size() - 1)
                cout << " or "; // add "or" between roles
        }

        cout << " to execute this command." << endl;
        return false;
    }

    return true;
}

// Print a table with optional sorting
void printTable(const vector<vector<string>>& table, int sortByColumn, bool ascending) {
    if (table.empty()) return;

    size_t numCols = 0;
    for (const auto& row : table)
        numCols = max(numCols, row.size());

    vector<vector<string>> sortedTable = table;

    // Sort by column if specified
    if (sortByColumn >= 0 && sortByColumn < numCols) {
        sort(sortedTable.begin() + 1, sortedTable.end(), [&](const vector<string>& a, const vector<string>& b) {
            string valA = (sortByColumn < a.size()) ? a[sortByColumn] : "";
            string valB = (sortByColumn < b.size()) ? b[sortByColumn] : "";
            return ascending ? (valA < valB) : (valA > valB);
            });
    }

    // Calculate column widths
    vector<size_t> colWidths(numCols, 0);
    for (const auto& row : sortedTable)
        for (size_t i = 0; i < row.size(); ++i)
            colWidths[i] = max(colWidths[i], row[i].length());

    // Lambda to print table border
    auto printBorder = [&]() {
        cout << "+";
        for (auto width : colWidths)
            cout << string(width + 2, '-') << "+";
        cout << '\n';
        };

    // Lambda to print a single row
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
            printBorder(); // Separate header from data
            isHeader = false;
        }
    }
    printBorder();
}
