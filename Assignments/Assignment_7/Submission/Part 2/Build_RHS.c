#include"myfuncs.h"
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
