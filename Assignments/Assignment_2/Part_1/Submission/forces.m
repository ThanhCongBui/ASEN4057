function [ FAB,FBA ] = forces( RA,RB,mA,mB )
%FORCES Summary of this function goes here
%   takes in the position vectors of 2 bodies and find the gravitational 
%   force vectors between them
G = 6.674E-11; %N
dAB = norm(RA-RB);
FAB = G*mA*mB*(RA-RB)/dAB^3;
FBA = -FAB;
end