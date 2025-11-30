function out = check_cword_normal(H,LLR)
    hard_word = (1-sign(LLR))/2;
    syndrom = mod((H*hard_word'),2);
    if(all(syndrom(:) == 0))
        out = 1; % check pass
    else
        out = 0; % check fail
    end
end