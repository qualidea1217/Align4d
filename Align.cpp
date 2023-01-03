#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "MSA.h"
#include "ProcessText.h"

int main() {
    std::string file_name{"../data/Bdb001_all_tokens.csv"};
    std::vector<std::vector<std::string>> content = read_csv(file_name);
    std::vector<std::string> hypothesis = get_total_hypothesis(content, 2);
    std::vector<std::vector<std::string>> reference_with_label = get_total_reference_with_label(content, 0, 1);
    std::vector<std::string> reference = reference_with_label[0];
    std::vector<std::string> reference_label = reference_with_label[1];
    std::vector<std::vector<int>> segment_index = get_segment_index(hypothesis, reference, 100, 6);


//    auto start = std::chrono::high_resolution_clock::now();
//    auto result = multi_sequence_alignment(hypothesis, reference);
//    auto end = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
//    std::cout << "time: " << duration.count() << std::endl;
//
//    write_csv("../data/test_output1.csv", result);

    return 0;
}