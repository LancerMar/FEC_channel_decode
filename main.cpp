#include <iostream>
#include <vector>

#include "FEC_channel_decode_api.h"
#include "comm_type.h"

int main() {
    auto app = CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type::FEC_channel_decode_test);
    DeleteFECChannelDecodeObj(app);


    return 0;
}