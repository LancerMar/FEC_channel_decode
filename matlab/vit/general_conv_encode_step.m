function [next_state,next_out] = general_conv_encode_step(state,input,g)
% general_conv_encode_step 单步运算卷积器
% [input]
%   state:  current state (row vector)
%   input:  1 input
%   g:      polynomial
% [output]
%   next_state
%   next_out
    [rows,~] = size(g);
    regs_state = [input state];
    next_state = regs_state(1:end-1);
    next_out = zeros(1,rows);

    for j = 1:rows
        next_out(j) = mod(sum(and(regs_state,g(j,:))),2);
    end

end