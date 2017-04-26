#ifndef _INTEGRATOR_H
#define _INTEGRATOR_H
//If this doesn't compile well, add <stdlib.h> or <stdio.h> to this file. Something 
// with FILE type might be conflicting

int integrator(double *y0, FILE * outfile, double clearance, int cond);


#endif
