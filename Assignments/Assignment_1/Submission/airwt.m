function [ Wair ] = airwt( r,h )
%AIRWT Summary of this function goes here
%   Detailed explanation goes here
% find temperature and pressure at altitude
if (h > 0) && (h <= 11000)
    T = 15.04 - 0.00649*h;
    P = 101.29*((T+273.1)/288.08)^5.256;
elseif (h > 11000) && (h <= 25000)
    T = -56.46;
    P = 22.65*exp(1.73-0.000157*h);
elseif h > 25000
    T = -131.21 + 0.00299*h;
    P = 2.488*((T+273.1)/216.6)^-11.388;
else
    T = 0;
    P = 0;
end
%find air density at altitude
rho =P/(0.2869*(T + 273.1));
%find weight of air balloon displaces
Wair = 4*pi*rho*r^3/3;    
end

