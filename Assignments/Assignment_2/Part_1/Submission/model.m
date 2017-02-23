function dydt = model(t,y,params)
%MODEL Summary of this function goes here
%   Detailed explanation goes here
%% takes parameters out
G = params(1); %N(m/kg)^2:
mM = params(2);%kg
mE = params(3);%kg
mS = params(4);%kg
rM = params(5);%m
rE = params(6);%m
%% pull out position of SC and Moon
RS = [y(1),y(2)];
RM = [y(5),y(6)];
RE = [0,0];
%% find forces on SC and Moon
[ FSM,FMS ] = forces( RS,RM,mS,mM );
[ FSE,FES ] = forces( RS,RE,mS,mE );
[ FME,FEM ] = forces( RM,RE,mM,mE );
%% find accels of SC and Moon
[ aSx, aSy ] = accels( FMS, FES, mS );
[ aMx, aMy ] = accels( FSM, FEM, mM );
%% set up differential equations(velocities and accels)
dydt = [y(3);...
    y(4);...
    aSx;...
    aSy;...
    y(7);...
    y(8);...
    aMx;...
    aMy];
end