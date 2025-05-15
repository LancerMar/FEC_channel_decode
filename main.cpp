#include <iostream>
#include <vector>
#include <fstream>

#include "FEC_channel_decode_api.h"
#include "comm_type.h"
#include "joeytest.h"

std::vector<char> load_char_file_to_vec(std::string file_path);
std::vector<double> load_double_file_to_vec(std::string file_path);

// Unit test for viterbi decoder
//hard decision demodulate data
void UTEST_VIT_decode(UTEST_result& result);
void UTEST_VIT_conv217_punc110110_decode(UTEST_result& result);
void UTEST_conv_encode(UTEST_result& result);

// soft decision demodulate data
void UTEST_VIT_LLR_decode(UTEST_result& result);
void UTEST_VIT_LLR_2_3_decode(UTEST_result& result);

int main() {

    UTEST_result result_vit;

    UTEST_VIT_decode(result_vit);
    UTEST_VIT_conv217_punc110110_decode(result_vit);
    UTEST_conv_encode(result_vit);

    UTEST_VIT_LLR_decode(result_vit);
    UTEST_VIT_LLR_2_3_decode(result_vit);


    UNIT_TEST_SUM("Unit test for vit decode");
    result_vit.print();

    getchar();
    return 0;
}

void UTEST_VIT_LLR_2_3_decode(UTEST_result& result) {
    UNIT_TEST_HEADER("UTEST_VIT_LLR_2_3_decode CONV216_133_171");
    result.total_case = result.total_case + 1;

    //prepare source data
    std::string vit_coded_data_path = "../test_data/vit/vit_source_1200_conv216_133_171_punc1101_encode_llr.dat";
    auto coded_data = load_double_file_to_vec(vit_coded_data_path);
    coded_data.erase(coded_data.end() - 1);

    std::string source_data_path = "../test_data/vit/vit_source_1200_conv216_133_171_punc1101_info.dat";
    auto source_data = load_char_file_to_vec(source_data_path);
    source_data.erase(source_data.end() - 1);

    //start decode
    auto app_vit = CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type::VIT);

    // step 1: initialize
    std::vector<char> punc_pattern{ 1,1,0,1 };
    std::vector<int> poly{ 133,171 };
    int constrain_length = 7;
    app_vit->set_polynomials(poly.data(), poly.size());
    app_vit->set_constrain_length(constrain_length);
    app_vit->set_puncture_pattern(punc_pattern.data(), punc_pattern.size());
    app_vit->init();

    // step 2: vit decode
    char* decode_data;
    int decode_data_len = 0;
    FEC_CHANNEL_DECODE::Result result_vit;

    clock_t start, stop;
    start = clock();
    app_vit->decode(coded_data.data(), coded_data.size(), decode_data, decode_data_len, result_vit);
    stop = clock();
    std::cout << "VIT llr decode time: " << double(stop - start) / CLOCKS_PER_SEC << std::endl;

    int count_diff_bit = 0;
    for (int i = 0; i < decode_data_len; i++) {
        if (decode_data[i] != source_data[i]) {
            count_diff_bit = count_diff_bit + 1;
        }
    }

    if (0 == count_diff_bit) {
        std::cout << "[PASS] : UTEST_VIT_LLR_2_3_decode CONV216_133_171" << std::endl;
        result.success_case = result.success_case + 1;
    }
    else {
        std::cout << "[FAIL] : UTEST_VIT_LLR_2_3_decode CONV216_133_171" << std::endl;
        result.failed_case = result.failed_case + 1;
    }

    DeleteFECChannelDecodeObj(app_vit);
}

void UTEST_VIT_LLR_decode(UTEST_result& result) {
    UNIT_TEST_HEADER("UTEST_VIT_LLR_decode CONV216_133_171");
    result.total_case = result.total_case + 1;

    //prepare source data
    std::string vit_coded_data_path = "../test_data/vit/vit_source_1200_conv213_7_133_171_snr_12_llr.dat";
    auto coded_data = load_double_file_to_vec(vit_coded_data_path);
    coded_data.erase(coded_data.end() - 1);

    std::string source_data_path = "../test_data/vit/vit_source_1200_conv213_7_133_171_snr_12_llr_info.dat";
    auto source_data = load_char_file_to_vec(source_data_path);
    source_data.erase(source_data.end() - 1);

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
    std::cout << "VIT llr decode time: " << double(stop - start) / CLOCKS_PER_SEC << std::endl;

    int count_diff_bit = 0;
    for (int i = 0; i < decode_data_len; i++) {
        if (decode_data[i] != source_data[i]) {
            count_diff_bit = count_diff_bit + 1;
        }
    }

    if (0 == count_diff_bit) {
        std::cout << "[PASS] : UTEST_VIT_LLR_decode CONV216_133_171" << std::endl;
        result.success_case = result.success_case + 1;
    }
    else {
        std::cout << "[FAIL] : UTEST_VIT_LLR_decode CONV216_133_171" << std::endl;
        result.failed_case = result.failed_case + 1;
    }
    
    DeleteFECChannelDecodeObj(app_vit);

}

void UTEST_conv_encode(UTEST_result& result) {
    UNIT_TEST_HEADER("UTEST_conv_encode");
    result.total_case = result.total_case + 1;

    //prepare source data
    std::string source_data_path = "../test_data/vit/vit_source_1200_171_7.dat";
    auto source_data = load_char_file_to_vec(source_data_path);
    source_data.erase(source_data.end()-1);

    //start encode
    auto app_vit = CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type::VIT);

    // step 1: initialize
    std::vector<int> poly{ 133,171 };
    app_vit->set_polynomials(poly.data(), poly.size());
    app_vit->set_constrain_length(7);
    app_vit->init();

    // step 2: conv encode
    char* encoded_data_ptr;
    int encoded_data_len;
    FEC_CHANNEL_DECODE::Result result_conv;
    app_vit->encode(source_data.data(),source_data.size(), encoded_data_ptr, encoded_data_len, result_conv);

    char* decode_data;
    int decode_data_len = 0;
    FEC_CHANNEL_DECODE::Result result_vit;
    app_vit->decode(encoded_data_ptr, encoded_data_len, decode_data, decode_data_len, result_vit);

    int count_diff_bit = 0;
    for (int i = 0; i < decode_data_len; i++) {
        if (decode_data[i] != source_data[i]) {
            count_diff_bit = count_diff_bit + 1;
        }
    }

    if (0 == count_diff_bit) {
        std::cout << "[PASS] : UTEST_conv_encode" << std::endl;
        result.success_case = result.success_case + 1;
    }
    else {
        std::cout << "[FAIL] : UTEST_conv_encode" << std::endl;
        result.failed_case = result.failed_case + 1;
    }

    DeleteFECChannelDecodeObj(app_vit);
}

void UTEST_VIT_conv217_punc110110_decode(UTEST_result& result) {
    UNIT_TEST_HEADER("UTEST_VIT_conv217_punc110110_decode");
    result.total_case = result.total_case + 1;

    //prepare source data
    std::string vit_coded_data_path = "../test_data/vit/vit_source_1200_conv217_punc_110110_encode_snr_8.dat";
    auto coded_data = load_char_file_to_vec(vit_coded_data_path);

    std::string source_data_path = "../test_data/vit/vit_source_1200_conv217_punc_110110.dat";
    auto source_data = load_char_file_to_vec(source_data_path);

    //start decode
    auto app_vit = CreateFECChannelDecodeObj(FEC_CHANNEL_DECODE::FEC_Obj_type::VIT);
    std::vector<int> poly{ 133,171 };
    int constrain_length = 7;
    std::vector<char> punc_pattern{ 1,1,0,1,1,0 };
    app_vit->set_polynomials(poly.data(), poly.size());
    app_vit->set_constrain_length(constrain_length);
    app_vit->set_puncture_pattern(punc_pattern.data(), punc_pattern.size());
    app_vit->init();

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
        std::cout << "[PASS] : UTEST_VIT_conv217_punc110110_decode" << std::endl;
        result.success_case = result.success_case + 1;
    }
    else {
        std::cout << "[FAIL] : UTEST_VIT_conv217_punc110110_decode" << std::endl;
        result.failed_case = result.failed_case + 1;
    }

    DeleteFECChannelDecodeObj(app_vit);
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


std::vector<double> load_double_file_to_vec(std::string file_path) {
    std::ifstream file_in_test;
    file_in_test.open(file_path, std::ios::binary);
    if (!file_in_test.is_open()) {
        std::cout << "read file " << file_path << "ERROR" << std::endl;
        return std::vector<double>();
    }

    double value_tmp = 0;
    std::vector<double> value_arry;
    while (!file_in_test.eof()) {
        file_in_test.read((char*)&value_tmp, sizeof(double));
        value_arry.push_back(value_tmp);
    }

    return value_arry;
}
