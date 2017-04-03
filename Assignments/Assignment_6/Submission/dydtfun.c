struct dydt_type dydtfun(struct dydt_type dydt, struct y_type y, double * masses){

// allocating memory for the derivatives vector

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
