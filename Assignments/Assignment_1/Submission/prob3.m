clear all;
close all;
clc;
%balloon parameters
r = 3; %m
Wpayload = 5; %kg
Wballoon = 0.6; %kg
MW = 4.02; %molecular weight of helium
%calculate max height
hmax = maxalt( r,MW,Wpayload,Wballoon ) %m