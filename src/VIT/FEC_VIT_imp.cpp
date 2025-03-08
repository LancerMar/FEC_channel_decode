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

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::decode(char* code_data_ptr, int code_data_len, char*& decode_data_ptr, int& decode_data_len, Result result) {
    int output_once = _poly.size();
    int coded_data_grouping = code_data_len / output_once;
    int state_count = std::pow(2, (_constrain_length - 1));
    int state_half = state_count / 2;
    int* HammingDist_set = new int[_output_reference.size()];
    int input = 0;

    char* ptr_code_data_tmp = code_data_ptr;
    for (int i = 0; i < coded_data_grouping; i++) {
        // calculate the Hamming dist of coded data and all refrence
        cal_Hamming_dist_set(ptr_code_data_tmp, HammingDist_set);
        
        for (int state = 0; state < state_count; state++) {
            int pre_path_0_state = (state << 1);
            int pre_path_1_state = (state << 1) + 1;
            input = 0;
            if (state > std::pow(2, state_half)) {
                pre_path_0_state = pre_path_0_state - state_count;
                pre_path_1_state = pre_path_1_state - state_count;
                input = 1;
            }
        }


        // next output group
        ptr_code_data_tmp = ptr_code_data_tmp + output_once;
    }


    delete[] HammingDist_set;
}

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

    _output_reference = gen_binary_matrix_by_decimal(output_count);
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

    // print vector
    //for (int i = 0; i < _input_ref_cur_pre_table.size(); i++) {
    //    for (int j = 0; j < _input_ref_cur_pre_table[0].size(); j++) {
    //        std::cout << static_cast<int>(_input_ref_cur_pre_table[i][j]) << " ";
    //    }
    //    std::cout << std::endl;
    //}
}

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::gen_next_out_table() {
    
}


void FEC_CHANNEL_DECODE::FEC_VIT_IMP::cal_Hamming_dist_set(char* code_data_ptr, int*& HammingDist_set_ptr) {
    for (int i = 0; i < _output_reference.size(); i++) {
        int ham_dist_i = 0;
        for (int j = 0; j < _output_reference[i].size(); j++) {
            ham_dist_i = ham_dist_i + (code_data_ptr[j] + _output_reference[i][j]) % 2;
        }
        HammingDist_set_ptr[i] = ham_dist_i;
    }
}


