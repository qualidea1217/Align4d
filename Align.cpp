#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "MSA.h"
#include "ProcessText.h"

int main() {
    std::string file_name{"../data/test2.csv"};
    std::vector<std::vector<std::string>> content = read_csv(file_name);
    std::vector<std::string> hypothesis(content[0]);
    std::vector<std::vector<std::string>> reference(content.begin() + 1, content.end());

    auto start = std::chrono::high_resolution_clock::now();
    auto result = multi_sequence_alignment(hypothesis, reference);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "time: " << duration.count() << std::endl;

    write_csv("../data/test_output2.csv", result);
    return 0;
}