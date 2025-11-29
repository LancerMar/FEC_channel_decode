clear all;
clc

EbNodB = 4;
msg = randi([0 1],3,1)';


% (6,3) simple sparse single parity check matrix
% generation matrix
G = [1 0 0 1 0 1;
     0 1 0 1 1 0;
     0 0 1 0 1 1];
code_word = mod(msg*G,2);

% col1 = msg(1)
% col2 = msg(2)
% col3 = msg(3)

% col4 = xor(col1,col2)
% col5 = xor(col2,col3)
% col6 = xor(col1,col3)

% single parity check matrix
H = [1 1 0 1 0 0;
     0 1 1 0 1 0;
     1 0 1 0 0 1];


[rows_H,cols_H] = size(H);
k = cols_H - rows_H;
n = cols_H;
code_rate = k/n;

% channel factor for BPSK modulation
EbNo = 10^(EbNodB/10);
sigma = sqrt(1/(2*code_rate*EbNo));

% BPSK convert bit to symbol
symbols = 1-2*code_word;
% simple AWGN channel
received_word = symbols+sigma*randn(1,n);

% init
LLR = received_word;
L_storage = LLR.*H;
L_sign = sign(L_storage);

max_iteration=3;
iteration = 1;
% iteration start
% row computation : SISO SPC decoder
for i = 1:rows_H
    min_sum_regs = L_storage(i,L_storage(i,:)~=0);
    L_sign_regs = L_sign(i,L_sign(i,:)~=0);

    [min1,idx_min1] = min(abs(min_sum_regs(1,:)));
    min_sum2_tmp = [min_sum_regs(1:(idx_min1-1)) min_sum_regs((idx_min1+1):end)];
    min2 = min(abs(min_sum2_tmp));

    S = prod(L_sign_regs);
    L_sign_regs = L_sign_regs*min1;
    L_sign_regs(idx_min1) = min2*sign(L_sign_regs(idx_min1));
    L_sign_regs = S*L_sign_regs;

    idx_L_sign_regs = 1;
    for j=1:cols_H
        if L_storage(i,j)~=0
            L_storage(i,j) = L_sign_regs(idx_L_sign_regs);
            idx_L_sign_regs=idx_L_sign_regs+1;
        end
    end
end





