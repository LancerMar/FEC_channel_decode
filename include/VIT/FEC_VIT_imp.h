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
        void decode();

        void set_polynomials(int* poly_ptr, int poly_len);
        void set_constrain_length(int constrain_length);
    
    private:
        void gen_output_reference();
        void gen_input_ref_cur_pre();
    private:
        std::vector<std::vector<char>> _poly;
        int _constrain_length;

        std::vector<std::vector<char>> _output_reference;
        std::vector<std::vector<char>> _input_ref_cur_pre_table;
    };
}

