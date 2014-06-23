#include "Commande.h"
#include "Trajectoire.h"
#include <math.h>

#define KP 30
#define KV 10

double getWCommande(double x, double y, double thetam, double v)
{
    
    double t0, y0,theta0,c0,g0;
    y = getDistanceFrom(&t0,x,y);
    
    c0 = getCourbure(t0);
    g0 = getDerivCourbure(t0);
    theta0 = getAngle(t0);
    
	double theta = thetam - theta0;


	double w0 = v*cos(theta) / (1 - c0*y0);
    double signeW0 = (w0>0)?1:-1;
	double w1 = y0*(g0*sin(theta) - KP*cos(theta))* cos(theta) / (1 - c0*y0);
	double w2 = sin(theta) * ( c0*sin(theta) - KV*cos(theta)*signeW0 );
	double w3 = c0;

	double w = w0*(w1 + w2 + w3);
    
    return w;
}
    