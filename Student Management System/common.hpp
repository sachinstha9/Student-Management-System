#ifndef COMMON_HPP
#define COMMON_HPP

std::vector<std::string> splitBySpace(std::string str = "");
std::vector<std::vector<std::string>> readTxtFile(const std::string& filename);
void checkCommandValidity(std::string command, std::vector<std::vector<std::string>> defCommands);
void printTable(const std::vector<std::vector<std::string>>& table, int sortByColumn = -1, bool ascending = true);
std::string trim(const std::string& s);

#endif