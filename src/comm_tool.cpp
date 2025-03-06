#include "comm_tool.h"

std::vector<char> Octal2binary(int Octal) {
    std::vector<std::vector<char>> octal_ref_binary = { {0,0,0},{0,0,1},{0,1,0},{0,1,1},
                                                    {1,0,0},{1,0,1},{1,1,0},{1,1,1} };

    int Octal_tmp = Octal;
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






//template <class T>
//std::vector<T> ptr2vec(T* data, int len) {
//    std::vector<T> vec(len, 0);
//    for (int i = 0; i < len; i++) {
//        vec[i] = data[i];
//    }
//    return vet;
//}

