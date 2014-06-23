#ifndef H_TRAJECTOIRE
#define H_TRAJECTOIRE


void getTrajectoirePoint(double t, double *x, double *y);
double functionToMinimize(double t, void* params);
double getDistanceFrom(double *t, double x, double y);

double dx(double t, void* params);


double getCourbure(double t);
double getDerivCourbure(double t);
double getAngle(double t);

#endif // H_TRAJECTOIRE
