function out = check_cword(B,z,c);
%B: base matrix
%z: expansion factor
%c: codewords(must be length of cols(B)*z)
%out = 1, code word valid; 0, code word not valid

[rows,cols] = size(B); 

syn = zeros(rows*z,1); %result of Hc^T

% Hc^T
for i = 1:rows
    for j = 1:cols
        % (ci*Ik) multiply base matrix and 1 block(expansion z) of codeword
        syn_block = mul_sh(c((j-1)*z+1:j*z),B(i,j));
        % (c(0)*Ik + c(1)*Ik +...+c(cols)*Ik) add all cols to get syndrome
        syn((i-1)*z+1:i*z) = mod(syn((i-1)*z+1:i*z)+syn_block',2);
    end
end

if(all(syn(:) == 0))
    out = 1;
else
    out = 0;
end
