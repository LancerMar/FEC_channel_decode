#pragma once

#define FEC_CHANNEL_DECODE_EXPORT
#ifdef FEC_CHANNEL_DECODE_EXPORT
#ifdef _WIN32
#define FEC_CHANNEL_DECODE_EXPORT _declspec(dllexport)
#else
#ifdef __GUNC__
#define FEC_CHANNEL_DECODE_EXPORT __attribute__ ((visibility("default")))
#endif //  __GUNC__
#endif // _WIN32
#endif // FEC_CHANNEL_DECODE_EXPORT

#include "comm_type.h"

namespace FEC_CHANNEL_DECODE{
    class FEC_CHANNEL_DECODE_API{
    public:
        FEC_CHANNEL_DECODE_API(){};
        virtual ~FEC_CHANNEL_DECODE_API(){};

        virtual void init() = 0;
        virtual void encode() = 0;
        virtual void decode() = 0;
    };    
}

extern "C" FEC_CHANNEL_DECODE_EXPORT FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_API * CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type fec_obj_type);
extern "C" void DeleteFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_API * &obj);
