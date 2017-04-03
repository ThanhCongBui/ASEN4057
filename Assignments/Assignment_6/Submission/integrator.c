#include <math.h>
#include "dydt.h"

double *forces(double RAx, double RAy, double RBx, double RBy, double mA, double mB);
struct *dydt(struct *y_ptr, double * masses);

void integrator(double *tspan,double h,double *y0,double *masses, FILE * outfile) {

//Set starting time to first value in tspan
double t = tspan[0];

//Predeclaring variables for RK4
double k1_xs, k1_ys, k1_xm, k1_ym, k1_vxs, k1_vys, k1_vxm, k1_vym...
  ,k2_xs, k2_ys, k2_xm, k2_ym, k2_vxs, k2_vys, k2_vxm, k2_vym...
  ,k3_xs, k3_ys, k3_xm, k3_ym, k3_vxs, k3_vys, k3_vxm, k3_vym...
  ,k4_xs, k4_ys, k4_xm, k4_ym, k4_vxs, k4_vys, k4_vxm, k4_vym...
  ,y1_xs, y1_ys, y1_xm, y1_xm, y1_vxs, y1_vys, y1_vxm, y1_vym...
  ,y2_xs, y2_ys, y2_xm, y2_xm, y2_vxs, y2_vys, y2_vxm, y2_vym...
  ,y3_xs, y3_ys, y3_xm, y3_xm, y3_vxs, y3_vys, y3_vxm, y3_vym...;

struct dydt{
    double xs_dot;
    double ys_dot;
    double xm_dot;
    double ym_dot;
    double vxs_dot;
    double vys_dot;
    double vxm_dot;
    double vym_dot;
  };

struct dydt *dydt_ptr;

//Create struct vector, assign initial conditions
struct y{
  double xs = y0[0];
  double ys = y0[1];
  double xm = y0[2];
  double ym = y0[3];
  double vxs = y0[4];
  double vys = y0[5];
  double vxm = y0[6];
  double vym = y0[7];
};
struct y *y_ptr;

//Declare counter
int i = 1;
  //Using RK4 method to get velocities and positions
/*-----------------------------------------------------------------*/
while (t <= tspan[1]){
  
  // First slopes (k1), y is evaluated at t
  dydt_ptr = dydt(y_ptr,masses);
  k1_xs = dydt_ptr->xs_dot;
  k1_ys = dydt_ptr->ys_dot;  
  k1_xm = dydt_ptr->xm_dot;
  k1_ym = dydt_ptr->ym_dot;
  k1_vxs = dydt_ptr->vxs_dot;
  k1_vys = dydt_ptr->vys_dot;  
  k1_vxm = dydt_ptr->vxm_dot;
  k1_vym = dydt_ptr->vym_dot;

  // Estimate of first state value at t=t+h/2 using k1
  y1_xs = y_ptr->xs + k1_xs*h/2;
  y1_ys = y_ptr->ys + k1_ys*h/2;
  y1_xm = y_ptr->xm + k1_xm*h/2;
  y1_ym = y_ptr->ym + k1_ym*h/2;
  y1_vxs = y_ptr->vxs + k1_vxs*h/2;
  y1_vys = y_ptr->vys + k1_vys*h/2;
  y1_vxm = y_ptr->vxm + k1_vxm*h/2;
  y1_vym = y_ptr->vym + k1_vym*h/2;

  // Second slopes (k2)
  dydt_ptr = dydt(y_ptr,masses);
  k2_xs = dydt_ptr->xs_dot;
  k2_ys = dydt_ptr->ys_dot;  
  k2_xm = dydt_ptr->xm_dot;
  k2_ym = dydt_ptr->ym_dot;
  k2_vxs = dydt_ptr->vxs_dot;
  k2_vys = dydt_ptr->vys_dot;  
  k2_vxm = dydt_ptr->vxm_dot;
  k2_vym = dydt_ptr->vym_dot;
  
  // Estimate of state values at t=t+h/2 using k2
  y2_xs = y_ptr->xs + k2_xs*h/2;
  y2_ys = y_ptr->ys + k2_ys*h/2;
  y2_xm = y_ptr->xm + k2_xm*h/2;
  y2_ym = y_ptr->ym + k2_ym*h/2;
  y2_vxs = y_ptr->vxs + k2_vxs*h/2;
  y2_vys = y_ptr->vys + k2_vys*h/2;
  y2_vxm = y_ptr->vxm + k2_vxm*h/2;
  y2_vym = y_ptr->vym + k2_vym*h/2;

   // Third slopes (k3)
  dydt_ptr = dydt(y_ptr,masses);
  k3_xs = dydt_ptr->xs_dot;
  k3_ys = dydt_ptr->ys_dot;  
  k3_xm = dydt_ptr->xm_dot;
  k3_ym = dydt_ptr->ym_dot;
  k3_vxs = dydt_ptr->vxs_dot;
  k3_vys = dydt_ptr->vys_dot;  
  k3_vxm = dydt_ptr->vxm_dot;
  k3_vym = dydt_ptr->vym_dot;

  
  // Estimate of state values at t=t+h using k3
  y3_xs = y_ptr->xs + k3_xs*h/2;
  y3_ys = y_ptr->ys + k3_ys*h/2;
  y3_xm = y_ptr->xm + k3_xm*h/2;
  y3_ym = y_ptr->ym + k3_ym*h/2;
  y3_vxs = y_ptr->vxs + k3_vxs*h/2;
  y3_vys = y_ptr->vys + k3_vys*h/2;
  y3_vxm = y_ptr->vxm + k3_vxm*h/2;
  y3_vym = y_ptr->vym + k3_vym*h/2;


  // Fourth slopes (k4)
  dydt_ptr = dydt(y_ptr,masses);
  k4_xs = dydt_ptr->xs_dot;
  k4_ys = dydt_ptr->ys_dot;  
  k4_xm = dydt_ptr->xm_dot;
  k4_ym = dydt_ptr->ym_dot;
  k4_vxs = dydt_ptr->vxs_dot;
  k4_vys = dydt_ptr->vys_dot;  
  k4_vxm = dydt_ptr->vxm_dot;
  k4_vym = dydt_ptr->vym_dot;
  // Estimate of state vaiables at t=t+h
  y_ptr->xs += h*(k1_xs+2*k2_xs+2*k3_xs+k4_xs)/6;
  y_ptr->ys += h*(k1_ys+2*k2_ys+2*k3_ys+k4_ys)/6;
  y_ptr->xm += h*(k1_xm+2*k2_xm+2*k3_xm+k4_xm)/6;
  y_ptr->ym += h*(k1_ym+2*k2_ym+2*k3_ym+k4_ym)/6;
  y_ptr->vxs += h*(k1_vxs+2*k2_vxs+2*k3_vxs+k4_vxs)/6;
  y_ptr->vys += h*(k1_vys+2*k2_vys+2*k3_vys+k4_vys)/6;
  y_ptr->vxm += h*(k1_vxm+2*k2_vxm+2*k3_vxm+k4_vxm)/6;
  y_ptr->vym += h*(k1_vym+2*k2_vym+2*k3_vym+k4_vym)/6;

  fprintf(outfile, "%d %d %d %d\n",y_ptr->xs,y_ptr->ys,y_ptr->xm,y_ptr->ym);
  
}
}

struct *dydt(struct *y_ptr, double * masses){

// allocating memory for the derivatives vector
  struct dydt{
    double xs_dot;
    double ys_dot;
    double xm_dot;
    double ym_dot;
    double vxs_dot;
    double vys_dot;
    double vxm_dot;
    double vym_dot;
  }

struct dydt *dydr_ptr;

int mE = masses[0];
int mM = masses[1];
int mS = masses[2];

double RSx = y_ptr->xs;// ->xs;//location of spacecraft in relation to Earth
double RSy = y_ptr->ys;
double RMx = y_ptr->xm;//location of moon in relation to Earth
double RMy = y_ptr->ym;

//Getting forces
double *FES;
double *FEM;
double *FMS;

FES = forces(0,0,RSx,RSy,mE,mS);//Getting force on spacecraft from Earth
FEM = forces(0,0,RMx,RMy,mE,mM);//Getting force on Moon from Earth
FMS = forces(RMx,RMy,RSx,RSy,mM,mS);//Getting force on Spacecraft from moon
  
// X and Y components of the derived forces
double FESx = FES[0];
double FESy = FES[1];

double FEMx = FEM[0];
double FEMy = FEM[1];

double FMSx = FMS[0];
double FMSy = FMS[1];

//Freeing variables
free(FES);
free(FEM);
free(FMS);

//Constructing a struct variable of derivatives
dydt_ptr->xs_dot = y_ptr->vxs;
dydt_ptr->ys_dot = y_ptr->vys;
dydt_ptr->xm_dot = y_ptr->vxm;
dydt_ptr->ym_dot = y_ptr->vym;
dydt_ptr->vxs_dot = (FMSx+FESx)/mS;
dydt_ptr->vys_dot = (FMSy+FESy)/mS;
dydt_ptr->vxm_dot = (FEMx+FSMx)/mM;
dydt_ptr->vym_dot = (FEMy+FSMy)/mM;
return dydt;
}

double *forces (double RAx, double RAy, double RBx, double RBy, double mA, double mB) {
   double G = 6.674*pow(10,-11);
   double dAB = sqrt( pow((RBx-RAx),2) + pow((RBy-RAy),2) );
   double *FAB = malloc(2*sizeof(double));
   FAB[0] = G*mA*mB*(RBx-RAx)/(pow(dAB,3));
   FAB[1] = G*mA*mB*(RBy-RAy)/(pow(dAB,3));
   printf("force vector is %1.4f, %1.4f\n",FAB[0],FAB[1]);
   return FAB;
}
