clear all;
clc

%% convelutional encoded data
info_len = 1200; % 信息向量长度
% trellis = poly2trellis(3,[7 5]);
trellis = poly2trellis(7,[133 171]);

data_info = randi([0 1],info_len,1);
data_info = [data_info(1:end-6).' [0 0 0 0 0 0]].';
coded_data = convenc(data_info,trellis);

tic;
data_decodec_vit = vitdec(coded_data,trellis,35,'trunc','hard');
toc;

biterr(data_decodec_vit,data_info)

% %polynomial = (3 [7,5])
% constrain_length = 3;
% poly_conv = [1 1 1; 1 0 1];

% polynomial = (7,[133 171]);
constrain_length = 7;
poly_conv = [1 0 1 1 0 1 1; 1 1 1 1 0 0 1];

rx_data = coded_data;

tic;
decode_data = viterbi_decoder(rx_data,constrain_length,poly_conv,[1 1]);
toc;

biterr(decode_data(1:end),data_info(1:end).')






