function [m,b] = bestfit(x,y)
%BESTFIT Summary of this function goes here
%   Detailed explanation goes here
%input
% x = [-3,12.1,20,0,8,3.7,-5.6,0.5,5.8,10];
% y = [-11.06,58.95,109.73,3.15,44.83,21.29,-27.29,5.11,34.01,43.25];

A = sum(x);
B = sum(y);
C = sum(x.*y);
D = sum(x.^2);
N = length(x);

m = (A*B-N*C)/(A^2-N*D);
b = (A*C-B*D)/(A^2-N*D);

% figure
% scatter(x,y)
% hold on
% plot(x,m*x+b)
end