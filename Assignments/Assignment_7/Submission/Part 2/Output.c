#include<stdio.h>
void Output(FILE * outfile, double * x, double * y, double * T, int ** index,int nnode){
    int i,j,ind;
    ind = 0;
    for(i=0;i<nnode;i++){
        for(j=0;j<nnode;j++){
            //ind = index[i][j];
            fprintf(outfile, "%lf ", x[i]);
            fprintf(outfile, "%lf ", y[j]);
            fprintf(outfile, "%lf\n", T[ind]);
            ind++;
        }
    }
}
