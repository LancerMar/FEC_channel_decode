#pragma once
#include "FEC_channel_decode_api.h"

namespace FEC_CHANNEL_DECODE{
    class FEC_CHANNEL_DECODE_TEST_IMP : public FEC_CHANNEL_DECODE_API{
        FEC_CHANNEL_DECODE_TEST_IMP();
        virtual ~FEC_CHANNEL_DECODE_TEST_IMP();

        void init();
        void encode();
        void decode();
    };

}
