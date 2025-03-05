function decode_data = viterbi_decoder(rx_data,constrain_length,poly,punc_pattern)
%viterbi_decoder viterbi decode algorithm

%[input] rx_data : received data sequence
%[input] constrain_length : constrain length
%[input] poly : polynomial for conv encode
%[input] punc_pattern : puncture pattern

%[output] punc_pattern : puncture pattern

%% generate output reference (refer to the length of polynomial)
[rows,cols] = size(poly);
ref_hard = zeros((2^rows),rows);
for i = 1:(2^rows)
    ref_tmp = dec2bin(i-1) - '0';
    ref_hard(i,:) = padarray(ref_tmp,[0 rows - length(ref_tmp)],0,'pre');
end


%% generate input refer to current state and previous state 
identity_matrix = eye(2^(constrain_length-2));
repeat_matrix = [1 1];
input_ref_cur_pre = [zeros(2^(constrain_length-2),2^(constrain_length-1));kron(identity_matrix,repeat_matrix)];
next_out_table = gen_next_out_table(poly);


%% survive path caculate
len_rx_data = length(rx_data);
path_matric = zeros((2^(cols-1)),1); % 寄存器状态个数
path_matric_temp = zeros((2^(cols-1)),1); % 寄存器状态个数
suvive_single_path = zeros((2^(cols-1)),1);
suvive_path = zeros((2^(cols-1)),len_rx_data/rows);

path_matric(1,1) = 0; % 初态从 0 开始计算
% 依次获取单元个数的输出，进行度量计算（根据多项式个数确定 N:一个clock的输出个数）
for i = 1:1:(len_rx_data/rows)
    rx_data_unit = rx_data(rows*(i-1)+1:rows*i).';
    
    % 计算当前接收到的值与每个参考点的汉明距离
    rx_data_unit_extend_dim = kron(ones(2^rows,1),rx_data_unit);
    HammingDist_set = sum(xor(rx_data_unit_extend_dim,ref_hard),2);
    
    % 计算当前输出对应的每个状态的分支度量，路径度量，与幸存路径（工程实现中先右移一位在左移一位即可完成状态调整）
    for state = 1:1:2^(constrain_length-1)
        pre_path_0_state = bitshift(state-1,1) + 1; % 左移一位补 0
        pre_path_1_state = bitshift(state-1,1) + 1 + 1; % 左移一位补 1
        
        input = 0;
        if state > 2^(constrain_length-2)
            pre_path_0_state = pre_path_0_state - 2^(constrain_length-1);
            pre_path_1_state = pre_path_1_state - 2^(constrain_length-1);
            input = 1;
        end
        
        next_out_0 = next_out_table(pre_path_0_state,input+1) + 1;
        next_out_1 = next_out_table(pre_path_1_state,input+1) + 1;
        branch_matric_0 = path_matric(pre_path_0_state,1) + HammingDist_set(next_out_0,1); 
        branch_matric_1 = path_matric(pre_path_1_state,1) + HammingDist_set(next_out_1,1);
        [branch_matric_min,idx] = min([branch_matric_0,branch_matric_1]); 
        
        path_matric_temp(state,1) = branch_matric_min;
%         suvive_single_path(state,1) = idx;
%         if 0 == mod(state,2)
%             suvive_single_path(state,1) = idx + 2;
%         end

        if state < 2^(constrain_length-2)+1
            suvive_single_path(state,1) = idx + (state - 1)*2;
        end
        
        if state > 2^(constrain_length-2)
            suvive_single_path(state,1) = idx + (state - (2^(constrain_length-2)+1))*2;
        end

        
    end
    path_matric = path_matric_temp;
    suvive_path(:,i) = suvive_single_path;
end

% trace back
current_state = 1; % 最后一位的初始状态
input = zeros(1,len_rx_data/rows); % 译码结果

for i= (len_rx_data/rows):-1:1
    pre_state = suvive_path(current_state,i);
    input(i) = input_ref_cur_pre(current_state,pre_state);
    current_state = pre_state;
end

decode_data = input;

end