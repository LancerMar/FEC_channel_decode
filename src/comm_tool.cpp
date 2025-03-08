#include "comm_tool.h"

std::vector<char> octal_2_binary(int octal) {
    std::vector<std::vector<char>> octal_ref_binary = { {0,0,0},{0,0,1},{0,1,0},{0,1,1},
                                                    {1,0,0},{1,0,1},{1,1,0},{1,1,1} };

    int Octal_tmp = octal;
    std::vector<char> binary_vec;
    while (Octal_tmp > 0.1) {
        int single_octal = Octal_tmp % 10;
        auto single_binary_vec = octal_ref_binary[single_octal];
        binary_vec.insert(binary_vec.begin(), single_binary_vec.begin(), single_binary_vec.end());

        Octal_tmp = Octal_tmp / 10;
    }

    //remove all 0s at start
    int first_non_zero = 0;
    for (int i = 0; i < binary_vec.size(); i++) {
        if (binary_vec[i] != 0) {
            first_non_zero = i;
            break;
        }
    }
    if (0 != first_non_zero) {
        binary_vec.erase(binary_vec.begin(), binary_vec.begin() + first_non_zero);
    }

    return binary_vec;
}


std::vector<char> decimal_2_binary(int decimal) {
    int decima_tmp = decimal;

    std::vector<char> binary_vec;
    while (decima_tmp > 0.1) {
        char binary_single = static_cast<char>(decima_tmp % 2);
        binary_vec.insert(binary_vec.begin(), binary_single);
        decima_tmp = decima_tmp / 2;
    }

    return binary_vec;
}

std::vector<std::vector<char>> gen_binary_matrix_by_decimal(int max_decimal) {
    std::vector<std::vector<char>> binary_matrix;
    
    for (int i = 0; i < max_decimal; i++) {
        binary_matrix.push_back(decimal_2_binary(i));
    }

    // padding 0 in MSB of Matrix
    int cols_max = binary_matrix[binary_matrix.size() - 1].size();

    for (int i = binary_matrix.size() / 2 - 1; i > -1; i--) {
        int num_zeros = cols_max - binary_matrix[i].size();
        std::vector<char> zero_vec(num_zeros, 0);
        zero_vec.insert(zero_vec.end(), binary_matrix[i].begin(), binary_matrix[i].end());
        binary_matrix[i] = zero_vec;
    }

    return binary_matrix;
}

//template <class T>
//std::vector<T> ptr2vec(T* data, int len) {
//    std::vector<T> vec(len, 0);
//    for (int i = 0; i < len; i++) {
//        vec[i] = data[i];
//    }
//    return vet;
//}

