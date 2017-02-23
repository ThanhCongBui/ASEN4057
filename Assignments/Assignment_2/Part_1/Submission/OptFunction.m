function f = OptFunction(x)
%% declare parameters
G = 6.674E-11; %N(m/kg)^2:
mM = 7.34767309E22;%kg
mE = 5.97219E24;%kg
mS = 28833;%kg
rM = 1737100;%m
rE = 6371000;%m
dES = 340000000;%m
vS = 1000;%m/s
thetaS = 50*pi/180;%rad
dEM = 384403000;%m
vM =sqrt(G*mE^2/((mE + mM)*dEM));%m/s
thetaM = 42.5*pi/180;%rad
%% gather all parameters into an array
params = [G,mM,mE,mS,rM,rE];
%% declare initial conditions
xS = dES*cos(thetaS);%m
yS = dES*sin(thetaS);%m
vSx = vS*cos(thetaS);%m/s
vSy = vS*sin(thetaS);%m/s
xM = dEM*cos(thetaM);%m
yM = dEM*sin(thetaM);%m
vMx = -vM*sin(thetaM);%m/s
vMy = vM*cos(thetaM);%m/s
IC = [xS;...
      yS;...
      vSx;...
      vSy;...
      xM;...
      yM;...
      vMx;...
      vMy];
IC(3) = IC(3) + x(1);
IC(4) = IC(4) + x(2);
%% simulate response to initial conditions
opts = odeset('Events',@spacecraft_stop,'RelTol', 1e-10);
tspan = [0 1e10];
[t,y,tE,yE,iE] = ode45(@(t,y) model(t,y,params),tspan,IC,opts);
if iE == 1
    fprintf('Crashed into Moon.\n');
elseif iE == 2
    fprintf('Returned to Earth.\n');
    %fprintf('The deltaV vector is: %.6f m/s, %.6f m/s\n',x(1),x(2))
    %fprintf('Magnitude: %.6f m/s\n',norm(x));
elseif iE == 3
    fprintf('Lost in space.\n');
end
%% if returned to Earth, then return magnitude of velocity change
if iE == 2
    f = norm(x);
else %else make it a huge velocity change
    f = 100000;
end