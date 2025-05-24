function [llr_data,hard_data] = qpsk_mod_demod_soft(data,snr)
    data_qpsk_symbol = zeros(length(data)/2,1);
    % 符号计算
    for i=1:(length(data)/2)
        data_qpsk_symbol(i) = data(2*i-1)*2 +data(2*i)*1;
    end
    
    data_qpsk_mod = pskmod(data_qpsk_symbol,4);
    
    % 加噪
    rxSig = awgn(data_qpsk_mod,snr);
	
	% 随机频率偏移
    Rb=1e6;
    maxfc=0.5e-4;
    fc=randperm(2*maxfc*Rb,1)/Rb-maxfc;
    rxSig = rxSig .* exp(sqrt(-1)*2*pi*fc*(0:length(rxSig)-1).');
    
    % 过信道后星座图
    % scatterplot(rxSig);
    
    % 软判决
    llr_data = pskdemod(rxSig,4,OutputType='llr');
    
    % 硬判决
    hard_data = zeros(length(llr_data),1);
    for i=1:length(llr_data)
        if llr_data(i)<0
            hard_data(i) = 1;
        else
            hard_data(i) = 0;
        end
    end
end