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
  double xs;
  double ys;
  double xm;
  double ym;
  double vxs;
  double vys;
  double vxm;
  double vym;
};
struct y_type y;
struct y_type ytemp;

double *forces(double RAx, double RAy, double RBx, double RBy, double mA, double mB);
struct dydt_type dydtfun(struct dydt_type dydt, struct y_type y, double * masses);
