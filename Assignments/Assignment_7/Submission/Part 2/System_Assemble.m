%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Function: System_Assemble
%
% Purpose: Assemble the linear solution!
%
% Input: cells_per_side = Number of cells/side
%        conductivity = Function handle
%        source = Function handle
%        BC = Function handle
%
% Output: K = LHS matrix
%         F = RHS vector
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [K,F] = System_Assemble(cells_per_side,conductivity,source,BC)

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
% Initialization:
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

K = zeros(nodes_per_side^2,nodes_per_side^2);
F = zeros(nodes_per_side^2,1);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Interior Assembly:
%
% Form the interior contributions to the
% LHS matrix and RHS vector
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for i = 2:cells_per_side
    for j = 2:cells_per_side        
        %%%%
        % Determine position of center
        
        x = x_array(i);
        y = y_array(j);
        
        %%%%
        % Determine the indices associated with five-point stencil
        
        index_center = index(i,j);        
        index_bottom = index(i,j-1);
        index_top = index(i,j+1);
        index_left = index(i-1,j);
        index_right = index(i+1,j);
        
        %%%%
        % Determine conductivities associated with five-point stencil
        
        kappa_bottom = conductivity(x,y-h/2);
        kappa_top = conductivity(x,y+h/2);
        kappa_left = conductivity(x-h/2,y);
        kappa_right = conductivity(x+h/2,y);        
        
        
        %%%%
        % Determine contributions to LHS matrix
        
        K(index_center,index_center) = (kappa_bottom + kappa_top + kappa_left + kappa_right)/h^2;        
        K(index_center,index_bottom) = -kappa_bottom/h^2;
        K(index_center,index_top) = -kappa_top/h^2;
        K(index_center,index_left) = -kappa_left/h^2;
        K(index_center,index_right) = -kappa_right/h^2;
        
        %%%%
        % Determine contribution to RHS vector
        
        F(index_center) = source(x,y);
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Boundary Assembly, Left and Right Sides:
%
% Form the boundary contributions to the
% LHS matrix and RHS vector, left and right
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for j = 1:nodes_per_side
    %%%%
    % Determine position
    
    x_left = x_array(1);
    x_right = x_array(nodes_per_side);
    
    y = y_array(j);
    
    %%%%
    % Determine the i indices
    
    i_left = 1;
    i_right = nodes_per_side;
    
    %%%%
    % Determine the position indices
    
    index_left = index(i_left,j);
    index_right = index(i_right,j);
    
    %%%%
    % Set diagonal entries of LHS
    
    K(index_left,index_left) = 1;
    K(index_right,index_right) = 1;
    
    %%%%
    % Set RHS according to BC
    
    F(index_left) = BC(x_left,y);
    F(index_right) = BC(x_right,y);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Boundary Assembly, Bottom and Top Sides:
%
% Form the boundary contributions to the
% LHS matrix and RHS vector, bottom and top
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for i = 1:nodes_per_side
    %%%%
    % Determine position
    
    x = x_array(i);
    
    y_bottom = y_array(1);
    y_top = y_array(nodes_per_side);
    
    %%%%
    % Determine the j indices
    
    j_bottom = 1;
    j_top = nodes_per_side;
    
    %%%%
    % Determine the position indices
    
    index_bottom = index(i,j_bottom);
    index_top = index(i,j_top);
    
    %%%%
    % Set diagonal entries of LHS
    
    K(index_bottom,index_bottom) = 1;
    K(index_top,index_top) = 1;
    
    %%%%
    % Set RHS according to BC
    
    F(index_bottom) = BC(x,y_bottom);
    F(index_top) = BC(x,y_top);
end