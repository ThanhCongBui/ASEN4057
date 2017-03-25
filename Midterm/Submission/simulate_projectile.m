function [tfin,xfin,yfin] = simulate_projectile(x0,y0,vx0,vy0,m,r,CD,e,n)
%SIMULATE_PROJECTILE Summary of this function goes here
%   Detailed explanation goes here
%% constants
g = 9.81;
G = m*g;
A = pi*r^2;
%% initial conditions and parameters
IC = [x0;y0;vx0;vy0];
params = [m,G,A,CD];
%% initialize event and stuff
opts = odeset('Events',@myevent,'RelTol', 1e-3);
tspan = [0 1e10];
tstart = 0;
tfin = [];
xfin = [];
yfin = [];
%% integrate and concatenate
for i = 1:n+1
    [t,y,tE,yE] = ode45(@(t,y) model(t,y,params),tspan,IC,opts);
    % store results away
    tfin = [tfin; tstart+t];
    xfin = [xfin; y(:,1)];
    yfin = [yfin; y(:,2)];
    % make new initial conditions
    tstart = tstart + tE;
    IC = [yE(1);yE(2);yE(3);-e*yE(4)];
end
% figure
% plot(tfin,xfin,'r',tfin,yfin,'b')
% figure
% plot(xfin,yfin)
end