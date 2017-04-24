#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cblas.h>
#include <lapacke.h>
double ** BuildMatrix(int Nrows, int Ncols);
void Output(FILE * outfile, double * x, double * y, double * T, int ** index,int nnode);
double Conductivity(double x, double y, int probindex);
double BC(double x, double y, int probindex);
double Source(double x, double y, int probindex);
void Build_RHS(double * f, double * x, double * y, int ** index, int nnode, int probindex);
void Build_LHS(double ** K, double * x, double * y, int ** index, int nnode, int probindex, double h);
