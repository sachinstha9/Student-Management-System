#ifndef FILEMODIFICATION_HPP
#define FILEMODIFICATION_HPP

std::vector<std::vector<std::string>> readTxtFile(
    const std::string& filename,
    int colNumber = -1,
    const std::string& colValue = ""
);

bool writeTxtFile(
    const std::string& filename,
    const std::vector<std::vector<std::string>>& data,
    bool append = true
);

bool addColumnToTxtFile(
    const std::string& filename,
    const std::vector<std::string>& newColumnData
);

bool deleteRowTxtFile(
    const std::string& filename,
    size_t matchColumnIndex,
    const std::string& matchValue
);

bool updateTxtFile(
    const std::string& filename,
    size_t matchColumnIndex,
    const std::string& matchValue,
    const std::vector<std::string>& newRow
);

#endif