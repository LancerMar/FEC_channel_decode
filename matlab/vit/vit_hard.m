clear all;
clc

%% convelutional encoded data
info_len = 1200; % 信息向量长度
% trellis = poly2trellis(3,[7 5]);
trellis = poly2trellis(7,[133 171]);

data_info = randi([0 1],info_len,1);
data_info = [data_info(1:end-6).' [0 0 0 0 0 0]].';
file_write_char(data_info,"D:\work\project\channel_encode_decode\git_repo\FEC_channel_decode\test_data\vit_source_1200_171_7.dat");

% data_info = file_read_char("D:\work\project\channel_encode_decode\git_repo\FEC_channel_decode\test_data\vit_source_1200.dat");

coded_data = convenc(data_info,trellis);
file_write_char(coded_data,"D:\work\project\channel_encode_decode\git_repo\FEC_channel_decode\test_data\vit_source_1200_conv213_7_133_171.dat");

% coded_data = file_read_char("D:\work\project\channel_encode_decode\git_repo\FEC_channel_decode\test_data\vit_source_1200_conv213_5_7.dat");
% 
% add noise
snr = 6;
[~,hard_bit] = qpsk_mod_demod_soft(coded_data,snr);
biterr(hard_bit,coded_data)

% file_write_char(hard_bit,"D:\work\project\channel_encode_decode\git_repo\FEC_channel_decode\test_data\vit_source_1200_conv213_5_7_snr_6.dat");
file_write_char(hard_bit,"D:\work\project\channel_encode_decode\git_repo\FEC_channel_decode\test_data\vit_source_1200_conv213_7_133_171_snr_6.dat");

tic;
data_decodec_vit = vitdec(hard_bit,trellis,35,'trunc','hard');
toc;

biterr(data_decodec_vit,data_info)

% %polynomial = (3 [7,5])
% constrain_length = 3;
% poly_conv = [1 1 1; 1 0 1];

% polynomial = (7,[133 171]);
constrain_length = 7;
poly_conv = [1 0 1 1 0 1 1; 1 1 1 1 0 0 1];

rx_data = hard_bit;

tic;
decode_data = viterbi_decoder(rx_data,constrain_length,poly_conv,[1 1]);
toc;

biterr(decode_data(1:end),data_info(1:end).')






