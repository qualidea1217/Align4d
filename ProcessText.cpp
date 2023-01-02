#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

std::vector<std::vector<std::string>> read_csv(const std::string& file_name) {
    /*
     * Process a csv file into nested vectors of strings
     *
     * @param file_name: file name as string
     * @return: 2d vector of strings, each vector of strings represent a row of csv file, each string is the content separated by comma
     */
    std::vector<std::vector<std::string>> content;
    std::vector<std::string> row;
    std::string line, word;

    std::fstream file(file_name);
    if (file.is_open()) {
        while (getline(file, line)) {
            row.clear();
            std::stringstream str(line);
            while (getline(str, word, ','))
                row.emplace_back(word);
            content.emplace_back(row);
        }
    } else {
        throw std::runtime_error("Could not open the file");
    }
    file.close();
    return content;
}

void write_csv(const std::string& file_name, const std::vector<std::vector<std::string>>& content) {
    std::ofstream file;
    file.open(file_name);
    if (file.is_open()) {
        for (const auto& row: content) {
            for (const auto& token: row) {
                file << token << ",";
            }
            file << std::endl;
        }
    } else {
        throw std::runtime_error("Could not open the file");
    }
    file.close();
};
