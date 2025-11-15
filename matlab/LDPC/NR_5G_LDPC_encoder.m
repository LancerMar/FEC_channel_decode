function out = NR_5G_LDPC_encoder(Base_matric,expansion_factor,msg)
%Base_matric: base matrix
%expansion_factor: expansion factor
%msg: information bits(length = (Base_matric(cols) - Base_matric(rows)) * expansion_factor) 
%out: code words

[rows,cols] = size(Base_matric);
msg_length = (cols - rows) * expansion_factor;

code_word = zeros(1,cols*expansion_factor);
% assign message word in code word
code_word(1:msg_length) = msg;

%% double-diagonal encode (only 4 rows here) 
% hint: this step is only for encode LDPC code in 5G NR standards 

% add first 4 rows of H*C^T
result_4_rows_sum = zeros(1,expansion_factor);
for row_idx = 1:4
    for msg_block_idx = 1:(cols - rows)
        % get message code block which length equals to expasion_factor
        msg_block = msg((msg_block_idx-1)*expansion_factor +1:msg_block_idx*expansion_factor);
        result_4_rows_sum = mod(result_4_rows_sum + mul_sh(msg_block,Base_matric(row_idx,msg_block_idx)),2);
    end
end

% find the Ik of P1 refers to Basic matrix
if Base_matric(2,cols-rows+1) == -1
    P1_Ik = Base_matric(3,cols-rows+1);
else
    P1_Ik = Base_matric(2,cols-rows+1);
end

% P1 = shift right (expansion_factor - k) steps of P1*Ik
P1 = mul_sh(result_4_rows_sum,expansion_factor-P1_Ik);
% add P1 in code_word
code_word((cols-rows)*expansion_factor+1:(cols-rows+1)*expansion_factor) = P1;

% find P2 P3 P4 
for row_idx = [1 2 3]
    Pi = zeros(1,expansion_factor);
    % in identity matrix, after Base_matric(cols-rows+row_idx) is all zeros in each row 
    for code_word_block_idx = 1:(cols-rows+row_idx)
        code_word_block = code_word((code_word_block_idx-1)*expansion_factor+1:code_word_block_idx*expansion_factor);
        % Ik = 0(identity matrix) => Pi*Ik = Pi
        Pi = mod(Pi + mul_sh(code_word_block,Base_matric(row_idx,code_word_block_idx)),2);
    end
    % add Pi in code word
    code_word((cols-rows+row_idx)*expansion_factor+1:(cols-rows+row_idx+1)*expansion_factor) = Pi;
end

%% Remaining parity calculation
% Remaining parity refer to the identity matrix part of Base matrix
% HINT: we do not have to calculate P4 in row4(already did that in row3).

for row_idx = 5:rows
    Pi = zeros(1,expansion_factor);
    % identity matrix; only need to sum the first (cols-rows+4) columns
    for code_word_block_idx = 1:(cols-rows+4)
        code_word_block = code_word((code_word_block_idx-1)*expansion_factor+1:code_word_block_idx*expansion_factor);
        Pi = mod(Pi + mul_sh(code_word_block,Base_matric(row_idx,code_word_block_idx)),2);
    end
    code_word((cols-rows+row_idx-1)*expansion_factor+1:(cols-rows+row_idx)*expansion_factor) = Pi;
end

out = code_word;

end