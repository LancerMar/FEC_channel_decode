clear all;
clc

%% =========== encode ================
info_len=1200;
poly = [133 171];
trellis = poly2trellis(7,poly);

data_info = randi([0 1],info_len,1);
data_info = [data_info(1:end-6).' [0 0 0 0 0 0]].';

coded_data = convenc(data_info,trellis);

%% ========== modulate - AWGN - demodulate ==========
snr=12;
[llr_data,hard_data] = qpsk_mod_demod_soft(coded_data,snr);
llr_path = "../../test_data/vit/vit_source_1200_conv213_7_133_171_snr_12_llr.dat";
file_write_double(llr_data,llr_path);

% hard decision decode
data_decodec_vit_hard = vitdec(coded_data,trellis,35,'trunc','hard');
biterr(data_info,data_decodec_vit_hard)
info_path = "../../test_data/vit/vit_source_1200_conv213_7_133_171_snr_12_llr_info.dat";
file_write_char(data_info,info_path);

%% ========= llr decode ================




