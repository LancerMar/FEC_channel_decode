clear all;
clc

%% =========== encode ================
info_len=1200;
poly = [133 171];
trellis = poly2trellis(7,poly);

data_info = randi([0 1],info_len,1);
data_info = [data_info(1:end-6).' [0 0 0 0 0 0]].';

coded_data = convenc(data_info,trellis);

%% ========== modulate - AWGN - demodulate ==========
snr=1;
[llr_data,hard_data] = qpsk_mod_demod_soft(coded_data,snr);
llr_path = "../../test_data/vit/vit_source_1200_conv213_7_133_171_snr_12_llr.dat";
% file_write_double(llr_data,llr_path);

% hard decision decode
data_decodec_vit_hard = vitdec(coded_data,trellis,35,'trunc','hard');
biterr(data_info,data_decodec_vit_hard)
info_path = "../../test_data/vit/vit_source_1200_conv213_7_133_171_snr_12_llr_info.dat";
% file_write_char(data_info,info_path);

%% ========= llr decode ================
% 
% soft_decision_data = sova0(llr_data',[1 0 1 1 0 1 1; 1 1 1 1 0 0 1]);
% input_llr_hard = [];
% for i = soft_decision_data
%     if i<0
%         input_llr_hard = [input_llr_hard 0];
%     else
%         input_llr_hard = [input_llr_hard 1];
%     end
% end
% 
% biterr(input_llr_hard',data_info)

rx_llr=llr_data;

constrain_length = 7;
poly_conv = [1 0 1 1 0 1 1; 1 1 1 1 0 0 1];

input_llr = vit_llr_decode_SISO(rx_llr,constrain_length,poly_conv,[]);

input_llr_hard = [];
for i = input_llr
    if i<0
        input_llr_hard = [input_llr_hard 0];
    else
        input_llr_hard = [input_llr_hard 1];
    end
end

biterr(input_llr_hard',data_info)




% info_bits = file_read_char(info_path);
% rx_llr = file_read_double(llr_path);


%% generate output reference (refer to the length of polynomial)
[rows,cols] = size(poly_conv);
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

next_out_table = gen_next_out_table(poly_conv);

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
biterr(decode_data',data_info)


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

input_llr_hard = [];
for i = input_llr
    if i<0
        input_llr_hard = [input_llr_hard 0];
    else
        input_llr_hard = [input_llr_hard 1];
    end
end

biterr(input_llr_hard',data_info)

