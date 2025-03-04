clear all;
clc



%% convelutional encoded data
info_len = 1200; % 信息向量长度
% trellis = poly2trellis(3,[7 5]);

trellis = poly2trellis(7,[133 171]);

data_info = randi([0 1],info_len,1);
data_info = [data_info(1:end-6).' [0 0 0 0 0 0]].';
% file_write_char(data_info,"data_info.dat");
coded_data = convenc(data_info,trellis);
% file_write_char(coded_data,"coded_data.dat");

tic;
data_decodec_vit = vitdec(coded_data,trellis,35,'trunc','hard');
toc;

biterr(data_decodec_vit,data_info)
% data_info = file_read_char("data_info.dat");
% coded_data = file_read_char("coded_data.dat");

%polynomial = (3 [7,5])
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


%% generate output reference (refer to the length of polynomial)
[rows,cols] = size(poly_conv);

ref_hard = zeros((2^rows),rows);
for i = 1:(2^rows)
    ref_tmp = dec2bin(i-1) - '0';
    ref_hard(i,:) = padarray(ref_tmp,[0 rows - length(ref_tmp)],0,'pre');
end


%% generate input refer to current state and previous state 
identity_matrix = eye(2^(constrain_length-2));
repeat_matrix = [1 1];
input_ref_cur_pre = [zeros(2^(constrain_length-2),2^(constrain_length-1));kron(identity_matrix,repeat_matrix)];

%% calculate the last state and the next state (only calculate once)（状态与卷积深度有关，输出与多项式个数有关）
% [rows,cols] = size(poly_conv);
% 
% % calculate state reference
% ref_regs_state = zeros((2^(cols-1)),cols-1);
% for i = 1:(2^(cols-1))
%     ref_tmp = dec2bin(i-1) - '0';
%     ref_regs_state(i,:) = padarray(ref_tmp,[0 cols - 1 - length(ref_tmp)],0,'pre');
% end
% 
% regs_state = ref_regs_state;
% % 输入 0 1 所有状态对应的下一个输出的状态表
% next_table = zeros(2^(cols-1),2);
% 
% % ==== exp: next out ==========
% %        | 0  | 1  |
% %-    --------------
% %     00 | 00 | 11 |
% %     01 | 11 | 00 |
% %     10 | 10 | 01 |
% %     11 | 01 | 10 |
% 
% for i = 1:length(regs_state)
%     last_state = regs_state(i,:);
% 
%     % input 0
%     [~,next_out] = general_conv_encode_step(last_state,0,poly_conv);
%     next_out_dec = bin_vec_2_dec(next_out);
%     next_table(i,1) = next_out_dec;
%     
%     % input 1
%     [~,next_out] = general_conv_encode_step(last_state,1,poly_conv);
%     next_out_dec = bin_vec_2_dec(next_out);
%     next_table(i,2) = next_out_dec;
% 
% end

next_out_table = gen_next_out_table(poly_conv);


%% survive path caculate
len_rx_data = length(rx_data);
path_matric = zeros((2^(cols-1)),1); % 寄存器状态个数
path_matric_temp = zeros((2^(cols-1)),1); % 寄存器状态个数
suvive_single_path = zeros((2^(cols-1)),1);
suvive_path = zeros((2^(cols-1)),len_rx_data/rows);

% initialize path matric to -infinity (工程实现中可以令其为当前容器的最大值)
% for i = 1:(2^(cols-1))
%     path_matric(i,1) = 0;
% end


tic;
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
        suvive_single_path(state,1) = idx;
        if 0 == mod(state,2)
            suvive_single_path(state,1) = idx + 2;
        end

        if state < 33
            suvive_single_path(state,1) = idx + (state - 1)*2;
        end
        
        if state > 32
            suvive_single_path(state,1) = idx + (state - 33)*2;
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

toc;


% for i = 1:length(input)
%     if input(i)~=data_info(i)
%        disp(i);
%     end
% end

biterr(input(1:end),data_info(1:end).')















%         % input = 0
%         if state < 2^(constrain_length-2)
%             pre_path_1_state = (state-1)*2 + 1; % 状态 * 2 -> 二进制整体左移一位
%             pre_path_2_state = (state-1)*2 + 1 + 1;
%             next_out_1 = next_out_table(pre_path_1_state,0+1) + 1;
%             next_out_2 = next_out_table(pre_path_2_state,0+1) + 1;
%     
%             branch_matric_1 = path_matric(pre_path_1_state,1) + HammingDist_set(next_out_1,1); 
%             branch_matric_2 = path_matric(pre_path_2_state,1) + HammingDist_set(next_out_2,1);
% 
% 
%         else
%             % input = 1
%             pre_path_1_state = (state-2^(constrain_length-2))*2 + 1; % state-2^(constrain_length-2): 二进制整体右移一位
%             pre_path_2_state = (state-2^(constrain_length-2))*2 + 1 + 1;
%             next_out_1 = next_out_table(pre_path_1_state,1+1) + 1;
%             next_out_2 = next_out_table(pre_path_2_state,1+1) + 1;
%     
%             branch_matric_1 = path_matric(pre_path_1_state,1) + HammingDist_set(next_out_1,1); 
%             branch_matric_2 = path_matric(pre_path_2_state,1) + HammingDist_set(next_out_2,1);
% 
% 
%         end





