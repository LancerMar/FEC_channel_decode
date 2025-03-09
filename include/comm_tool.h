#pragma once

#include<vector>

std::vector<char> octal_2_binary(int Octal);

std::vector<char> decimal_2_binary(int decimal);
int binary_2_decimal(std::vector<char> bin_vec);

std::vector<std::vector<char>> gen_binary_matrix_by_decimal(int max_decimal);
