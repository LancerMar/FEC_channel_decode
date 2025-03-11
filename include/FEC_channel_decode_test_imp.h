#pragma once
#include "FEC_channel_decode_api.h"

namespace FEC_CHANNEL_DECODE{
    class FEC_CHANNEL_DECODE_TEST_IMP : public FEC_CHANNEL_DECODE_API{
    public:
        FEC_CHANNEL_DECODE_TEST_IMP();
        virtual ~FEC_CHANNEL_DECODE_TEST_IMP();

        void init();
        void encode();
        void decode(char* code_data_ptr, int code_data_len, char*& decode_data_ptr, int& decode_data_len, Result result);

        void set_polynomials(int* poly_ptr, int poly_len);
        void set_constrain_length(int constrain_length);
        void set_puncture_pattern(char* punc_pattern_ptr, int punc_pattern_len);
    };

}
