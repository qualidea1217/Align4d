#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#ifndef MSA_PROCESSTEXT_H
#define MSA_PROCESSTEXT_H

std::vector<std::vector<std::string>> read_csv(const std::string&);

void write_csv(const std::string&, const std::vector<std::vector<std::string>>&);

#endif //MSA_PROCESSTEXT_H
