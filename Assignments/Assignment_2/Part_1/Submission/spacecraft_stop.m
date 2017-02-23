function [ value, isterminal, direction ] = spacecraft_stop( t,y )
%SPACECRAFT_STOP Summary of this function goes here
%defines the stopping points for the ODE45 integration
rM = 1737100;%m
rE = 6371000;%m
dEM = 384403000;%m
%% find distance between SC and Moon, and SC and Earth
dSM = norm([y(1),y(2)]-[y(5),y(6)]);
dSE = norm([y(1),y(2)]-[0,0]);
%% define the values that get tracked as they go to zero
value = [dSM-rM; dSE-rE; dSE-2*dEM]; %crashed Moon, crashed Earth, lost
%% stop ODE45 if value go to zero?
isterminal = [1;1;1]; 
%% direction as value go to zero
direction = [-1;-1;1];
end