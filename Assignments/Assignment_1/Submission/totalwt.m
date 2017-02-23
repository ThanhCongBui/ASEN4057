function [ Wtotal ] = totalwt( r,MW,Wpayload,Wballoon )
%TOTALWT Summary of this function goes here
%   Detailed explanation goes here
rho = 1.225; %density of air at SL
%find weight of gas inside balloon
Wgas = (4*pi*rho*r^3/3)*(MW/28.966);
%find total weight of balloon
Wtotal = Wgas + Wpayload + Wballoon;
end