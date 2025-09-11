#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "filemodification.hpp"

using namespace std;

// Read a text file and optionally filter by a specific column value
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

        while (getline(ss, field, '|')) { // Split line by '|'
            row.push_back(field);
        }

        // Filter by column if colNumber is specified
        if (colNumber == -1 ||
            (colNumber >= 0 && colNumber < row.size() && row[colNumber] == colValue)) {
            data.push_back(row);
        }
    }

    inputFile.close();
    return data;
}

// Write data to a text file, optionally appending
bool writeTxtFile(
    const string& filename,
    const vector<vector<string>>& data,
    bool append
) {
    ofstream outputFile;

    if (append) {
        outputFile.open(filename, ios::out | ios::app);
    }
    else {
        outputFile.open(filename, ios::out | ios::trunc); // overwrite
    }

    if (outputFile.is_open()) {
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                outputFile << row[i];
                if (i < row.size() - 1) {
                    outputFile << "|"; // column separator
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

// Add a new column to an existing text file
bool addColumnToTxtFile(
    const string& filename,
    const vector<string>& newColumnData
) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        return false;
    }

    std::vector<std::vector<std::string>> fileData;
    std::string line;
    size_t rowCount = 0;

    // Read existing file content
    while (std::getline(inputFile, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, '|')) {
            row.push_back(cell);
        }

        fileData.push_back(row);
        ++rowCount;
    }
    inputFile.close();

    // Ensure new column size matches existing row count
    if (newColumnData.size() != rowCount) {
        return false;
    }

    // Append new column values
    for (size_t i = 0; i < rowCount; ++i) {
        fileData[i].push_back(newColumnData[i]);
    }

    // Write updated data back to file
    return writeTxtFile(filename, fileData, false);  // overwrite
}

// Delete a row from a text file based on a column match
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
            found = true; // skip this row
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

// Update a row in a text file based on a column match
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
                row.resize(newRow.size()); // resize if new row is bigger
            }

            for (size_t i = 0; i < newRow.size(); ++i) {
                if (newRow[i] != "")
                    row[i] = newRow[i]; // update non-empty fields
            }

            found = true;
            break;
        }
    }

    if (found) {
        return writeTxtFile(filename, data, false); // overwrite
    }
    else {
        return false;
    }
}
