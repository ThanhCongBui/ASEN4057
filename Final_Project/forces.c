//
// Created by cong on 4/26/17.
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
double *forces (double RAx, double RAy, double RBx, double RBy, double mA, double mB) {
    double G = 6.674*pow(10,-11);
    double dAB = sqrt( pow((RBx-RAx),2) + pow((RBy-RAy),2) );
    double *FAB = malloc(2*sizeof(double));
    FAB[0] = G*mA*mB*(RAx-RBx)/(pow(dAB,3));
    FAB[1] = G*mA*mB*(RAy-RBy)/(pow(dAB,3));
    return FAB;
}