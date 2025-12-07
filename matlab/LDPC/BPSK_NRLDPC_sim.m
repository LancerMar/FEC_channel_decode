clear all;
clc

EbNodB = 4;
max_iteration = 8;

load Base_Matrics_5G\NR_1_0_16.txt
Base_matric = NR_1_0_16;
[rows_B,cols_B] = size(Base_matric);
z = 16;

% number of message bits
k = (cols_B - rows_B)*z;
% number of codeword bits
n = cols_B * z;
% channel factor
code_rate = k/n;
EbNo = 10^(EbNodB/10);
sigma = sqrt(1/(2*code_rate*EbNo));

% all zeros NR LDPC demo,to test if decode is OK
% in all linear code all-0 message refers all-0 codeword

% generate k-bit message
msg = zeros(1,k);
% encode
codeword = zeros(1,n);

% BPSK convert bit to symbol
symbols = 1-2*codeword;
% simple AWGN channel
received_word = symbols+sigma*randn(1,n);

% SISO iteration message-passing layered decode
LLR = received_word;
iteration = 0; % times of iteration
while iteration <max_iteration
    % each block layer is a layer
    % count of layer = rows of Base Matrix
    for layer = 1:rows_B
        
    end
    iteration = iteration+1;
end



