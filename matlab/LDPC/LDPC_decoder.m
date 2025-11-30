function [decode_bits,syndrome,iterate_times] = LDPC_decoder(H,LLR,max_iteration)
%LDPC_decoder normal LDPC decoder(the H should be (0,1) matrix)
% [INPUT]
%       H               parity check matrix
%       LLR             belief of received code word( 0:LLR>0; 1:LLR<0 )
%       max_iteration   maxinum of iteration times
% [OUTPUT]
%       decode_bits     decoded bits(after hard decision)
%       syndrome        check result of H*c^T
%       iterate_times   actual iteration times
    L_storage = LLR.*H;
    L_sign = sign(L_storage);
    
    iteration = 0;
    while iteration<max_iteration
        iteration = iteration+1;
        % iteration start
        % row computation : SISO SPC decoder
        for i = 1:rows_H
            min_sum_regs = L_storage(i,L_storage(i,:)~=0);
            L_sign_regs = L_sign(i,L_sign(i,:)~=0);
        
            % get min1 and min2
            [min1,idx_min1] = min(abs(min_sum_regs(1,:)));
            min_sum2_tmp = [min_sum_regs(1:(idx_min1-1)) min_sum_regs((idx_min1+1):end)];
            min2 = min(abs(min_sum2_tmp));
        
            % flip by S
            S = prod(L_sign_regs);
            L_sign_regs = L_sign_regs*min1;
            L_sign_regs(idx_min1) = min2*sign(L_sign_regs(idx_min1));
            L_sign_regs = S*L_sign_regs;
        
            % update storage matrix
            idx_L_sign_regs = 1;
            for j=1:cols_H
                if L_storage(i,j)~=0
                    L_storage(i,j) = L_sign_regs(idx_L_sign_regs);
                    idx_L_sign_regs=idx_L_sign_regs+1;
                end
            end
        end
    
        % get extrinsic message of SPC
        LLR_extrinsic = sum(L_storage);
        % update the received code word by extrinsic message and intrinsic
        % message
        LLR = LLR_extrinsic + LLR;
    
        % check if curret LLR is correct
        check_result = check_cword_normal(H,received_word);
        if check_result == 1
            disp("check pass!");
            break;
        else
            disp("check fail");
        end

    end
end








