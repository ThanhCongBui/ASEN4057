function dydt = model( ~,y,params )
%MODEL Summary of this function goes here
%   Detailed explanation goes here
%% take out parameters
m = params(1);
G = params(2);
A = params(3);
CD = params(4);
rho = 1.275;
%% take out state variables
vx = y(3);
vy = y(4);
v = norm([vx vy]);
%% calculate drag
D = 0.5*CD*rho*v^2*A;
theta = atan2(vy,vx);
%% find accelerations
ax = -D*cos(theta)/m;
ay = (-D*sin(theta) - G)/m;
%% define rate of change state vector
dydt = [vx;vy;ax;ay];
end