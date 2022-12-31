#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>

#define FULLY_MATCH_SCORE 2
#define PARTIAL_MATCH_SCORE 1
#define MISMATCH_SCORE (-1)
#define GAP_SCORE (-1)
#define GAP "-"

int edit_distance(const std::string &token1, const std::string &token2) {
    if (token1 == token2) {
        return 0;
    }
//    int matrix[token1.length() + 1][token2.length() + 1]; // use this if compile with GCC not MSVC for better performance
//    std::fill(&matrix[0][0], &matrix[0][0] + sizeof(matrix) / sizeof(int), 0); // use this if compile with GCC not MSVC for better performance
    std::vector<std::vector<int>> matrix(token1.length() + 1, std::vector<int>(token2.length() + 1, 0)); // use this or pointers if compile with MSVC
    for (int i = 1; i < token1.length() + 1; ++i) {
        matrix[i][0] = i;
    }
    for (int j = 1; j < token2.length() + 1; ++j) {
        matrix[0][j] = j;
    }
    for (int i = 1; i < token1.length() + 1; ++i) {
        for (int j = 1; j < token2.length() + 1; ++j) {
            int substitution = token1[i - 1] == token2[j - 1] ? 0 : 1;
            int parameter[3] = {matrix[i - 1][j] + 1, matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + substitution};
            matrix[i][j] = *std::ranges::min_element(parameter);
        }
    }
    return matrix[token1.length()][token2.length()];
}

int compare(const std::string &hypothesis, const std::vector<std::string> &reference_list) {
    std::string reference = GAP;
    for (const std::string &token: reference_list) {
        if (token != GAP) {
            if (reference == GAP) {
                reference = token;
            } else {
                return -1; // score for more than 2 tokens, should not happen
            }
        }
    }
    if (reference == GAP) {
        return GAP_SCORE;
    } else {
        if (hypothesis == reference) {
            return FULLY_MATCH_SCORE;
        } else if (edit_distance(hypothesis, reference) < 2) {
            return PARTIAL_MATCH_SCORE;
        } else {
            return MISMATCH_SCORE;
        }
    }
}

void get_sequence_position_list_aux(const std::vector<int> &position, int size, int index,
                                    std::vector<std::vector<int>> &sequence_position, std::vector<int> &combination) {
    if (combination.size() == size) {
        sequence_position.emplace_back(combination);
        return;
    }
    for (int i = index; i < position.size(); ++i) {
        combination.emplace_back(position[i]);
        get_sequence_position_list_aux(position, size, i + 1, sequence_position, combination);
        combination.pop_back();
    }
}

std::vector<std::vector<int>> get_sequence_position_list(int speaker_sequence_length) {
    std::vector<int> position;
    for (int i = 0; i < speaker_sequence_length; ++i) {
        position.emplace_back(i);
    }
    std::vector<std::vector<int>> sequence_position;
    std::vector<int> combination;
    for (int i = 1; i < speaker_sequence_length + 1; ++i) {
        get_sequence_position_list_aux(position, i, 0, sequence_position, combination);
    }
    return sequence_position;
}

std::vector<std::vector<int>> get_current_index(const std::vector<int>& position, const std::vector<int>& matrix_size) {
    int sequence_num = matrix_size.size();
    std::vector<std::vector<int>> current_list;
    std::vector<int> current;
    for (int i = 0; i < sequence_num; ++i) {
        if (std::ranges::find(position, i) == position.end()) {
            current.emplace_back(0);
        } else {
            current.emplace_back(1);
        }
    }
    while (true) {
        current_list.emplace_back(current);
        current[position.back()]++;
        for (int i = position.size() - 1; i >= 0; i--) {
            if (current[position[0]] == matrix_size[position[0]]) {
                return current_list;
            }
            if (current[position[i]] == matrix_size[position[i]]) {
                current[position[i - 1]]++;
                current[position[i]] = 1;
            }
        }
    }
}

std::vector<std::vector<int>> get_parameter_index_list(const std::vector<int>& sequence_position, const std::vector<int>& current_index) {
    std::vector<std::vector<int>> parameter_index_list;
    for (int index: sequence_position) {
        std::vector<int> single_token_index;
        for (int i = 0; i < current_index.size(); ++i) {
            single_token_index.emplace_back(i != index ? current_index[i] : current_index[i] - 1);
        }
        parameter_index_list.emplace_back(single_token_index);
        if (std::ranges::find(sequence_position, 0) != sequence_position.end() && index != 0) {
            std::vector<int> double_token_index = single_token_index;
            double_token_index[0]--;
            parameter_index_list.emplace_back(double_token_index);
        }
    }
    return parameter_index_list;
}

std::vector<std::string> get_compare_parameter(const std::vector<int>& current_index, const std::vector<int>& parameter_index, const std::vector<std::vector<std::string>>& speaker_sequence) {
    std::vector<std::string> compare_parameter;
    for (int i = 0; i < current_index.size(); ++i) {
        if (current_index[i] != parameter_index[i]) {
            compare_parameter.emplace_back(speaker_sequence[i][parameter_index[i]]);
        } else {
            compare_parameter.emplace_back(GAP);
        }
    }
    return compare_parameter;
}

size_t get_index(const std::vector<int>& current_index, const std::vector<int>& matrix_size) {
    int index{0};
    for (int i = 0; i < matrix_size.size(); ++i) {
        if (current_index[i] != 0) {
            int temp{1};
            for (int j = i + 1; j < matrix_size.size(); ++j) {
                temp *= matrix_size[j];
            }
            index +=current_index[i] * temp;
        }
    }
    return index;
}

std::vector<std::vector<std::string>> multi_sequence_alignment(const std::vector<std::string>& hypothesis, const std::vector<std::vector<std::string>>& reference) {
    std::vector<std::vector<std::string>> speaker_sequence;
    speaker_sequence.emplace_back(hypothesis);
    for (const std::vector<std::string>& ref: reference) {
        speaker_sequence.emplace_back(ref);
    }
    std::vector<int> matrix_size;
    size_t total_cell{1};
    for (const std::vector<std::string>& speaker: speaker_sequence) {
        matrix_size.emplace_back(speaker.size() + 1);
        total_cell *= speaker.size() + 1;
    }
    std::vector<int> score(total_cell);

    // computing score
    for (const std::vector<int>& sequence_position: get_sequence_position_list((int)speaker_sequence.size())) {
        for (const std::vector<int>& current_index: get_current_index(sequence_position, matrix_size)) {
            std::vector<int> parameter;
            for (const std::vector<int>& parameter_index: get_parameter_index_list(sequence_position, current_index)) {
                std::vector<std::string> compare_parameter = get_compare_parameter(current_index, parameter_index, speaker_sequence);
                std::string hypo = compare_parameter[0];
                std::vector<std::string> ref(compare_parameter.begin() + 1, compare_parameter.end());
                parameter.emplace_back(score[get_index(parameter_index, matrix_size)] + compare(hypo, ref));
            }
            score[get_index(current_index, matrix_size)] = *std::ranges::max_element(parameter);
        }
    }

    // backtracking
    std::vector<std::vector<std::string>> align_sequence(speaker_sequence.size());
    // initialize mappings here
    std::vector<int> current_index;
    for (int size: matrix_size) {
        current_index.emplace_back(size - 1);
    }
    while (std::accumulate(current_index.begin(), current_index.end(), 0) > 0) {
        std::vector<int> sequence_position;
        for (int i = 0; i < current_index.size(); ++i) {
            if (current_index[i] != 0) {
                sequence_position.emplace_back(i);
            }
        }
        for (const std::vector<int>& parameter_index: get_parameter_index_list(sequence_position, current_index)) {
            std::vector<std::string> compare_parameter = get_compare_parameter(current_index, parameter_index, speaker_sequence);
            std::string hypo = compare_parameter[0];
            std::vector<std::string> ref(compare_parameter.begin() + 1, compare_parameter.end());
            if (score[get_index(current_index, matrix_size)] == compare(hypo, ref) + score[get_index(parameter_index, matrix_size)]) {
                align_sequence[0].emplace_back(hypo);
                for (int i = 1; i < align_sequence.size(); ++i) {
                    align_sequence[i].emplace_back(ref[i - 1]);
                    // update mappings here
                }
                current_index = parameter_index;
                break;
            }
        }
    }
    for (std::vector<std::string> &sequence : align_sequence) {
        std::ranges::reverse(sequence);
    }
    return align_sequence;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::string> hypo{"ok", "I", "am", "a", "fish", "Are", "you", "Hello", "there", "How", "are", "you", "ok"};
    std::vector<std::vector<std::string>> ref{
            {"I", "am", "a", "fish"},
            {"Are", "you"},
            {"ok"},
            {"Hello", "there"},
            {"How", "are", "you"}
    };
    auto result = multi_sequence_alignment(hypo, ref);
    auto end = std::chrono::high_resolution_clock::now();

    for (const auto& sequence: result) {
        for (const auto& token: sequence) {
            std::cout << token << " ";
        }
        std::cout << std::endl;
    }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "time: " << duration.count() << std::endl;
    return 0;
}
