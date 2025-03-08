#pragma once
# include "FEC_channel_decode_api.h"

#include <vector>

namespace FEC_CHANNEL_DECODE {
    class FEC_VIT_IMP :public FEC_CHANNEL_DECODE_API {
    public:
        FEC_VIT_IMP();
        ~FEC_VIT_IMP();

        void init();
        void encode();

        void decode(char* code_data_ptr, int code_data_len, char*& decode_data_ptr, int& decode_data_len, Result result);

        void set_polynomials(int* poly_ptr, int poly_len);
        void set_constrain_length(int constrain_length);
    
    private:
        // enumerate all output states
        void gen_output_reference();
        // enumerate all input refer to current state and pre state
        void gen_input_ref_cur_pre();
        // enumerate all output refer to all current state and all input 
        void gen_next_out_table();

        // calculate Hamming distance set for every output groups
        void cal_Hamming_dist_set(char* code_data_ptr, int* &HammingDist_set_ptr);
    private:
        std::vector<std::vector<char>> _poly;
        int _constrain_length;

        std::vector<std::vector<char>> _output_reference;
        std::vector<std::vector<char>> _input_ref_cur_pre_table;
    };
}

