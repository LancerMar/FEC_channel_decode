#include <iostream>
#include <vector>
#include <fstream>

#include "FEC_channel_decode_api.h"
#include "comm_type.h"

std::vector<char> load_char_file_to_vec(std::string file_path);


int main() {
    auto app = CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type::FEC_channel_decode_test);
    DeleteFECChannelDecodeObj(app);

    auto app_vit = CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type::VIT);

    std::string vit_coded_data_path = "../test_data/vit_source_1200_conv213_5_7.dat";
    auto coded_data = load_char_file_to_vec(vit_coded_data_path);

    std::vector<int> poly{ 7,5 };
    app_vit->set_polynomials(poly.data(), poly.size());
    app_vit->set_constrain_length(3);
    app_vit->init();

    char* decode_data;
    int decode_data_len = 0;
    FEC_CHANNEL_DECODE::Result result;
    app_vit->decode(coded_data.data(), coded_data.size(), decode_data, decode_data_len, result);


    DeleteFECChannelDecodeObj(app_vit);
    return 0;
}

std::vector<char> load_char_file_to_vec(std::string file_path) {
    std::ifstream file_in;
    file_in.open(file_path, std::ios::in | std::ios::binary);

    if (!file_in.is_open()) {
        std::cout << "read file " << file_path << "ERROR" << std::endl;
        return std::vector<char>();
    }

    char c;
    std::vector<char> result;
    while (!file_in.eof()) {
        c = file_in.get();
        //file operate for every T
        result.push_back(c);
    }

    file_in.close();

    return result;
}
