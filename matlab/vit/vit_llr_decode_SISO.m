function decode_data = vit_llr_decode_SISO(rx_llr,constrain_length,poly,punc_pattern)
%% proc input data by puncture pattern
if 0 ~= length(punc_pattern)
    punc_pattern_1 = sum(punc_pattern);
    depunc_group = floor(length(rx_llr)/punc_pattern_1);
    punc_seq = [];
    for i=1:1:depunc_group
        punc_seq = [punc_seq punc_pattern];
    end

    for i=1:1:mod(length(rx_llr),punc_pattern_1)
        punc_seq = [punc_seq punc_pattern(i)];
    end

    rx_unpunc_seq = zeros(length(punc_seq),1);
    rx_llr_idx = 1;
    for i =1:1:length(punc_seq)
        if punc_seq(i) == 1
            rx_unpunc_seq(i) = rx_llr(rx_llr_idx);
            rx_llr_idx = rx_llr_idx+1;
        end
    end
    rx_llr = rx_unpunc_seq;
end

%% generate output reference (refer to the length of polynomial)
[rows,cols] = size(poly);
ref_hard = zeros((2^rows),rows);
for i = 1:(2^rows)
    ref_tmp = dec2bin(i-1) - '0';
    ref_hard(i,:) = padarray(ref_tmp,[0 rows - length(ref_tmp)],0,'pre');
end

ref_llr = 2*ref_hard-1;

%% generate input refer to current state and previous state
identity_matrix = eye(2^(constrain_length-2));
repeat_matrix = [1 1];
input_ref_cur_pre = [zeros(2^(constrain_length-2),2^(constrain_length-1));kron(identity_matrix,repeat_matrix)];

next_out_table = gen_next_out_table(poly);

%% survive path caculate
len_rx_data = length(rx_llr);
path_matric = zeros((2^(cols-1)),1); % 寄存器状态个数
path_matric_temp = zeros((2^(cols-1)),1); % 寄存器状态个数
suvive_single_path = zeros((2^(cols-1)),1);
suvive_path = zeros((2^(cols-1)),len_rx_data/rows);

branch_diff_matric = zeros((2^(cols-1)),len_rx_data/rows);
branch_diff_matric_tmp = zeros((2^(cols-1)),1);

path_matric(1,1) = 0; % init state should calculate from 0

% unit set of output
for i = 1:1:(len_rx_data/rows)
    rx_data_unit = rx_llr(rows*(i-1)+1:rows*i).';

    % calculate the euclidean distance between rx data and [00 01 10 11]
    rx_data_unit_extend_dim = kron(ones(2^rows,1),rx_data_unit);
    euclidean_dist = sum(rx_data_unit_extend_dim.*ref_llr,2); % row sum

    % 计算当前输出对应的每个状态的分支度量，路径度量，与幸存路径
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
        branch_matric_0 = path_matric(pre_path_0_state,1) + euclidean_dist(next_out_0,1);
        branch_matric_1 = path_matric(pre_path_1_state,1) + euclidean_dist(next_out_1,1);
        [branch_matric_min,idx] = min([branch_matric_0,branch_matric_1]);

        path_matric_temp(state,1) = branch_matric_min;
        branch_diff_matric_tmp(state,1) = abs(branch_matric_0 - branch_matric_1);

        if state < 2^(constrain_length-2)+1
            suvive_single_path(state,1) = idx + (state - 1)*2;
        end

        if state > 2^(constrain_length-2)
            suvive_single_path(state,1) = idx + (state - (2^(constrain_length-2)+1))*2;
        end
    end
    path_matric = path_matric_temp;
    suvive_path(:,i) = suvive_single_path;
    branch_diff_matric(:,i) = branch_diff_matric_tmp;
end

% trace back(SIHO)
current_state = 1; % 最后一位的初始状态
refer_state = [];
refer_state = [refer_state current_state];
input = zeros(1,len_rx_data/rows); % 译码结果

for i= (len_rx_data/rows):-1:1
    pre_state = suvive_path(current_state,i);
    input(i) = input_ref_cur_pre(current_state,pre_state);
    refer_state = [refer_state pre_state];
    current_state = pre_state;
end

decode_data = input;


% check delta delay for every bit to find minimum
% llr_output(SISO)
delta = 8;
input_llr = zeros(1,len_rx_data/rows); % 译码结果

decode_Data_len = (len_rx_data/rows);
for t= 1:1:(len_rx_data/rows)
    llr_temp = Inf;
    for i=0:1:delta
        if t+i<decode_Data_len
            % 找到当前数据后第i个输入与i+1个状态作为初始状态
            bit_input = 1 - input(t+i); % 翻转输入bit以找到最小竞争路径
            cur_state = refer_state(t+i+1);

            % 回溯到t时刻
            for j=i-1:-1:0
                pre_state = suvive_path(cur_state,t+i);
                bit_input = input_ref_cur_pre(cur_state,pre_state);
                cur_state = pre_state;
            end
            if pre_state~=input(t)
                llr_temp = min(llr_temp,branch_diff_matric(refer_state(t+i+1),t+i+1));
            end
        end
    end

    input_llr(t) = (2*input(t) - 1) * llr_temp;
end
decode_data = input_llr;
end