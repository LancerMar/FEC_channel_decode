clear all;
clc

% Monte Carlo experiment‌ 
EbNodBs = -2:1:8;
max_experiment_times = 100000;

G = [1 0 0 1 0 1;
     0 1 0 1 1 0;
     0 0 1 0 1 1];

H = [1 1 0 1 0 0;
     0 1 1 0 1 0;
     1 0 1 0 0 1];

[rows_H,cols_H] = size(H);
k = cols_H - rows_H;
n = cols_H;
code_rate = k/n;

channel_bit_errs = zeros(length(EbNodBs),1);
channel_bit_errs_idx = 1;
decode_bit_errs = zeros(length(EbNodBs),1);
decode_bit_errs_idx = 1;

for EbNodB = EbNodBs
    channel_bit_err_count = 0;
    decode_bit_err_count = 0;
    for experiment_time = 1:max_experiment_times
        msg = randi([0 1],3,1)';
        code_word = mod(msg*G,2);

        % channel factor for BPSK modulation
        EbNo = 10^(EbNodB/10);
        sigma = sqrt(1/(2*code_rate*EbNo));
        
        % BPSK convert bit to symbol
        symbols = 1-2*code_word;
        % simple AWGN channel
        received_word = symbols+sigma*randn(1,n);
        hard_bits = (1-sign(received_word))/2;
        channel_bit_err = biterr(hard_bits,code_word);
        channel_bit_err_count = channel_bit_err_count + channel_bit_err;

        %channel decode
        LLR = received_word;
        max_iteration=8;
        [decode_bits,~,~] = LDPC_decoder(H,LLR,max_iteration);
        decode_bit_err = biterr(decode_bits,code_word);
        decode_bit_err_count = decode_bit_err_count+decode_bit_err;
    end
    code_word_count = max_experiment_times*length(hard_bits);
    channel_bit_errs(channel_bit_errs_idx) = channel_bit_err_count/code_word_count;
    channel_bit_errs_idx=channel_bit_errs_idx+1;

    decode_bit_errs(decode_bit_errs_idx) = decode_bit_err_count/code_word_count;
    decode_bit_errs_idx = decode_bit_errs_idx+1;
end

figure
semilogy(EbNodBs,channel_bit_errs,"--",EbNodBs,decode_bit_errs);
xlabel ('EBNodB')
ylabel('bit error ratio')
legend('channel bit erro ratio','decode bit erro ratio')
grid on
