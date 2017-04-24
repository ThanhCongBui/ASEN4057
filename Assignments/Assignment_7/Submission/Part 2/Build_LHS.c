#include"myfuncs.h"
#include<math.h>
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
