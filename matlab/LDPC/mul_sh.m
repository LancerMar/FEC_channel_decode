function y = mul_sh(x,k)
% multiplication by shifted identity
% x input metrix
% k -1 or shift value
% y output

if (k==-1)
    y = zeros(1,length(x));  % multiply all zeros matrix 
else
    y = [x(k+1:end) x(1:k)]; % multiply shifted identity
end