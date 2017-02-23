function [hmax] = maxalt( r,MW,Wpayload,Wballoon )
%MAXALT Summary of this function goes here
%   Detailed explanation goes here
Wtotal = 0;
Wair = 0;
h = 0;
%find altitude at which total weight of balloon is heavier than weight of
%air displaced by balloon
while (Wtotal - Wair) <= 0
   h = h + 10;
   Wtotal = totalwt( r,MW,Wpayload,Wballoon );
   Wair = airwt( r,h );
end
hmax = h;
end