clear all;
clc


load Base_Matrics_5G\NR_1_0_16.txt
Base_matric = NR_1_0_16;
[rows_B,cols_B] = size(Base_matric);
z = 16;

% number of message bits
k = (cols_B - rows_B)*z;
% number of codeword bits
n = cols_B * z;




