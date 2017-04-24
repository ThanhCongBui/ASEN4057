clear all;
close all;
clc;
%% matlab prob 1
n = 5;
x = linspace(0,1,n);
y = linspace(0,1,n);
cells_per_side = n-1;
conductivity = @(x,y) 1;
source = @(x,y) 0;
BC = @(x,y) x;
[K,F] = System_Assemble(cells_per_side,conductivity,source,BC);
d = K\F;
System_Plot(n-1,d);
%% c prob 1
Ttemp = load('heat_solution_4_1');
T = zeros(n,n);
k = 1;
for i = 1:n
    for j = 1:n
    T(i,j) = Ttemp(k,3);
    k = k+1;
    end
end
System_Plot(n-1,T);
%% matlab prob 2
n = 51;
x = linspace(0,1,n);
y = linspace(0,1,n);
cells_per_side = n-1;
conductivity = @(x,y) 1;
source = @(x,y) 2*( y*(1-y) + x*(1-x) );
BC = @(x,y) 0;
[K,F] = System_Assemble(cells_per_side,conductivity,source,BC);
d = K\F;
System_Plot(n-1,d);
%% c prob 2
Ttemp = load('heat_solution_50_2');
T = zeros(n,n);
k = 1;
for i = 1:n
    for j = 1:n
    T(i,j) = Ttemp(k,3);
    k = k+1;
    end
end
System_Plot(n-1,T);
%% convergence study
Ttemp = load('heat_solution_5_2');
n = 6;
T1 = zeros(n,n);
k = 1;
for i = 1:n
    for j = 1:n
    T1(i,j) = Ttemp(k,3);
    k = k+1;
    end
end
System_Plot(n-1,T1);

n = 11;
Ttemp = load('heat_solution_10_2');
T2 = zeros(n,n);
k = 1;
for i = 1:n
    for j = 1:n
    T2(i,j) = Ttemp(k,3);
    k = k+1;
    end
end
System_Plot(n-1,T2);

n = 16;
Ttemp = load('heat_solution_15_2');
T3 = zeros(n,n);
k = 1;
for i = 1:n
    for j = 1:n
    T3(i,j) = Ttemp(k,3);
    k = k+1;
    end
end
System_Plot(n-1,T3);

n = 21;
Ttemp = load('heat_solution_20_2');
T4 = zeros(n,n);
k = 1;
for i = 1:n
    for j = 1:n
    T4(i,j) = Ttemp(k,3);
    k = k+1;
    end
end
System_Plot(n-1,T4);