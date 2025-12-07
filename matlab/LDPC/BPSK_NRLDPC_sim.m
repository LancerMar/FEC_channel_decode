clear all;
clc

EbNodB = 4;
max_iteration = 8;

load Base_Matrics_5G\NR_1_0_16.txt
Base_matric = NR_1_0_16;
[rows_B,cols_B] = size(Base_matric);
z = 16; % expansion factor

%number of none -1 block in base matrix
none_minu1_B_len = sum(B(:)~=-1);
% storage of none -1 block matrix
% it is a linear storage, store every none -1 matrix
R = zeros(none_minu1_B_len,z);
% Row processing
% 1. L-R(L: total belief of received bits)
% 2. min-sum every row
% 3. (L-R)+min-sum-update-value

% temp regs for row values for min-sum(max 1s in one row)
% get the row_weights
row_weights = sum(Base_matric~=-1,2);
max_row_weights = max(row_weights);
temp_regs = zeros(max_row_weights,z);



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
row_idx = 0;
while iteration <max_iteration
    % each block layer is a layer
    % count of layer = rows of Base Matrix
    for layer = 1:rows_B
        temp_regs_idx = 0; % number of no -1 in each layer
        for col_idx = 1:cols_B
            if Base_matric(layer,col_idx)~=-1
                temp_regs_idx = temp_regs_idx+1;
                row_idx = row_idx + 1;
                % (L-R) 
                LLR((col_idx-1)*z+1:col_idx*z) = LLR((col_idx-1)*z+1:col_idx*z) - R(row_idx,:);
                % store in temp regs
                temp_regs(temp_regs_idx,:) = mul_sh(LLR((col_idx-1)*z+1:col_idx*z),Base_matric(layer,col_idx));
            end
        end
        % min-sum on temp regs in each layer
        % process temp_regs(1:,col_temp_reg_idx)
        for col_temp_reg_idx = 1:z 
            [min1,pos] = min(abs(temp_regs(1:temp_regs_idx,col_temp_reg_idx)));
            min2 = min(abs(temp_regs([1:pos-1 pos+1:temp_regs_idx],col_temp_reg_idx)));
            sign_reg_i = sign(temp_regs(1:temp_regs_idx,col_temp_reg_idx));
            parity = prod(sign_reg_
            temp_regs(1:temp_regs_idx,col_temp_reg_idx) = min1; % abs value
            temp_regs(pos,col_temp_reg_idx) = min2; % abs va
            temp_regs(1:temp_regs_idx,col_temp_reg_idx) = parity*sign_reg_i.*temp_regs(1:temp_regs_idx,col_temp_reg_idx);% sign them
        end

        % reset the pointer back
        row_idx = row_idx - temp_regs_idx;
        temp_regs_idx = 0;
        for col_idx = 1:cols_B
            if Base_matric()
                temp_regs_idx = temp_regs_idx+1;
                row_idx = row_idx + 1;
                % reverse the temp regs in R(linear storage)
                R(row_idx,:) = mul_sh(temp_regs(temp_regs_idx,:),z - Base_matric(layer,col_idx));
                % sum R : update the LLR
                LLR((col_idx-1)*z+1:col_idx*z) = LLR((col_idx-1)*z+1:col_idx*z) + R(row_idx,:);
            end
        end
    end
    % hard decision
    msg_recv = LLR(1:k)<0;
    iteration = iteration+1;
    
end



