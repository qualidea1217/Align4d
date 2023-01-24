#include <iostream>
#include <string>
#include <vector>

#include "MSA.h"
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

std::vector<std::string> get_token_match_result(const std::vector<std::vector<std::string>>& final_result) {
    /*
     * Get the match result (fully match, partially match, mismatch, gap) for each position of token after alignment
     * The rule of comparison must be the same as compare function in MSA
     *
     * @param final_result: final output of the alignment with just the aligned sequences from multi_sequence_alignment
     * @return: match result for each position of token specified in strings
     */
    std::vector<std::string> token_match_result;
    std::vector<std::string> compare_token;
    for (int i = 0; i < final_result[0].size(); ++i) {
        for (const std::vector<std::string>& sequence: final_result) {
            if (sequence[i] != GAP) {
                compare_token.emplace_back(sequence[i]);
            }
        }
        if (compare_token.size() == 2) {
            if (compare_token[0] == compare_token[1]) {
                token_match_result.emplace_back("fully match");
            } else if (edit_distance(compare_token[0], compare_token[1]) < 2) {
                token_match_result.emplace_back("partially match");
            } else {
                token_match_result.emplace_back("mismatch");
            }
        } else {
            token_match_result.emplace_back("gap");
        }
    }
    return token_match_result;
}

std::vector<std::vector<int>> get_align_indexes(const std::vector<std::vector<std::string>>& final_result) {
    /*
     * Get the indexes representing the mapping from each token in the separated reference sequences to the hypothesis.
     *
     * Each index shows the relative position (index) in the hypothesis sequence of the
     * non-gap token (fully match, partially match, or mismatch) from the separated reference sequences.
     * If the index is -1, it means that the current token does not aligned to any token in the hypothesis (align to a gap).
     *
     * @param final_result: final output of the alignment with just the aligned sequences from multi_sequence_alignment
     * @return: indexes showing the mapping between tokens from separated reference to hypothesis
     */
    std::vector<std::vector<int>> align_indexes(final_result.size() - 1);
    std::vector<std::string> align_hypo = final_result[0];
    for (int i = 1; i < final_result.size(); ++i) {
        std::vector<int> indexes;
        for (int j = 0; j < align_hypo.size(); ++j) {
            if (final_result[i][j] != GAP) {
                indexes.emplace_back(align_hypo[j] != GAP ? j : -1);
            }
        }
        align_indexes.emplace_back(indexes);
    }
    return align_indexes;
}


