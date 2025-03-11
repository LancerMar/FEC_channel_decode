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
        void set_puncture_pattern(char* punc_pattern_ptr, int punc_pattern_len);
    
    private:
        // enumerate all output states
        void gen_output_reference();
        // enumerate all input refer to current state and pre state
        void gen_input_ref_cur_pre();
        // enumerate all output refer to all current state and all input 
        void gen_next_out_table();

        /**
        * @brief cal_Hamming_dist_set                           calculate Hamming distance set for every output groups
        * @param code_data_ptr                                  [input] pointer of coded data sequence position
        * @param HammingDist_set_ptr                            [output] pointer of HammingDist set
        */
        void cal_Hamming_dist_set(char* code_data_ptr, int* &HammingDist_set_ptr,char* punc_unit_ptr = nullptr);
        
        /**
        * @brief conv_encode_step                               calculate Hamming distance set for every output groups
        * @param state                                          [input] pointer of coded data sequence position
        * @param intput                                         [input] input number 1 or 0
        * @param next_state                                     [output] state after calculate
        * @param output                                         [output] output sequence
        */
        void conv_encode_step(std::vector<char> state, char input, std::vector<char> &next_state, std::vector<char> &output);
    private:
        std::vector<std::vector<char>> _poly;
        int _constrain_length;

        std::vector<std::vector<char>> _output_reference;
        std::vector<std::vector<char>> _input_ref_cur_pre_table;
        std::vector<std::vector<char>> next_out_table;
        
        std::vector<char> _punc_pattern_vec;                                                            // puncture pattern
        int _input_count_punc_pattern = 0;                                                              // input nums of 1 punc_pattern
        int _output_count_punc_pattern = 0;                                                             // output nums of 1 punc pattern 
    };
}

