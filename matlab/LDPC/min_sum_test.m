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
