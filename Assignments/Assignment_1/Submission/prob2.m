clear all;
close all;
clc;
%inputs
a = [-5,-2,0,2,3,5,7,10,14];
Cl = [-0.008,-0.003,0.001,0.005,0.007,0.006,0.009,0.017,0.019];

[m,b] = bestfit(a,Cl)

figure
scatter(a,Cl)
hold on
plot(a,m*a+b)
xlabel('Angle of Attack(degrees)')
ylabel('Coefficient of Lift')
legend('Experimental Data','Best Fit Line')