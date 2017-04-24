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
int main(int argc, char * argv[]) {
    // Check number of arguments
    if (argc != 3) {
        printf("Error: please input 2 arguments.\n");
        return 1;
    }
    // Check arguments' values
    int ncell = atoi(argv[1]);
    int probindex = atoi(argv[2]);
    if (ncell < 1 || probindex < 1 || probindex > 5){
        printf("Error: please check your arguments. Number of cells must be bigger than 0 and problem index must be 1-5.");
        return 1;
    }
    // Create matrices
    int nnode = ncell+1;
    double * x = calloc(nnode, sizeof(double));
    double * y = calloc(nnode, sizeof(double));
    int ** index = (int**)BuildMatrix(nnode, nnode);
    double ** K = BuildMatrix(nnode*nnode, nnode*nnode);
    double * f = calloc(nnode*nnode, sizeof(double));
    // Find x and y array values
    double L;
    if(probindex == 5) {L = 0.025;}
    else {L = 1; }
    double h = (double) L / ncell;
    int i,j;
    for (i=0; i<nnode; i++) {
        x[i] = (double) i / ncell * L;
        y[i] = (double) i / ncell * L;
    }
    // Create indexes
    for (i=0; i<nnode; i++){
        for (j=0; j<nnode; j++){
            index[i][j] = i*nnode + j;
        }
    }
    // Create LHS and RHS
    Build_LHS(K,x,y,index,nnode,probindex,h);
    Build_RHS(f,x,y,index,nnode,probindex);
    // Solve for T
    lapack_int ipiv[nnode*nnode];
    LAPACKE_dgesv (LAPACK_ROW_MAJOR,nnode*nnode,1,*K,nnode*nnode,ipiv,f,1);
    // Create output file
    FILE *outfile;
    char *filename;
    filename = malloc((17 + strlen(argv[1]) + strlen(argv[2])) * sizeof(char));
    sprintf(filename, "heat_solution_%s_%s", argv[1], argv[2]);
    outfile = fopen(filename, "w");
    free(filename);
    // Write output file
    Output(outfile,x,y,f,index,nnode);
    free(K);
    free(f);
    free(x);
    free(y);
    free(index);
    fclose(outfile);
    return 0;
}
void Build_LHS(double ** K, double * x, double * y, int ** index, int nnode, int probindex, double h){
    //Left and right sides
    int j,ileft,iright;
    for (j=0;j<nnode;j++){
        ileft = index[0][j];
        iright = index[nnode-1][j];
        K[ileft][ileft] = 1;
        K[iright][iright] = 1;
    }
    //Top and bottom sides
    int i,itop,ibottom;
    for (i=0;i<nnode;i++){
        ibottom = index[i][0];
        itop = index[i][nnode-1];
        K[ibottom][ibottom] = 1;
        K[itop][itop] = 1;
    }
    //Interior
    int icenter;
    double kcenter,kbottom,ktop,kleft,kright;
    for (i=1;i<nnode-1;i++){
        for(j=1;j<nnode-1;j++){
            //set indices
            icenter = index[i][j];
            ibottom = index[i][j-1];
            itop = index[i][j+1];
            ileft = index[i-1][j];
            iright = index[i+1][j];
            //approximating constants
            kbottom = Conductivity( x[i], y[j] - h/2, probindex);
            ktop = Conductivity( x[i], y[j] + h/2, probindex);
            kleft = Conductivity( x[i] - h/2, y[j], probindex);
            kright = Conductivity( x[i] + h/2, y[j], probindex);
            //build K matrix
            K[icenter][icenter] = (kbottom + ktop + kleft +kright) / pow(h,2);
            K[icenter][ibottom] = -kbottom / pow(h,2);
            K[icenter][itop] = -ktop / pow(h,2);
            K[icenter][ileft] = -kleft / pow(h,2);
            K[icenter][iright] = -kright / pow(h,2);
        }
    }
}
void Build_RHS(double * f, double * x, double * y, int ** index, int nnode, int probindex){
    //Left and right sides
    int j,ileft,iright;
    double xleft,xright;
    xleft = x[0];
    xright = x[nnode-1];
    for (j=0;j<nnode;j++){
        ileft = index[0][j];
        iright = index[nnode-1][j];
        f[ileft] = BC(xleft,y[j],probindex);
        f[iright] = BC(xright,y[j],probindex);
    }
    //Top and bottom sides
    int i,itop,ibottom;
    double ytop,ybottom;
    ybottom = y[0];
    ytop = y[nnode-1];
    for (i=0;i<nnode;i++){
        ibottom = index[i][0];
        itop = index[i][nnode-1];
        f[ibottom] = BC(x[i],ybottom,probindex);
        f[itop] = BC(x[i],ytop,probindex);
    }
    //Interior
    int icenter;
    for (i=1;i<nnode-1;i++){
        for(j=1;j<nnode-1;j++){
            icenter = index[i][j];
            f[icenter] = Source(x[i],y[j],probindex);
        }
    }
}
double Source(double x, double y, int probindex){
    double f;
    if (probindex == 1) {
        f = 0;
    }
    else if (probindex == 2) {
        f = 2*( y*(1-y) + x*(1-x) );
    }
    else if (probindex == 3) {
        f = exp( -50 * sqrt( pow((x-0.5),2) + pow((y-0.5),2) ) );
    }
    else if (probindex == 4) {
        if (x < 0.1){f = 1;}
        else{f = 0;}
    }
    else if (probindex == 5) {
        if (0.01<x<0.015 && 0.01<y<0.015) {f = 1600000;}
        else{f = 0;}
    }
    return f;
}
double BC(double x, double y, int probindex){
    double g;
    if (probindex == 1) {
        g = x;
    }
    else if (probindex == 5) {
        g = 70+273;
    }
    else{
        g = 0;
    }
    return g;
}
double Conductivity(double x, double y, int probindex){
    double k;
    if (probindex == 4){
        if (x > 0.5){k = 20;}
        else{k = 1;}
    }
    else if(probindex == 5){
        if (0.01<x<0.015 && 0.01<y<0.015){k = 167;}
        else{k = 157;}
    }
    else{
        k = 1;
    }
    return k;
}
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
double ** BuildMatrix(int Nrows, int Ncols) {
    int n;
    double **A = (double **) calloc(Nrows, sizeof(double *));
    A[0] = (double *) calloc(Nrows * Ncols, sizeof(double));
    for (n = 1; n < Nrows; ++n) {
        A[n] = A[n - 1] + Ncols;
    }
    return A;
}