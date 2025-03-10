#include <iostream>
#include <vector>
#include <fstream>

#include "FEC_channel_decode_api.h"
#include "comm_type.h"
#include "joeytest.h"

std::vector<char> load_char_file_to_vec(std::string file_path);

void UTEST_VIT_decode(UTEST_result& result);

int main() {

    UTEST_result result_vit;

    UTEST_VIT_decode(result_vit);

    UNIT_TEST_SUM("Unit test for vit decode");
    result_vit.print();

    return 0;
}

void UTEST_VIT_decode(UTEST_result& result) {
    UNIT_TEST_HEADER("UTEST_CONV216_133_171_VIT_decode");
    result.total_case = result.total_case + 1;

    //prepare source data
    std::string vit_coded_data_path = "../test_data/vit/vit_source_1200_conv213_7_133_171_snr_6.dat";
    auto coded_data = load_char_file_to_vec(vit_coded_data_path);

    std::string source_data_path = "../test_data/vit/vit_source_1200_171_7.dat";
    auto source_data = load_char_file_to_vec(source_data_path);

    //start decode
    auto app_vit = CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type::VIT);

    // step 1: initialize
    std::vector<int> poly{ 133,171 };
    app_vit->set_polynomials(poly.data(), poly.size());
    app_vit->set_constrain_length(7);
    app_vit->init();

    // step 2: vit decode
    char* decode_data;
    int decode_data_len = 0;
    FEC_CHANNEL_DECODE::Result result_vit;

    clock_t start, stop;
    start = clock();
    app_vit->decode(coded_data.data(), coded_data.size(), decode_data, decode_data_len, result_vit);
    stop = clock();
    std::cout << "VIT decode time: " << double(stop - start) / CLOCKS_PER_SEC << std::endl;

    int count_diff_bit = 0;
    for (int i = 0; i < decode_data_len; i++) {
        if (decode_data[i] != source_data[i]) {
            count_diff_bit = count_diff_bit + 1;
        }
    }

    if (0 == count_diff_bit) {
        std::cout << "[PASS] : UTEST_CONV216_133_171_VIT_decode" << std::endl;
        result.success_case = result.success_case + 1;
    }
    else {
        std::cout << "[FAIL] : UTEST_CONV216_133_171_VIT_decode" << std::endl;
        result.failed_case = result.failed_case + 1;
    }

    DeleteFECChannelDecodeObj(app_vit);
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
