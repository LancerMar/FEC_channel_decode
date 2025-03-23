#include "FEC_VIT_imp.h"

#include <iostream>
#include <vector>

#include "comm_tool.h"

FEC_CHANNEL_DECODE::FEC_VIT_IMP::FEC_VIT_IMP() {
    //std::cout << "FEC_VIT_IMP()" << std::endl;
}

FEC_CHANNEL_DECODE::FEC_VIT_IMP::~FEC_VIT_IMP() {
    //std::cout << "~FEC_VIT_IMP()" << std::endl;
}

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::init() {
    //step 1: generate output reference table
    gen_output_reference();

    //step 2: generate input reference previous state and current state table
    gen_input_ref_cur_pre();

    //step 3: generate next out table
    gen_next_out_table();
}

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::encode(char* source_data_ptr, int source_data_len, char*& encode_data_ptr, int& encode_data_len, Result& result) {
    // step 1: create registers
    std::vector<char> regs(_constrain_length, 0);

    // step 2: input 1 bit to registers
    std::vector<char> state = regs; 
    std::vector<char> encode_data;
    for (int i = 0; i < source_data_len; i++) {
        std::vector<char> next_state;
        std::vector<char> output_step;

        conv_encode_step(state, source_data_ptr[i], next_state, output_step);
        state = next_state;
        encode_data.insert(encode_data
    }
}

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::decode(char* code_data_ptr, int code_data_len, char*& decode_data_ptr, int& decode_data_len, Result& result) {
    
    char* ptr_code_data_tmp = code_data_ptr;
    char* punc_sequence_ptr = nullptr;
    std::vector<char> punc_all;
    std::vector<char> coded_data_no_punc;
    int _input_data_len = code_data_len;

    // generate puncture pattern
    if (0 != _punc_pattern_vec.size()) {
        int code_frame_no_punc_size = _poly.size() * (code_data_len * _input_count_punc_pattern) / _output_count_punc_pattern;
        _input_data_len = code_frame_no_punc_size;
        int cycle = code_frame_no_punc_size / _punc_pattern_vec.size();

        
        for (int i = 0; i < cycle; i++) {
            punc_all.insert(punc_all.end(), _punc_pattern_vec.begin(), _punc_pattern_vec.end());
        }

        //expand the encoded data
        std::vector<char> coded_data_no_punc_tmp(punc_all.size(),0);
        coded_data_no_punc = coded_data_no_punc_tmp;
        int coded_data_index = 0;
        for (int i = 0; i < punc_all.size(); i++) {
            if (1 == punc_all[i]) {
                coded_data_no_punc[i] = code_data_ptr[coded_data_index];
                coded_data_index = coded_data_index + 1;
            }
        }
        ptr_code_data_tmp = coded_data_no_punc.data();
        punc_sequence_ptr = punc_all.data();
    }
    
    int output_once = _poly.size();
    int coded_data_grouping = _input_data_len /output_once;
    int state_count = std::pow(2, (_constrain_length - 1));
    int state_half = state_count / 2;
    int* HammingDist_set = new int[_output_reference.size()];
    
    int* path_matric = new int[state_count];
    for (int i = 0; i < state_count; i++) {
        path_matric[i] = 0;
    }
    int* path_matric_temp = new int[state_count];
    for (int i = 0; i < state_count; i++) {
        path_matric_temp[i] = 0;
    }
    std::vector<std::vector<int>> suvive_path(state_count, std::vector<int>(coded_data_grouping,0));

    int next_out_0;
    int next_out_1;
    int branch_matric_0;
    int branch_matric_1;

    int idx = 0;

    int input = 0;
    
    for (int i = 0; i < coded_data_grouping; i++) {
        // calculate the Hamming dist of coded data and all refrence
        cal_Hamming_dist_set(ptr_code_data_tmp, HammingDist_set, punc_sequence_ptr);
        
        for (int state = 0; state < state_count; state++) {
            int pre_path_0_state = (state << 1);
            int pre_path_1_state = (state << 1) + 1;
            input = 0;
            if (state > state_half - 1) {
                pre_path_0_state = pre_path_0_state - state_count;
                pre_path_1_state = pre_path_1_state - state_count;
                input = 1;
            }

            next_out_0 = next_out_table[pre_path_0_state][input];
            next_out_1 = next_out_table[pre_path_1_state][input];
            branch_matric_0 = path_matric[pre_path_0_state] + HammingDist_set[next_out_0];
            branch_matric_1 = path_matric[pre_path_1_state] + HammingDist_set[next_out_1];

            if (branch_matric_1 < branch_matric_0) {
                idx = 1;
                path_matric_temp[state] = branch_matric_1;
            }else{
                idx = 0;
                path_matric_temp[state] = branch_matric_0;
            }

            if (state < state_half) {
                suvive_path[state][i] = idx + state * 2;
            }
            else if (state >= state_half)
            {
                suvive_path[state][i] = idx + (state - state_half) * 2;
            }
        }
        
        memcpy(path_matric, path_matric_temp, state_count * sizeof(int));

        // next output group
        ptr_code_data_tmp = ptr_code_data_tmp + output_once;
        // next puncture unit for output
        if (nullptr != punc_sequence_ptr) {
            punc_sequence_ptr = punc_sequence_ptr + output_once;
        }
    }


    //trace back
    int current_state = 0; // initial state for last bit
    decode_data_ptr = new char[coded_data_grouping]; 
    decode_data_len = coded_data_grouping;

    int pre_state = 0;
    for (int i = coded_data_grouping - 1; i > -1; i--) {
        pre_state = suvive_path[current_state][i];
        decode_data_ptr[i] = _input_ref_cur_pre_table[current_state][pre_state];
        current_state = pre_state;
    }



    delete[] HammingDist_set;
    delete[] path_matric;
    delete[] path_matric_temp;
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

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::set_puncture_pattern(char* punc_pattern_ptr, int punc_pattern_len) {
    std::vector<char> punc_pattern_vec(punc_pattern_ptr, punc_pattern_ptr + punc_pattern_len);
    _punc_pattern_vec = punc_pattern_vec;
    
    int input_count_punc_pattern = punc_pattern_vec.size() / _poly.size();
    int output_count_punc_pattern = 0;
    for (int i = 0; i < punc_pattern_vec.size(); i++) {
        if (1 == punc_pattern_vec[i]) {
            output_count_punc_pattern = output_count_punc_pattern + 1;
        }
    }

    _input_count_punc_pattern = input_count_punc_pattern;
    _output_count_punc_pattern = output_count_punc_pattern;

}

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
    int regs_count = std::pow(2, (_constrain_length - 1));
    auto ref_regs_status_all = gen_binary_matrix_by_decimal(regs_count);

    // input 0/1 , the output for every status 
    for (int i = 0; i < ref_regs_status_all.size(); i++) {
        
        std::vector<char> next_state;
        std::vector<char> next_out;
        std::vector<char> next_out_table_tmp;

        // input 0 
        conv_encode_step(ref_regs_status_all[i], 0, next_state, next_out);
        next_out_table_tmp.push_back(binary_2_decimal(next_out));
        next_out.clear();

        // input 1
        conv_encode_step(ref_regs_status_all[i], 1, next_state, next_out);
        next_out_table_tmp.push_back(binary_2_decimal(next_out));

        next_out_table.push_back(next_out_table_tmp);
    }

}


void FEC_CHANNEL_DECODE::FEC_VIT_IMP::cal_Hamming_dist_set(char* code_data_ptr, int*& HammingDist_set_ptr,char* punc_unit_ptr) {
    char* punc_unit_temp_ptr = punc_unit_ptr;
    for (int i = 0; i < _output_reference.size(); i++) {
        int ham_dist_i = 0;
        for (int j = 0; j < _output_reference[i].size(); j++) {
            if (nullptr == punc_unit_temp_ptr) {
                ham_dist_i = ham_dist_i +  (code_data_ptr[j] + _output_reference[i][j]) % 2;
            }
            else {
                ham_dist_i = ham_dist_i + static_cast<int>(punc_unit_temp_ptr[j] & (code_data_ptr[j] + _output_reference[i][j])) % 2;
            }
        }
        HammingDist_set_ptr[i] = ham_dist_i;
    }
}


void FEC_CHANNEL_DECODE::FEC_VIT_IMP::conv_encode_step(std::vector<char> state, char input, std::vector<char>& next_state, std::vector<char>& output) {
    int output_count = _poly.size();
    std::vector<char> regs;
    
    // gen registors set
    regs.push_back(input);
    for (int i = 0; i < state.size(); i++) {
        regs.push_back(state[i]);
    }

    // 计算每一个多项式对应的输出
    for (int i = 0; i < output_count; i++) {
        int output_tmp = 0;
        for(int j = 0;j< _poly[i].size(); j++) {
            if (0 != _poly[i][j]) {
                output_tmp = output_tmp + regs[j];
            }
        }

        output.push_back(output_tmp % 2);
    }

    regs.pop_back();
    next_state = regs;
}

