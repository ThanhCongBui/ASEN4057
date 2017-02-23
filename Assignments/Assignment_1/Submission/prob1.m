clear all;
close all;
clc;
%get inputs
theta = input('Enter the initial angle: ');
v0 = input('Enter the initial airspeed: ');
g = -9.81;
%find time projectile hits ground
t = -2*v0*sind(theta)/g;
%calculate x and y position
t = linspace(0,t,300);
x = v0*cosd(theta)*t;
y = 0.5*g*t.^2 + v0*sind(theta)*t;
plot(x,y)
xlabel('x-position(m)')
ylabel('y-position(m)')