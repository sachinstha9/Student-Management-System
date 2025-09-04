#ifndef COMMON_HPP
#define COMMON_HPP

std::vector<std::string> splitBySpace(std::string str = "");
bool isValidDateFormat(const std::string& dateStr);
bool isValidEmail(const std::string& email);
bool isValidNZMobileNumber(const std::string& number);
std::string getTodayDate();
std::vector<std::vector<std::string>> readTxtFile(
    const std::string& filename,
    int colNumber = -1,
    const std::string& colValue = ""
);
bool writeTxtFile(const std::string& filename, const std::vector<std::vector<std::string>>& data, bool append = true);
int countRowsInFile(
    const std::string& filename,
    int colNumber = -1,
    const std::string& colValue = "",
    char delimiter = '|'
);
bool updateTxtFile(
    const std::string& filename,
    size_t matchColumnIndex,
    const std::string& matchValue,
    const std::vector<std::string>& newRow
);
bool deleteRowTxtFile(
    const std::string& filename,
    size_t matchColumnIndex,
    const std::string& matchValue
);
bool checkCommandValidity(std::string command, std::vector<std::vector<std::string>> defCommands);
void printTable(const std::vector<std::vector<std::string>>& table, int sortByColumn = -1, bool ascending = true);
std::string trim(const std::string& s);

#endif