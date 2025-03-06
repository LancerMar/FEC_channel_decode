#include "FEC_channel_decode_api.h"

#include "FEC_channel_decode_test_imp.h"

FEC_CHANNEL_DECODE_EXPORT FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_API * CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type fec_obj_type){
    if(FEC_CHANNEL_DECODE::FEC_Obj_type::FEC_channel_decode_test == fec_obj_type){
        FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_API* app = new FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_TEST_IMP();
        return app;
    }
}

FEC_CHANNEL_DECODE_EXPORT void DeleteFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_API * &obj){
    if( nullptr != obj ){
        delete obj;
        obj = nullptr;
    }
} 
