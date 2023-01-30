#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "MSA.h"
#include "PreProcess.h"
#include "PostProcess.h"

int main() {
//    std::string file_name{"../data/Bed002_all_tokens.csv"};
//    std::vector<std::vector<std::string>> content = read_csv(file_name);
//    std::vector<std::string> hypothesis = get_total_hypothesis(content, 2);
//    std::vector<std::vector<std::string>> reference_with_label = get_total_reference_with_label(content, 0, 1);
//    std::vector<std::string> reference = reference_with_label[0];
//    std::vector<std::string> reference_label = reference_with_label[1];
//    std::vector<std::string> unique_speaker_label = get_unique_speaker_label(reference_with_label[1]);
//
//    std::vector<std::vector<int>> segment_index = get_segment_index(hypothesis, reference, 88, 6);
//    std::vector<std::vector<std::string>> segmented_hypothesis_list = get_segment_sequence(hypothesis, segment_index[0]);
//    std::vector<std::vector<std::string>> segmented_reference_list = get_segment_sequence(reference, segment_index[1]);
//    std::vector<std::vector<std::string>> segmented_reference_label_list = get_segment_sequence(reference_label, segment_index[1]);
//
//    for (int i = 0; i < segment_index[0].size() - 1; ++i) {
//        std::cout << "segment hypo: " << segment_index[0][i + 1] - segment_index[0][i] << " segment ref: " << segment_index[1][i + 1] - segment_index[1][i] << std::endl;
//    }
//    std::vector<std::vector<std::string>> final_result(unique_speaker_label.size() + 1);
//    long long total_time{0};
//    for (int i = 0; i < segmented_hypothesis_list.size(); ++i) {
//        std::cout << " segment from: " << segment_index[0][i] << " to: " << segment_index[0][i + 1];
//        auto segment_hypothesis = segmented_hypothesis_list[i];
//        auto separated_reference_with_label = get_separate_sequence_with_label(segmented_reference_list[i], segmented_reference_label_list[i]);
//        std::vector<std::vector<std::string>> separated_reference(separated_reference_with_label.begin(), separated_reference_with_label.end() - 1);
//        std::vector<std::string> segment_reference_speaker_label = separated_reference_with_label.back();
//
//        auto start = std::chrono::high_resolution_clock::now();
//        auto result = multi_sequence_alignment(segment_hypothesis, separated_reference);
//        auto end = std::chrono::high_resolution_clock::now();
//        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
//        std::cout << " time: " << duration.count() << std::endl;
//        total_time += duration.count();
//
//        final_result[0].insert(final_result[0].end(), result[0].begin(), result[0].end());
//        for (int j = 0; j < separated_reference.size(); ++j) {
//            int final_result_index = std::ranges::find(unique_speaker_label, segment_reference_speaker_label[j]) - unique_speaker_label.begin() + 1;
//            final_result[final_result_index].insert(final_result[final_result_index].end(), result[j + 1].begin(),result[j + 1].end());
//        }
//        for (int j = 1; j < final_result.size(); ++j) {
//            while (final_result[j].size() < final_result[0].size()) {
//                final_result[j].emplace_back(GAP);
//            }
//        }
//    }
//
//    auto token_match_result = get_token_match_result(final_result);
//    auto align_indexes = get_align_indexes(final_result);
//
//
//    write_csv_multiple_line<std::vector<std::vector<std::string>>>("../data/Bed002_test.csv", final_result);
//    write_csv_multiple_line<std::vector<std::vector<int>>>("../data/Bed002_test.csv", align_indexes);
//    write_csv_single_line<std::vector<std::string>>("../data/Bed002_test.csv", token_match_result);
//    std::cout << "total time: " << total_time << std::endl;

    std::string file_name{"../data/example.csv"};
    std::vector<std::vector<std::string>> content = read_csv(file_name);
    std::vector<std::string> hypothesis = get_total_hypothesis(content, 0);
    std::vector<std::vector<std::string>> reference_with_label = get_total_reference_with_label(content, 3, 4);
    std::vector<std::string> reference = reference_with_label[0];
    std::vector<std::string> reference_label = reference_with_label[1];
    std::vector<std::string> unique_speaker_label = get_unique_speaker_label(reference_with_label[1]);
    std::vector<std::vector<std::string>> separated_ref = get_separate_sequence(reference, reference_label);
    auto start = std::chrono::high_resolution_clock::now();
    auto result = multi_sequence_alignment(hypothesis, separated_ref);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "\ntime: " << duration.count() << std::endl;
    auto token_match_result = get_token_match_result(result);
    auto align_indexes = get_align_indexes(result);
    auto ref_original_index = get_ref_original_index(reference, reference_label);
    write_csv_multiple_line<std::vector<std::vector<std::string>>>("../data/example_output.csv", result);
    write_csv_single_line<std::vector<std::string>>("../data/example_output.csv", token_match_result);
    write_csv_multiple_line<std::vector<std::vector<int>>>("../data/example_output.csv", align_indexes);
    write_csv_multiple_line("../data/example_output.csv", ref_original_index);


//    std::vector<std::string> hypo{"ok", "I", "am", "a", "fish", "Are", "you", "Hello", "there", "How", "are", "you", "ok"};
//    std::vector<std::string> ref{"I", "am", "a", "fish", "ok", "Are", "you", "Hello", "there", "How", "are", "you"};
//    std::vector<std::string> ref_speaker_labels{"B", "B", "B", "B", "A", "C", "C", "D", "D", "E", "E", "E"};
//    std::vector<std::vector<std::string>> separated_ref = get_separate_sequence(ref, ref_speaker_labels);
//    auto start = std::chrono::high_resolution_clock::now();
//    auto result = multi_sequence_alignment(hypo, separated_ref);
//    auto end = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
//    std::cout << "\ntime: " << duration.count() << std::endl;
//
//    auto token_match_result = get_token_match_result(result);
//    auto align_indexes = get_align_indexes(result);
//    auto ref_original_index = get_ref_original_index(ref, ref_speaker_labels);
//
//    write_csv_multiple_line<std::vector<std::vector<std::string>>>("../data/small_test.csv", result);
//    write_csv_single_line<std::vector<std::string>>("../data/small_test.csv", token_match_result);
//    write_csv_multiple_line<std::vector<std::vector<int>>>("../data/small_test.csv", align_indexes);
//    write_csv_multiple_line("../data/small_test.csv", ref_original_index);

    return 0;
}