#include <iostream>

#include "FEC_channel_decode_api.h"


int main() {
    auto app = CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type::FEC_channel_decode_test);
    DeleteFECChannelDecodeObj(app);


    return 0;
}