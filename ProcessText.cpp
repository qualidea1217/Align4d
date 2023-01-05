#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "ProcessText.h"

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

std::vector<std::string> get_total_hypothesis(const std::vector<std::vector<std::string>>& content, int line) {
    return content[line];
}

std::vector<std::vector<std::string>> get_total_reference_with_label(const std::vector<std::vector<std::string>>& content, int reference_token_line, int speaker_label_line) {
    std::vector<std::vector<std::string>> output{content[reference_token_line], content[speaker_label_line]};
    return output;
}

std::vector<std::string> get_unique_speaker_label(const std::vector<std::string>& speaker_labels) {
    std::set<std::string> unique_speaker_label_set(speaker_labels.begin(), speaker_labels.end());
    std::vector<std::string> unique_speaker_labels(unique_speaker_label_set.begin(), unique_speaker_label_set.end());
    return unique_speaker_labels;
}

std::vector<std::vector<int>> get_segment_index(const std::vector<std::string>& hypothesis, const std::vector<std::string>& reference, int segment_length, int barrier_length) {
    /*
     * Segment the hypothesis and reference text into segments with about the length of segment_length.
     *
     * This algorithm starts at the beginning of hypothesis, advance the length of segment_length,
     * then it compare the next sequence with length of barrier_length with every possible sequence with length of barrier_length within reference;
     * if two sequence are equal, the index for segmentation is the midpoint of these two barrier_length sequence,
     * then it advance the length of segment_length and continue segmentation with the above procedure;
     * if no equal sequence are found in reference, the hypothesis index will only advance by 1 and continue above procedure
     *
     * @param hypothesis: hypothesis token sequence as vector of string
     * @param reference: reference token sequence as vector of string
     * @param segment_length: length of each segment (around this value, mostly will be equal or greater) based on hypothesis segment
     * @param barrier_length: length of sequence that is used to determine the absolute correct point to chop the segment,
     * larger will create more accurate segmentation but will reduce number of segment and increase length of each segment
     * @return: 2d vector of int, including 2 vector of int, the first one is the index of segmentation of hypothesis,
     * the second one is for reference, including first and last index of the whole text
     */
    std::vector<int> hypo_index{0}, ref_index{0};
    bool is_same_sequence;
    for (int i = segment_length; i < hypothesis.size() - barrier_length; ++i) {
        for (int j = ref_index.back(); j < reference.size() - barrier_length; ++j) {
//            if (j - ref_index.back() > 5 * segment_length) {
//                break;
//            }
            is_same_sequence = std::equal(hypothesis.begin() + i, hypothesis.begin() + i + barrier_length, reference.begin() + j);
            if (is_same_sequence) {
                hypo_index.emplace_back(i + (int)(barrier_length / 2));
                ref_index.emplace_back(j + (int)(barrier_length / 2));
                i += segment_length;
                break;
            }
        }
    }
    hypo_index.emplace_back(hypothesis.size() - 1);
    ref_index.emplace_back(reference.size() - 1);
    std::vector<std::vector<int>> segment_index{hypo_index, ref_index};
    return segment_index;
}

std::vector<std::vector<std::string>> get_segment_sequence(const std::vector<std::string>& tokens, const std::vector<int>& segment_index) {
    std::vector<std::vector<std::string>> segments;
    for (int i = 0; i < segment_index.size() - 1; ++i) {
        segments.emplace_back(tokens.begin() + segment_index[i], tokens.begin() + segment_index[i + 1]);
    }
    return segments;
}

std::vector<std::vector<std::string>> get_separate_sequence(const std::vector<std::string>& tokens, const std::vector<std::string>& speaker_labels) {
    std::vector<std::string> unique_speaker_labels = get_unique_speaker_label(speaker_labels);
    std::vector<std::vector<std::string>> reference(unique_speaker_labels.size());
    for (int i = 0; i < tokens.size(); ++i) {
        reference[std::ranges::find(unique_speaker_labels, speaker_labels[i]) - unique_speaker_labels.begin()].emplace_back(tokens[i]);
    }
    return reference;
}

std::vector<std::vector<std::string>> get_separate_sequence_with_label(const std::vector<std::string>& tokens, const std::vector<std::string>& speaker_labels) {
    std::vector<std::string> unique_speaker_labels = get_unique_speaker_label(speaker_labels);
    std::vector<std::vector<std::string>> reference(unique_speaker_labels.size());
    for (int i = 0; i < tokens.size(); ++i) {
        reference[std::ranges::find(unique_speaker_labels, speaker_labels[i]) - unique_speaker_labels.begin()].emplace_back(tokens[i]);
    }
    reference.emplace_back(unique_speaker_labels);
    return reference;
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
}
