#include<math.h>
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
