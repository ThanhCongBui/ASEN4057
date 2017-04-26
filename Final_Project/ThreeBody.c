#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//STRUCTS
struct dydt_type{
    double xs_dot;
    double ys_dot;
    double xm_dot;
    double ym_dot;
    double vxs_dot;
    double vys_dot;
    double vxm_dot;
    double vym_dot;
  };
struct dydt_type dydt;
struct y_type{
  double xs; //= y0[0];
  double ys; //= y0[1];
  double xm; //= y0[2];
  double ym; //= y0[3];
  double vxs; //= y0[4];
  double vys; //= y0[5];
  double vxm; //= y0[6];
  double vym; //= y0[7];
};
struct y_type y;
struct y_type ytemp;

//FUNCTION PROTOTYPES
double *forces(double RAx, double RAy, double RBx, double RBy, double mA, double mB);
struct dydt_type dydtfun(struct dydt_type dydt, struct y_type y, double * masses);
int integrator(double *y0, FILE * outfile, double clearance, int cond);
double *delVmin_opt(double *y0, struct y_type y,double clearance,double accuracy, FILE * outfile1, double rE);
double *delVtime_opt(double *y0, struct y_type y,double clearance,double accuracy, FILE * outfile2, double rE);
char * myrename(char *name);

//---------------------------------MAIN--------------------------------------------
int main(int argc, char *argv[]){
  //Take out arguments
  int cond = 1; //We want integrator to write data to a file
  int objective = atoi(argv[1]);
  double clearance = atof(argv[2]);
  double accuracy = atof(argv[3]);
  double rE = 6371000;
  //Rename if theres a "." in argument
  char * name = malloc(sizeof(char)*strlen(argv[3]));
  sprintf(name, "%s", argv[3]);
  char * new = myrename(name);
  //Create output files
  FILE *outfile, *outfile1, *outfile2;
  char *file, *outfile_delvmin;
  file = malloc( ( 10 + strlen(argv[1]) + strlen(argv[2]) + strlen(argv[3]) ) * sizeof(char) );
  sprintf(file, "Optimum_%s_%s_%s", argv[1], argv[2],argv[3] );
  outfile = fopen(file, "w");
  //Create initial conditions
  double *y0 = malloc(sizeof(double)*8);
  y0[0] = 2.1855*pow(10,8);
  y0[1] = 2.6046*pow(10,8);
  y0[2] = 2.8341*pow(10,8);
  y0[3] = 2.597*pow(10,8);
  y0[4] = 642.7876;
  y0[5] = 766.0444;
  y0[6] = -683.7457;
  y0[7] = 746.1774;
  //Create struct vector, assign initial conditions
  y.xs = y0[0];
  y.ys = y0[1];
  y.xm = y0[2];
  y.ym = y0[3];
  y.vxs = y0[4];
  y.vys = y0[5];
  y.vxm = y0[6];
  y.vym = y0[7];
  double * delV_min;
  int er;
  //Choose objective 
    if(objective == 1){
	    outfile1 = fopen("outfile1","w");
	    delV_min = delVmin_opt(y0, y,clearance,accuracy, outfile1, rE);
	    fclose(outfile1);
	    y0[4] += delV_min[0];
  	    y0[5] += delV_min[1];
            er = integrator(y0, outfile, clearance, 1);
	    free(delV_min);
    }
    else if(objective == 2){
	    outfile2 = fopen("outfile2","w");
	    delV_min = delVtime_opt(y0, y,clearance,accuracy, outfile2, rE);
	    fclose(outfile2);
	    y0[4] += delV_min[0];
  	    y0[5] += delV_min[1];
	    er = integrator(y0, outfile, clearance, 1);
	    free(delV_min);
    }
free(name);
free(file);
fclose(outfile);
free(y0);
return 0;
}
//---------------------------------END OF MAIN--------------------------------------------

//---------------------------------INTEGRATOR--------------------------------------------
int integrator(double *y0, FILE * outfile, double clearance, int cond) {
//If cond=1, then integrator will print stuff. otherwise skip that
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
//---------------------------------DYDT FUNCTON--------------------------------------------
struct dydt_type dydtfun(struct dydt_type dydt, struct y_type y, double * masses){

double mE = masses[0];
double mM = masses[1];
double mS = masses[2];

double RSx = y.xs;// ->xs;//location of spacecraft in relation to Earth
double RSy = y.ys;
double RMx = y.xm;//location of moon in relation to Earth
double RMy = y.ym;

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
dydt.xs_dot = y.vxs;
dydt.ys_dot = y.vys;
dydt.xm_dot = y.vxm;
dydt.ym_dot = y.vym;
dydt.vxs_dot = (FMSx + FESx)/mS;
dydt.vys_dot = (FMSy + FESy)/mS;
dydt.vxm_dot = (FEMx - FMSx)/mM;
dydt.vym_dot = (FEMy - FMSy)/mM;
return dydt;
}
//---------------------------------FORCES--------------------------------------------
double *forces (double RAx, double RAy, double RBx, double RBy, double mA, double mB) {
   double G = 6.674*pow(10,-11);
   double dAB = sqrt( pow((RBx-RAx),2) + pow((RBy-RAy),2) );
   double *FAB = malloc(2*sizeof(double));
   FAB[0] = G*mA*mB*(RAx-RBx)/(pow(dAB,3));
   FAB[1] = G*mA*mB*(RAy-RBy)/(pow(dAB,3));
   return FAB;
}
//---------------------------------OPTIMIZATION(objective 1)---------------------------------------
double * delVmin_opt(double *y0, struct y_type y,double clearance,double accuracy, FILE * outfile1, double rE){
    int cond=0; //We dont want integrator to put anything in text file
    int er;
    double delVx,delVy,delV_mag,delVx_temp,delVy_temp;
    double delV_temp=100;
    double * yte = malloc(sizeof(double)*8);
    yte[0] = y0[0];
    yte[1] = y0[1];
    yte[2] = y0[2];
    yte[3] = y0[3];
    yte[6] = y0[6];
    yte[7] = y0[7];
    for (delVx=-100/sqrt(2);delVx<=100/sqrt(2);delVx+=accuracy){
        yte[4] = y0[4] + delVx;
        for (delVy=-100/sqrt(2);delVy<=100/sqrt(2);delVy+=accuracy){
            //alter initial conditions
            yte[5] = y0[5] + delVy;
            er = integrator(yte, outfile1, clearance, cond);// calculate trajectpory
            if(er == 2){ //discard if not returned to earth
                delV_mag = sqrt(pow(delVx,2) + pow(delVy,2)); //calculate magnitude of delta v
                if (delV_mag <= delV_temp){//if delta V is smaller than guess, set guess to the delta V
                    delV_temp = delV_mag;
                    delVx_temp = delVx;
		    delVy_temp = delVy;
                }
            }
        }
    }
    free(yte);
    fprintf(outfile1,"Minimum change in velocity to get to Earth is %.4f [m/s]\n", delV_temp);
    double *delV = malloc(sizeof(double)*2);
    delV[0] = delVx_temp;
    delV[1] = delVy_temp;
    return delV;
}
//---------------------------------OPTIMIZATION(objective 2)---------------------------------------
double * delVtime_opt(double *y0, struct y_type y,double clearance,double accuracy, FILE * outfile2, double rE){
    int cond=0; //We dont want integrator to put anything in text file
    int er;
    double delVx,delVy,delV_mag,delVx_temp,delVy_temp;
    double delV_temp=3000;
    double * yte = malloc(sizeof(double)*8);
    yte[0] = y0[0];
    yte[1] = y0[1];
    yte[2] = y0[2];
    yte[3] = y0[3];
    yte[6] = y0[6];
    yte[7] = y0[7];
    for (delVx=-100/sqrt(2);delVx<=100/sqrt(2);delVx+=accuracy){
        yte[4] = y0[4] + delVx;
        for (delVy=-100/sqrt(2);delVy<=100/sqrt(2);delVy+=accuracy){
            //alter initial conditions
            yte[5] = y0[5] + delVy;
            er = integrator(yte, outfile2, clearance, cond);// calculate trajectpory
            if(er == 2){ //discard if not returned to earth
                delV_mag = sqrt(pow(delVx,2) + pow(delVy,2)); //calculate magnitude of delta v
                if (delV_mag <= delV_temp){//if delta V is smaller than guess, set guess to the delta V
                    delV_temp = delV_mag;
                    delVx_temp = delVx;
		    delVy_temp = delVy;
                }
            }
        }
    }
    free(yte);
    fprintf(outfile2,"Change in velocity to get to Earth in fastest time is %.4f [m/s]\n", delV_temp);
    double *delV = malloc(sizeof(double)*2);
    delV[0] = delVx_temp;
    delV[1] = delVy_temp;    
    return delV;
}
//--------------------------RENAME----------------------------------------------------
char * myrename(char *name){
  const char pt = '.';
  char *ptr = strchr(name,pt);
  if(ptr == NULL) {
    char * new = name;
    return new;
  }
  else {
    *ptr = 'p';
    return name;
  }
}