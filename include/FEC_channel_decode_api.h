#pragma once

#ifdef _WIN32
#define FEC_CHANNEL_DECODE_EXPORT _declspec(dllexport)
#else
#ifdef __GUNC__
#define FEC_CHANNEL_DECODE_EXPORT __attribute__ ((visibility("default")))
#endif //  __GUNC__
#endif // _WIN32

#include "comm_type.h"

namespace FEC_CHANNEL_DECODE{
    class FEC_CHANNEL_DECODE_API{
    public:
        FEC_CHANNEL_DECODE_API(){};
        virtual ~FEC_CHANNEL_DECODE_API(){};

        virtual void init() = 0;
        virtual void encode() = 0;
        virtual void decode() = 0;

        /**
        * @brief set_polynomials                                set convolutional encode polynomials                        
        * @param poly_ptr                                       [input] polynomial of convolution encode (Octal)
        * @param poly_len                                       [input] length of polynomial
        * @return Result                                        execute result
        */
        virtual void set_polynomials(int* poly_ptr, int poly_len) = 0;


        /**
        * @brief set_constrain_length                           set convolutional encode polynomials
        * @param constrain_length                               [input] polynomial of convolution encode
        * @return Result                                        execute result
        */
        virtual void set_constrain_length(int constrain_length) = 0;
    };    
}

extern "C" FEC_CHANNEL_DECODE_EXPORT FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_API * CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type fec_obj_type);
extern "C" FEC_CHANNEL_DECODE_EXPORT void DeleteFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_API * &obj);
