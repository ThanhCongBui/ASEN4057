#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// Include other header files from modularized code later
int integrator(double *y0, FILE * outfile, double clearance, int cond){
  // If cond == 1, integrator will print stuff. Otherwise set to 0. 

  int er;
double * masses = malloc(sizeof(double)*3);
double * tspan = malloc(sizeof(double)*2);
tspan[0] = 0;
tspan[1] = 1000000;
double h = 10;
masses[0] = 5.9722*pow(10,24);
masses[1] = 7.342*pow(10,22);
masses[2] = 28.833;
//Set starting time to first value in tspan
double t = tspan[0];
double dMS;
double dES;
double dEM;
double rM = 1737100;
double rE = 6371000;

//Predeclaring variables for RK4
double k1_xs, k1_ys, k1_xm, k1_ym, k1_vxs, k1_vys, k1_vxm, k1_vym;
double k2_xs, k2_ys, k2_xm, k2_ym, k2_vxs, k2_vys, k2_vxm, k2_vym;
double k3_xs, k3_ys, k3_xm, k3_ym, k3_vxs, k3_vys, k3_vxm, k3_vym;
double k4_xs, k4_ys, k4_xm, k4_ym, k4_vxs, k4_vys, k4_vxm, k4_vym;

//Create struct vector, assign initial conditions
y.xs = y0[0];
y.ys = y0[1];
y.xm = y0[2];
y.ym = y0[3];
y.vxs = y0[4];
y.vys = y0[5];
y.vxm = y0[6];
y.vym = y0[7];

  //Using RK4 method to get velocities and positions
/*-----------------------------------------------------------------*/
while (t <= tspan[1]){
  
  // First slopes (k1), y is evaluated at t
  dydt = dydtfun(dydt,y,masses);
  k1_xs = dydt.xs_dot;
  k1_ys = dydt.ys_dot;  
  k1_xm = dydt.xm_dot;
  k1_ym = dydt.ym_dot;
  k1_vxs = dydt.vxs_dot;
  k1_vys = dydt.vys_dot;  
  k1_vxm = dydt.vxm_dot;
  k1_vym = dydt.vym_dot;

  // Estimate of first state value at t=t+h/2 using k1
  ytemp.xs = y.xs + k1_xs*h/2;
  ytemp.ys = y.ys + k1_ys*h/2;
  ytemp.xm = y.xm + k1_xm*h/2;
  ytemp.ym = y.ym + k1_ym*h/2;
  ytemp.vxs = y.vxs + k1_vxs*h/2;
  ytemp.vys = y.vys + k1_vys*h/2;
  ytemp.vxm = y.vxm + k1_vxm*h/2;
  ytemp.vym = y.vym + k1_vym*h/2;

  // Second slopes (k2)
  dydt = dydtfun(dydt,ytemp,masses);
  k2_xs = dydt.xs_dot;
  k2_ys = dydt.ys_dot;  
  k2_xm = dydt.xm_dot;
  k2_ym = dydt.ym_dot;
  k2_vxs = dydt.vxs_dot;
  k2_vys = dydt.vys_dot;  
  k2_vxm = dydt.vxm_dot;
  k2_vym = dydt.vym_dot;
  
  // Estimate of state values at t=t+h/2 using k2
  ytemp.xs = y.xs + k2_xs*h/2;
  ytemp.ys = y.ys + k2_ys*h/2;
  ytemp.xm = y.xm + k2_xm*h/2;
  ytemp.ym = y.ym + k2_ym*h/2;
  ytemp.vxs = y.vxs + k2_vxs*h/2;
  ytemp.vys = y.vys + k2_vys*h/2;
  ytemp.vxm = y.vxm + k2_vxm*h/2;
  ytemp.vym = y.vym + k2_vym*h/2;

   // Third slopes (k3)
  dydt = dydtfun(dydt,ytemp,masses);
  k3_xs = dydt.xs_dot;
  k3_ys = dydt.ys_dot;  
  k3_xm = dydt.xm_dot;
  k3_ym = dydt.ym_dot;
  k3_vxs = dydt.vxs_dot;
  k3_vys = dydt.vys_dot;  
  k3_vxm = dydt.vxm_dot;
  k3_vym = dydt.vym_dot;

  
  // Estimate of state values at t=t+h using k3
  ytemp.xs = y.xs + k3_xs*h/2;
  ytemp.ys = y.ys + k3_ys*h/2;
  ytemp.xm = y.xm + k3_xm*h/2;
  ytemp.ym = y.ym + k3_ym*h/2;
  ytemp.vxs = y.vxs + k3_vxs*h/2;
  ytemp.vys = y.vys + k3_vys*h/2;
  ytemp.vxm = y.vxm + k3_vxm*h/2;
  ytemp.vym = y.vym + k3_vym*h/2;


  // Fourth slopes (k4)
  dydt = dydtfun(dydt,ytemp,masses);
  k4_xs = dydt.xs_dot;
  k4_ys = dydt.ys_dot;  
  k4_xm = dydt.xm_dot;
  k4_ym = dydt.ym_dot;
  k4_vxs = dydt.vxs_dot;
  k4_vys = dydt.vys_dot;  
  k4_vxm = dydt.vxm_dot;
  k4_vym = dydt.vym_dot;
    
  // Estimate of state vaiables at t=t+h
  y.xs += h*(k1_xs+2*k2_xs+2*k3_xs+k4_xs)/6;
  y.ys += h*(k1_ys+2*k2_ys+2*k3_ys+k4_ys)/6;
  y.xm += h*(k1_xm+2*k2_xm+2*k3_xm+k4_xm)/6;
  y.ym += h*(k1_ym+2*k2_ym+2*k3_ym+k4_ym)/6;
  y.vxs += h*(k1_vxs+2*k2_vxs+2*k3_vxs+k4_vxs)/6;
  y.vys += h*(k1_vys+2*k2_vys+2*k3_vys+k4_vys)/6;
  y.vxm += h*(k1_vxm+2*k2_vxm+2*k3_vxm+k4_vxm)/6;
  y.vym += h*(k1_vym+2*k2_vym+2*k3_vym+k4_vym)/6;
  t += h;
    if (cond == 1){
        fprintf(outfile, "%.4f %.4f %.4f %.4f %.4f\n",t,y.xs,y.ys,y.xm,y.ym);
    }
  // Calculate end conditions
  dMS = sqrt( pow((y.xs-y.xm),2) + pow((y.ys-y.ym),2) );
  dES = sqrt( pow((y.xs),2) + pow((y.ys),2) );
  dEM = sqrt( pow((y.xm),2) + pow((y.ym),2) );
  
  if(dMS <= rM + clearance){
   printf("SC hit the moon.\n");
   er = 1;
   break;
  }
  else if(dES <= rE){
   printf("SC returned to Earth.\n");
   er = 2;
   break;
  }
  else if(dES >= 2*dEM){
   printf("SC lost in space.\n");
   er = 3;
   break;
  }
 }
free(masses);
free(tspan);
return er;

}
