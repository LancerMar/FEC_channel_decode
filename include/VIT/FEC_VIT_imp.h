#pragma once
# include "FEC_channel_decode_api.h"

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
        int* _poly_ptr;
        int _poly_len;
        int _constrain_length;
    };
}

