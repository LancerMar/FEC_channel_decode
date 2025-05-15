clear all;
clc

%% =========== encode ================
info_len=1200;
poly = [133 171];
trellis = poly2trellis(7,poly);
punc_pattern = [1 1 0 1];

data_info = randi([0 1],info_len,1);
data_info = [data_info(1:end-6).' [0 0 0 0 0 0]].';
% file_write_char(data_info,"../../test_data/vit/vit_source_1200_conv216_133_171_punc1101_info.dat");

coded_data = convenc(data_info,trellis,punc_pattern);

snr=6;
[llr_data,hard_data] = qpsk_mod_demod_soft(coded_data,snr);
% file_write_double(llr_data,"../../test_data/vit/vit_source_1200_conv216_133_171_punc1101_encode_llr.dat");

constrain_length = 7;
poly_conv = [1 0 1 1 0 1 1; 1 1 1 1 0 0 1];

decode_data_func_test = viterbi_decoder_llr(llr_data,constrain_length,poly_conv,punc_pattern);
biterr(decode_data_func_test',data_info)


