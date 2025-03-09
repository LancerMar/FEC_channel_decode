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

    std::vector<std::vector<int>> poly_t{ {1,2,3},{4,5,6} };
    int a = poly_t[0].size();

    app_vit->init();

    DeleteFECChannelDecodeObj(app_vit);
    return 0;
}