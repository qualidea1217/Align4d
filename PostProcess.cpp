#include <iostream>
#include <string>
#include <vector>

#include "PostProcess.h"

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
}


