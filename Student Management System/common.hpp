#ifndef COMMON_HPP
#define COMMON_HPP

std::vector<std::string> split(std::string str = "", char delimiter = ' ');
std::string trim(const std::string& s);
bool isValidEmail(const std::string& email);
std::string getTodayDate();
bool checkCommandValidity(
	std::string command,
	std::vector<std::vector<std::string>> defCommands,
	int userType
);
void printTable(
	const std::vector<std::vector<std::string>>& table,
	int sortByColumn = -1,
	bool ascending = true
);

#endif