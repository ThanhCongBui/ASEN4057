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
struct y_type ytemp1;
struct y_type ytemp2;

//FUNCTION PROTOTYPES
double * delVmin_opt(double *y0, struct y_type y,double clearance,double accuracy, FILE * outfile);
double * delVtime_opt(double *y0, struct y_type y,double clearance,double accuracy,FILE * outfile);
int integrator(double *y0, FILE * outfile, double clearance, int cond);
struct dydt_type dydtfun(struct dydt_type dydt, struct y_type y, double * masses);
double *forces(double RAx, double RAy, double RBx, double RBy, double mA, double mB);
void myrename(char *name);

int main(int argc, char *argv[]){
    //Take out arguments
    int objective = atoi(argv[1]);
    double clearance = atof(argv[2]);
    double accuracy = atof(argv[3]);
    //Rename if there's a "." in argument
    char * name = malloc(sizeof(char)*strlen(argv[3]));
    sprintf(name, "%s", argv[3]);
    myrename(name);
    //Create output file
    FILE *outfile;
    char *file;
    file = malloc( ( 10 + strlen(argv[1]) + strlen(argv[2]) + strlen(argv[3]) ) * sizeof(char) );
    sprintf(file, "Optimum_%s_%s_%s", argv[1], argv[2], name);
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
    //Set up for optimizer and choose objective
    double * delV_min;
    int er;
    int cond = 1; //We want integrator to write optimum data to a file
    if(objective == 1){
        delV_min = delVmin_opt(y0, y,clearance,accuracy, outfile);
        y0[4] += delV_min[0];
        y0[5] += delV_min[1];
        er = integrator(y0, outfile, clearance, cond);
        free(delV_min);
    }
    else if(objective == 2){
        delV_min = delVtime_opt(y0, y,clearance,accuracy, outfile);
        y0[4] += delV_min[0];
        y0[5] += delV_min[1];
        er = integrator(y0, outfile, clearance, cond);
        free(delV_min);
    }
    //Clean up
    free(name);
    free(file);
    fclose(outfile);
    free(y0);
    return 0;
}

double * delVmin_opt(double *y0, struct y_type y,double clearance,double accuracy, FILE * outfile){
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
            er = integrator(yte, outfile, clearance, cond);// calculate trajectpory
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
    //print result to file
    FILE * outfile1;
    outfile1 = fopen("Vmin", "w");
    fprintf(outfile1,"Minimum change in velocity to get to Earth is %.4f [m/s]\n", delV_temp);
    fprintf(outfile1,"[ %.4f, %.4f] m/s\n",delVx_temp,delVy_temp);
    //return deltaV
    double *delV = malloc(sizeof(double)*2);
    delV[0] = delVx_temp;
    delV[1] = delVy_temp;
    return delV;
}

double * delVtime_opt(double *y0, struct y_type y,double clearance,double accuracy,FILE * outfile){
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
            er = integrator(yte, outfile, clearance, cond);// calculate trajectpory
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
    //print result to file
    FILE * outfile2;
    outfile2 = fopen("Vtime", "w");
    fprintf(outfile2,"Change in velocity to get to Earth in fastest time is %.4f [m/s]\n", delV_temp);
    fprintf(outfile2,"[ %.4f, %.4f] m/s\n",delVx_temp,delVy_temp);
    //return deltaV
    double *delV = malloc(sizeof(double)*2);
    delV[0] = delVx_temp;
    delV[1] = delVy_temp;
    return delV;
}

int integrator(double *y0, FILE * outfile, double clearance, int cond) {
//If cond=1, then integrator will print stuff. otherwise skip that
    //Create struct vector, assign initial conditions
    y.xs = y0[0];
    y.ys = y0[1];
    y.xm = y0[2];
    y.ym = y0[3];
    y.vxs = y0[4];
    y.vys = y0[5];
    y.vxm = y0[6];
    y.vym = y0[7];
    //Predeclaring variables for RK45
    double k1_xs, k1_ys, k1_xm, k1_ym, k1_vxs, k1_vys, k1_vxm, k1_vym;
    double k2_xs, k2_ys, k2_xm, k2_ym, k2_vxs, k2_vys, k2_vxm, k2_vym;
    double k3_xs, k3_ys, k3_xm, k3_ym, k3_vxs, k3_vys, k3_vxm, k3_vym;
    double k4_xs, k4_ys, k4_xm, k4_ym, k4_vxs, k4_vys, k4_vxm, k4_vym;
    double k5_xs, k5_ys, k5_xm, k5_ym, k5_vxs, k5_vys, k5_vxm, k5_vym;
    double k6_xs, k6_ys, k6_xm, k6_ym, k6_vxs, k6_vys, k6_vxm, k6_vym;
    double diffxs, diffys, diffxm, diffym, diffvxs, diffvys, diffvxm, diffvym;
    double R, delta;
    double dMS,dES,dEM;
    double rM = 1737100;
    double rE = 6371000;
    double * masses = malloc(sizeof(double)*3);
    masses[0] = 5.9722*pow(10,24);
    masses[1] = 7.342*pow(10,22);
    masses[2] = 28.833;
    double eps = 0.01;
    double t = 0;
    double tspan = 1000000;
    double h = 10;
    int er = 0;
    //Using RK45 method to get velocities and positions
/*-----------------------------------------------------------------*/
while (t <= tspan) {
    // First slopes (k1), y is evaluated at t
    dydt = dydtfun(dydt, y, masses);
    k1_xs = dydt.xs_dot;
    k1_ys = dydt.ys_dot;
    k1_xm = dydt.xm_dot;
    k1_ym = dydt.ym_dot;
    k1_vxs = dydt.vxs_dot;
    k1_vys = dydt.vys_dot;
    k1_vxm = dydt.vxm_dot;
    k1_vym = dydt.vym_dot;
    // Estimate of first state value at t=t+h/4 using k1
    ytemp1.xs = y.xs + k1_xs * h / 4.0;
    ytemp1.ys = y.ys + k1_ys * h / 4.0;
    ytemp1.xm = y.xm + k1_xm * h / 4.0;
    ytemp1.ym = y.ym + k1_ym * h / 4.0;
    ytemp1.vxs = y.vxs + k1_vxs * h / 4.0;
    ytemp1.vys = y.vys + k1_vys * h / 4.0;
    ytemp1.vxm = y.vxm + k1_vxm * h / 4.0;
    ytemp1.vym = y.vym + k1_vym * h / 4.0;
    // Second slopes (k2)
    dydt = dydtfun(dydt, ytemp1, masses);
    k2_xs = dydt.xs_dot;
    k2_ys = dydt.ys_dot;
    k2_xm = dydt.xm_dot;
    k2_ym = dydt.ym_dot;
    k2_vxs = dydt.vxs_dot;
    k2_vys = dydt.vys_dot;
    k2_vxm = dydt.vxm_dot;
    k2_vym = dydt.vym_dot;
    // Estimate of state values at t=t+3h/8 using k2
    ytemp1.xs = y.xs + k2_xs * 3.0 * h / 8.0;
    ytemp1.ys = y.ys + k2_ys * 3.0 * h / 8.0;
    ytemp1.xm = y.xm + k2_xm * 3.0 * h / 8.0;
    ytemp1.ym = y.ym + k2_ym * 3.0 * h / 8.0;
    ytemp1.vxs = y.vxs + k2_vxs * 3.0 * h / 8.0;
    ytemp1.vys = y.vys + k2_vys * 3.0 * h / 8.0;
    ytemp1.vxm = y.vxm + k2_vxm * 3.0 * h / 8.0;
    ytemp1.vym = y.vym + k2_vym * 3.0 * h / 8.0;
    // Third slopes (k3)
    dydt = dydtfun(dydt, ytemp1, masses);
    k3_xs = dydt.xs_dot;
    k3_ys = dydt.ys_dot;
    k3_xm = dydt.xm_dot;
    k3_ym = dydt.ym_dot;
    k3_vxs = dydt.vxs_dot;
    k3_vys = dydt.vys_dot;
    k3_vxm = dydt.vxm_dot;
    k3_vym = dydt.vym_dot;
    // Estimate of state values at t=t+12h/13 using k3
    ytemp1.xs = y.xs + k3_xs * 12.0 * h / 13.0;
    ytemp1.ys = y.ys + k3_ys * 12.0 * h / 13.0;
    ytemp1.xm = y.xm + k3_xm * 12.0 * h / 13.0;
    ytemp1.ym = y.ym + k3_ym * 12.0 * h / 13.0;
    ytemp1.vxs = y.vxs + k3_vxs * 12.0 * h / 13.0;
    ytemp1.vys = y.vys + k3_vys * 12.0 * h / 13.0;
    ytemp1.vxm = y.vxm + k3_vxm * 12.0 * h / 13.0;
    ytemp1.vym = y.vym + k3_vym * 12.0 * h / 13.0;
    // Fourth slopes (k4)
    dydt = dydtfun(dydt, ytemp1, masses);
    k4_xs = dydt.xs_dot;
    k4_ys = dydt.ys_dot;
    k4_xm = dydt.xm_dot;
    k4_ym = dydt.ym_dot;
    k4_vxs = dydt.vxs_dot;
    k4_vys = dydt.vys_dot;
    k4_vxm = dydt.vxm_dot;
    k4_vym = dydt.vym_dot;
    // Estimate of state values at t=t+h using k4
    ytemp1.xs = y.xs + k4_xs * h;
    ytemp1.ys = y.ys + k4_ys * h;
    ytemp1.xm = y.xm + k4_xm * h;
    ytemp1.ym = y.ym + k4_ym * h;
    ytemp1.vxs = y.vxs + k4_vxs * h;
    ytemp1.vys = y.vys + k4_vys * h;
    ytemp1.vxm = y.vxm + k4_vxm * h;
    ytemp1.vym = y.vym + k4_vym * h;
    // Fifth slopes (k5)
    dydt = dydtfun(dydt, ytemp1, masses);
    k5_xs = dydt.xs_dot;
    k5_ys = dydt.ys_dot;
    k5_xm = dydt.xm_dot;
    k5_ym = dydt.ym_dot;
    k5_vxs = dydt.vxs_dot;
    k5_vys = dydt.vys_dot;
    k5_vxm = dydt.vxm_dot;
    k5_vym = dydt.vym_dot;
    // Estimate of state values at t=t+h/2 using k5
    ytemp1.xs = y.xs + k5_xs * h / 2.0;
    ytemp1.ys = y.ys + k5_ys * h / 2.0;
    ytemp1.xm = y.xm + k5_xm * h / 2.0;
    ytemp1.ym = y.ym + k5_ym * h / 2.0;
    ytemp1.vxs = y.vxs + k5_vxs * h / 2.0;
    ytemp1.vys = y.vys + k5_vys * h / 2.0;
    ytemp1.vxm = y.vxm + k5_vxm * h / 2.0;
    ytemp1.vym = y.vym + k5_vym * h / 2.0;
    // Sixth slopes (k6)
    dydt = dydtfun(dydt, ytemp1, masses);
    k6_xs = dydt.xs_dot;
    k6_ys = dydt.ys_dot;
    k6_xm = dydt.xm_dot;
    k6_ym = dydt.ym_dot;
    k6_vxs = dydt.vxs_dot;
    k6_vys = dydt.vys_dot;
    k6_vxm = dydt.vxm_dot;
    k6_vym = dydt.vym_dot;
    // Estimates of state variables at 5th order
    ytemp1.xs = y.xs + h * (16.0 / 135.0 * k1_xs + 6656.0 / 12825.0 * k3_xs + 28561.0 / 56430.0 * k4_xs -
                           9.0 / 50.0 * k5_xs + 2.0 / 55.0 * k6_xs);
    ytemp1.ys = y.ys + h * (16.0 / 135.0 * k1_ys + 6656.0 / 12825.0 * k3_ys + 28561.0 / 56430.0 * k4_ys -
                           9.0 / 50.0 * k5_ys + 2.0 / 55.0 * k6_ys);
    ytemp1.xm = y.xm + h * (16.0 / 135.0 * k1_xm + 6656.0 / 12825.0 * k3_xm + 28561.0 / 56430.0 * k4_xm -
                           9.0 / 50.0 * k5_xm + 2.0 / 55.0 * k6_xm);
    ytemp1.ym = y.ym + h * (16.0 / 135.0 * k1_ym + 6656.0 / 12825.0 * k3_ym + 28561.0 / 56430.0 * k4_ym -
                           9.0 / 50.0 * k5_ym + 2.0 / 55.0 * k6_ym);
    ytemp1.vxs = y.vxs + h * (16.0 / 135.0 * k1_vxs + 6656.0 / 12825.0 * k3_vxs + 28561.0 / 56430.0 * k4_vxs -
                            9.0 / 50.0 * k5_vxs + 2.0 / 55.0 * k6_vxs);
    ytemp1.vys = y.vys + h * (16.0 / 135.0 * k1_vys + 6656.0 / 12825.0 * k3_vys + 28561.0 / 56430.0 * k4_vys -
                            9.0 / 50.0 * k5_vys + 2.0 / 55.0 * k6_vys);
    ytemp1.vxm = y.vxm + h * (16.0 / 135.0 * k1_vxm + 6656.0 / 12825.0 * k3_vxm + 28561.0 / 56430.0 * k4_vxm -
                            9.0 / 50.0 * k5_vxm + 2.0 / 55.0 * k6_vxm);
    ytemp1.vym = y.vym + h * (16.0 / 135.0 * k1_vym + 6656.0 / 12825.0 * k3_vym + 28561.0 / 56430.0 * k4_vym -
                            9.0 / 50.0 * k5_vym + 2.0 / 55.0 * k6_vym);
    // Estimates of state variables at 4th order
    ytemp2.xs = y.xs + h * (25.0 / 216.0 * k1_xs + 1408.0 / 2565.0 * k3_xs + 2197.0 / 4104.0 * k4_xs - 1.0 / 5.0 * k5_xs);
    ytemp2.ys = y.ys + h * (25.0 / 216.0 * k1_ys + 1408.0 / 2565.0 * k3_ys + 2197.0 / 4104.0 * k4_ys - 1.0 / 5.0 * k5_ys);
    ytemp2.xm = y.xm + h * (25.0 / 216.0 * k1_xm + 1408.0 / 2565.0 * k3_xm + 2197.0 / 4104.0 * k4_xm - 1.0 / 5.0 * k5_xm);
    ytemp2.ym = y.ym + h * (25.0 / 216.0 * k1_ym + 1408.0 / 2565.0 * k3_ym + 2197.0 / 4104.0 * k4_ym - 1.0 / 5.0 * k5_ym);
    ytemp2.vxs = y.vxs + h * (25.0 / 216.0 * k1_vxs + 1408.0 / 2565.0 * k3_vxs + 2197.0 / 4104.0 * k4_vxs - 1.0 / 5.0 * k5_vxs);
    ytemp2.vys = y.vys + h * (25.0 / 216.0 * k1_vys + 1408.0 / 2565.0 * k3_vys + 2197.0 / 4104.0 * k4_vys - 1.0 / 5.0 * k5_vys);
    ytemp2.vxm = y.vxm + h * (25.0 / 216.0 * k1_vxm + 1408.0 / 2565.0 * k3_vxm + 2197.0 / 4104.0 * k4_vxm - 1.0 / 5.0 * k5_vxm);
    ytemp2.vym = y.vym + h * (25.0 / 216.0 * k1_vym + 1408.0 / 2565.0 * k3_vym + 2197.0 / 4104.0 * k4_vym - 1.0 / 5.0 * k5_vym);
    //calculate error between 4th and 5th order and determine how to change the time step h with delta
    diffxs = pow((ytemp1.xs - ytemp2.xs), 2);
    diffys = pow((ytemp1.ys - ytemp2.ys), 2);
    diffxm = pow((ytemp1.xm - ytemp2.xm), 2);
    diffym = pow((ytemp1.ym - ytemp2.ym), 2);
    diffvxs = pow((ytemp1.vxs - ytemp2.vxs), 2);
    diffvys = pow((ytemp1.vys - ytemp2.vys), 2);
    diffvxm = pow((ytemp1.vxm - ytemp2.vxm), 2);
    diffvym = pow((ytemp1.vym - ytemp2.vym), 2);
    R = 1.00/h*sqrt(diffxs + diffys + diffxm + diffym + diffvxs + diffvys + diffvxm + diffvym);
    delta = 0.84 * pow(eps / R, 0.25);
    //print values if error tolerance is satisfied; re-calcuate otherwise
    if (R <= eps) {
        y.xs = ytemp2.xs;
        y.ys = ytemp2.ys;
        y.xm = ytemp2.xm;
        y.ym = ytemp2.ym;
        y.vxs = ytemp2.vxs;
        y.vys = ytemp2.vys;
        y.vxm = ytemp2.vxm;
        y.vym = ytemp2.vym;
        t = t + h;
        h = delta * h;
        if (h > 60000) { h = 60000; }
        if (cond == 1) {fprintf(outfile, "%.4f %.4f %.4f %.4f %.4f\n", t, y.xs, y.ys, y.xm, y.ym);}
    }
    else {
        if (h < 10) {
            y.xs = ytemp2.xs;
            y.ys = ytemp2.ys;
            y.xm = ytemp2.xm;
            y.ym = ytemp2.ym;
            y.vxs = ytemp2.vxs;
            y.vys = ytemp2.vys;
            y.vxm = ytemp2.vxm;
            y.vym = ytemp2.vym;
            t = t + h;
            h = 10;
            if (cond == 1) { fprintf(outfile, "%.4f %.4f %.4f %.4f %.4f\n", t, y.xs, y.ys, y.xm, y.ym); }
        }
        else{
            h = delta*h;
        }
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
return er;
}

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

double *forces (double RAx, double RAy, double RBx, double RBy, double mA, double mB) {
   double G = 6.674*pow(10,-11);
   double dAB = sqrt( pow((RBx-RAx),2) + pow((RBy-RAy),2) );
   double *FAB = malloc(2*sizeof(double));
   FAB[0] = G*mA*mB*(RAx-RBx)/(pow(dAB,3));
   FAB[1] = G*mA*mB*(RAy-RBy)/(pow(dAB,3));
   return FAB;
}

void myrename(char *name){
    const char pt = '.';
    char *ptr = strchr(name,pt);
    if(ptr == NULL){
    }
    else {
        *ptr = 'p';
    }
}