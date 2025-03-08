#include <iostream>
#include <vector>

#include "FEC_channel_decode_api.h"
#include "comm_type.h"

int main() {
    auto app = CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type::FEC_channel_decode_test);
    DeleteFECChannelDecodeObj(app);

    auto app_vit = CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type::VIT);

    std::vector<int> poly{ 5,7 };
    app_vit->set_polynomials(poly.data(), poly.size());
    app_vit->set_constrain_length(3);
    app_vit->init();

    std::vector<char> coded_data = { 1,0,1,1 };
    char* decode_data;
    int decode_data_len = 0;
    FEC_CHANNEL_DECODE::Result result;
    app_vit->decode(coded_data.data(), coded_data.size(), decode_data, decode_data_len, result);


    DeleteFECChannelDecodeObj(app_vit);
    return 0;
}