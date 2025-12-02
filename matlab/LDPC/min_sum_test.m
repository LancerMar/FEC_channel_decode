clear all;
clc


f = @(x) abs(log(tanh(abs(x)/2)));
x=[-5:0.01:5];
plot(x,f(x));
grid on

%% this is just a experiment,we get positive vector to test min sum algorithm)
% some calculate for min-sum calculation
% r = 1+0.8*randn(1,3);

l=(2/(0.8^2))*r;

f = @(x) abs(log(tanh(abs(x)/2)));
L1ext = f(f(l(2))+f(l(3)));
L2ext = f(f(l(1))+f(l(3)));
L3ext = f(f(l(1))+f(l(2)));


%% min-sum for SPC SISO decoder
n = 7;
r = 1+0.8*randn(1,n);

l=(2/(0.8)^2)*r;
S = prod(sign(l));

[m1,pos] = min(abs(l));
m2 = min(abs([l(1:pos-1) l(pos+1:end)]));

% find every min sequence[min(|l(2)| |l(3)| ...|l(n)|) min(|l(1)| |l(3)| ...|l(n)|) ... min(|l(1)| |l(2)| ...|l(n-1)|]
min_seq = ones(1,n)*m1;
min_seq(pos) = m2;

% find [sign(ext,1) sign(ext,2) ... sign(ext,n)]
sign_ext = S*sign(l);

% get Lextrinsic sequence
Lextrinsic_seq = sign_ext.*min_seq;

