//
// Created by cong on 4/26/17.
//

#ifndef FINAL_PROJECT_APOLLO_H
#define FINAL_PROJECT_APOLLO_H
double *forces(double RAx, double RAy, double RBx, double RBy, double mA, double mB);
struct dydt_type dydtfun(struct dydt_type dydt, struct y_type y, double * masses);
int integrator(double *y0, FILE * outfile, double clearance, int cond);
double *delVmin_opt(double *y0, struct y_type y,double clearance,double accuracy, FILE * outfile1, double rE);
double *delVtime_opt(double *y0, struct y_type y,double clearance,double accuracy, FILE * outfile2, double rE);
char * myrename(char *name);
void IC(struct y_type y0);
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
#endif //FINAL_PROJECT_APOLLO_H
