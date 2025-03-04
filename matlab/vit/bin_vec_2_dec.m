function dec = bin_vec_2_dec(bin_vec)
% bin_vec_2_dec trans binary vector to decimal 
    dec = 0;
    for i = 1:length(bin_vec)
        if bin_vec(i) == 1
            dec = dec + 2^(length(bin_vec) - i);
        end
    end
end