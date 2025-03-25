clear all;
clc

info_len=1200;
info = randi([0 1],info_len,1);

polynomial = [1 1 1 1 0 0 1 ; 1 0 1 1 0 1 1];
constrain_len = 7;
trellis = poly2trellis(7,[171,133]);

code_data_1 = convenc(info,trellis).';

% todo: convolutional code encode
poly_1 = polynomial(1,:);
poly_2 = polynomial(2,:);

% init state all zeros
Regs=zeros(constrain_len,1).';

% encode
code_data_2 = [];
for i=1:1:length(info)
    input = info(i);
    Regs = [input Regs(1:end-1)];

    output_poly_1 = mod(sum(and(Regs,poly_1)),2);
    output_poly_2 = mod(sum(and(Regs,poly_2)),2);

    code_data_2=[code_data_2 output_poly_1];
    code_data_2=[code_data_2 output_poly_2];
end

biterr(code_data_2,code_data_1)




