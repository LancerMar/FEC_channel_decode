clear all;
clc

load Base_Matrics_5G\NR_1_2_20.txt
Base_matric = NR_1_2_20;
expansion_factor = 20;

[rows,cols] = size(Base_matric);

% number of rows*expansion_factor = length of parity 
%number of cols*expansion_factor = length of code_word
msg_length = (cols - rows) * expansion_factor;


msg = randi([0 1],1,msg_length);

code_word = zeros(1,cols*expansion_factor);
% assign message word in code word
code_word(1:msg_length) = msg;


%% double-diagonal encode
for i = 

