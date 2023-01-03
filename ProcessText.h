#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#ifndef MSA_PROCESSTEXT_H
#define MSA_PROCESSTEXT_H

std::vector<std::vector<std::string>> read_csv(const std::string&);

std::vector<std::string> get_total_hypothesis(const std::vector<std::vector<std::string>>&, int);

std::vector<std::vector<std::string>> get_total_reference_with_label(const std::vector<std::vector<std::string>>&, int, int);

std::vector<std::vector<int>> get_segment_index(const std::vector<std::string>&, const std::vector<std::string>&, int, int);

std::vector<std::vector<std::string>> separate_reference(const std::vector<std::string>&, const std::vector<std::string>&);

void write_csv(const std::string&, const std::vector<std::vector<std::string>>&);

#endif //MSA_PROCESSTEXT_H
