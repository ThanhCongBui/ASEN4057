%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Function: System_Plot
%
% Purpose: Plot the solution!
%
% Input: cells_per_side = Number of cells/side
%        d = Solution vector
%
% Output: N/A
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function System_Plot(cells_per_side,d)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Useful Variable Definitions:
%
% h = Mesh size
% x_array = Array of x positions
% y_array = Array of y positions
% nodes_per_side = Number of nodes per side
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

h = 1/cells_per_side;
x_array = 0:h:1; y_array = 0:h:1;
nodes_per_side = cells_per_side + 1;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Index Array:
%
% Takes two-dimensional index (i,j) to
% single index (i-1)*nodes_per_side + j
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for i = 1:nodes_per_side
    for j = 1:nodes_per_side
        index(i,j) = (i-1)*nodes_per_side + j;
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Plotting:
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

X_plot = zeros(nodes_per_side,nodes_per_side);
Y_plot = zeros(nodes_per_side,nodes_per_side);
Z_plot = zeros(nodes_per_side,nodes_per_side);

for i = 1:nodes_per_side
    for j = 1:nodes_per_side
        X_plot(i,j) = x_array(i);
        Y_plot(i,j) = y_array(j);
        Z_plot(i,j) = d(index(i,j));
    end
end

figure
surf(X_plot,Y_plot,Z_plot);
xlabel('x (in meters)','FontName','Times','FontSize',20)
ylabel('y (in meters)','FontName','Times','FontSize',20)
zlabel('T (in degrees Kelvin)','FontName','Times','FontSize',20)
set(gca,'FontName','Times','FontSize',18)