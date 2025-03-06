#include "FEC_VIT_imp.h"

#include <iostream>
#include <vector>

#include "comm_tool.h"

FEC_CHANNEL_DECODE::FEC_VIT_IMP::FEC_VIT_IMP() {
    std::cout << "FEC_VIT_IMP()" << std::endl;
}

FEC_CHANNEL_DECODE::FEC_VIT_IMP::~FEC_VIT_IMP() {
    std::cout << "~FEC_VIT_IMP()" << std::endl;
}

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::init() {}

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::encode() {}

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::decode() {}


void FEC_CHANNEL_DECODE::FEC_VIT_IMP::set_polynomials(int* poly, int poly_len) {
    std::vector<int> poly_vec(poly, poly+poly_len);


};

void FEC_CHANNEL_DECODE::FEC_VIT_IMP::set_constrain_length(int constrain_length) {

};


