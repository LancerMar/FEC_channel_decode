#include "FEC_VIT_imp.h"

#include <iostream>
#include <vector>

#include "comm_tool.h"

FEC_CHANNEL_DECODE::FEC_VIT_IMP::FEC_VIT_IMP() {
    std::cout << "FEC_VIT_IMP()" << std::endl;
}

FEC_CHANNEL_DECODE::FEC_VIT_IMP::~FEC_VIT_IMP() {
    std::cout << "~FEC_VIT_IMP()" << std::endl;
}

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::init() {
    //step 1: generate output reference table
    gen_output_reference();

    //step 2: generate input reference previous state and current state table
    gen_input_ref_cur_pre();
}

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::encode() {}

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::decode() {}

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::set_polynomials(int* poly_ptr, int poly_len) {
    std::vector<int> poly_vec(poly_ptr, poly_ptr + poly_len);

    for (int i = 0; i < poly_vec.size(); i++) {
        _poly.push_back(octal_2_binary(poly_vec[i]));
    }
};

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::set_constrain_length(int constrain_length) {
    _constrain_length = constrain_length;
};


void FEC_CHANNEL_DECODE::FEC_VIT_IMP::gen_output_reference() {
    int output_count = std::pow(2, _poly.size());
    for (int i = 0; i < output_count; i++) {
        _output_reference.push_back(decimal_2_binary(i));
    }
}

void  FEC_CHANNEL_DECODE::FEC_VIT_IMP::gen_input_ref_cur_pre() {
    int row_col_count = std::pow(2, _constrain_length - 1);

    std::vector<std::vector<char>> input_ref_cur_pre_table(row_col_count, std::vector<char>(row_col_count, 0));
    for (int i = row_col_count / 2; i < row_col_count; i++) {
        int cols = i - row_col_count / 2;
        input_ref_cur_pre_table[i][2 * cols] = 1;
        input_ref_cur_pre_table[i][2 * cols + 1] = 1;
    }

    _input_ref_cur_pre_table = input_ref_cur_pre_table;

    for (int i = 0; i < _input_ref_cur_pre_table.size(); i++) {
        for (int j = 0; j < _input_ref_cur_pre_table[0].size(); j++) {
            std::cout << static_cast<int>(_input_ref_cur_pre_table[i][j]) << " ";
        }
        std::cout << std::endl;
    }
}


