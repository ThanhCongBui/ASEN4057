#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cblas.h>
#include <lapacke.h>
#include "myfuncs.h"

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
