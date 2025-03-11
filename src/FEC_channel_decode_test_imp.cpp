#include "FEC_channel_decode_test_imp.h"

#include <iostream>

FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_TEST_IMP::FEC_CHANNEL_DECODE_TEST_IMP(){
    std::cout << "FEC_CHANNEL_DECODE_TEST_IMP()" << std::endl;
}

FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_TEST_IMP::~FEC_CHANNEL_DECODE_TEST_IMP(){
    std::cout << "~FEC_CHANNEL_DECODE_TEST_IMP()" << std::endl;
}

void FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_TEST_IMP::init(){

}
        
void FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_TEST_IMP::encode(){

}
        
void FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_TEST_IMP::decode(char* code_data_ptr, int code_data_len, char*& decode_data_ptr, int& decode_data_len, Result result) {}


void FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_TEST_IMP::set_polynomials(int* poly_ptr, int poly_len) {}
void FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_TEST_IMP::set_constrain_length(int constrain_length) {}
void FEC_CHANNEL_DECODE::FEC_CHANNEL_DECODE_TEST_IMP::set_puncture_pattern(char* punc_pattern_ptr, int punc_pattern_len) {}
