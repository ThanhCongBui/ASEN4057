%% housekeeping
clear all;
close all;
clc;
%% initial guess
vpert0 = [0,50]';
%% optimize perturbation velocity Objective 1
vpert = fminsearch(@(x)OptFunction(x),vpert0)
vpertmagnitude = norm(vpert)