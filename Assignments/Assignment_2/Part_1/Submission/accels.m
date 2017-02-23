function [ aCx, aCy ] = accels( F_A, F_B, m_C )
%ACCELERATIONS
%Finds the acceleration of object C given forces by object A and object B
%and mass of object C
aCx = (F_A(1) + F_B(1))/m_C;
aCy = (F_A(2) + F_B(2))/m_C;
end

