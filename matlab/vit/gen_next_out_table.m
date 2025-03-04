function next_out_table = gen_next_out_table(poly_conv)
% gen_next_state_table generate next state table by polynomial
% [input] poly_conv: 卷积多项式

    [~,cols] = size(poly_conv);

    % calculate state reference
    ref_regs_state = zeros((2^(cols-1)),cols-1);
    for i = 1:(2^(cols-1))
        ref_tmp = dec2bin(i-1) - '0';
        ref_regs_state(i,:) = padarray(ref_tmp,[0 cols - 1 - length(ref_tmp)],0,'pre');
    end
    
    regs_state = ref_regs_state;
    % 输入 0 1 所有状态对应的下一个输出的状态表
    next_table = zeros(2^(cols-1),2);
    
    % ==== exp: next out ==========
    %        | 0  | 1  |
    %-    --------------
    %     00 | 00 | 11 |
    %     01 | 11 | 00 |
    %     10 | 10 | 01 |
    %     11 | 01 | 10 |
    
    for i = 1:length(regs_state)
        last_state = regs_state(i,:);
    
        % input 0
        [~,next_out] = general_conv_encode_step(last_state,0,poly_conv);
        next_out_dec = bin_vec_2_dec(next_out);
        next_table(i,1) = next_out_dec;
        
        % input 1
        [~,next_out] = general_conv_encode_step(last_state,1,poly_conv);
        next_out_dec = bin_vec_2_dec(next_out);
        next_table(i,2) = next_out_dec;
    
    end

    next_out_table = next_table;
end