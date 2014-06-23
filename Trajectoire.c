#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_min.h>
#include <gsl/gsl_bspline.h>
#include <gsl/gsl_deriv.h>

#define STEP 0.001

struct Position {double x,y;};

void getTrajectoirePoint(double t, double *x, double *y)
{
    if(t<0.0)
        t=0.0;
    if(t>1.0)
        t=1.0;

    *x = cos(t);
    *y = sin(t);
}

double functionToMinimize(double t, void* params)
{
    double x0,y0;
    struct Position *pos = (struct Position *)params;
    x0 = pos->x;
    y0 = pos->y;

    double x,y;


    getTrajectoirePoint(t,&x,&y);

    double r = sqrt(pow(x0-x,2)+pow(y0-y,2));

    printf("%lf %lf\n",t, r);
    return r;

}

double xt(double t, void* params)
{
    double x,y;

    getTrajectoirePoint(t,&x,&y);
    
    return x;
}

double yt(double t, void* params)
{
    double x,y;

    getTrajectoirePoint(t,&x,&y);
    
    return y;
}

double deriv(double t, double (*f)(double, void*))
{
    
    gsl_function F;
    F.function = f;
    F.params = NULL;
    
    double r, a;
    
    if(t<STEP)
        gsl_deriv_forward (&F, t, STEP, &r,&a);
    else if(t>1.0-STEP)
        gsl_deriv_backward (&F, t, STEP, &r,&a);
    else
        gsl_deriv_central (&F, t, STEP, &r,&a);
    
    return r;
}

double dx(double t, void* params)
{
    return deriv(t,&xt);
}
double dy(double t, void* params)
{
    return deriv(t,&yt);
}

double d2x(double t, void* params)
{
    return deriv(t,&dx);
}
double d2y(double t, void* params)
{
    return deriv(t,&dy);
}

double courbure(double t, void *params)
{
    double _dx = dx(t,NULL);
    double _dy = dy(t,NULL);
    double _d2x = d2x(t,NULL);
    double _d2y = d2y(t,NULL);
    
    return (_dx*_d2y - _dy*_d2x)/pow(_dx*_dx + _dy*_dy,1.5);
}

double getDistanceFrom(double *t, double x, double y)
{
    const gsl_min_fminimizer_type * T = gsl_min_fminimizer_goldensection;
    gsl_min_fminimizer * s = gsl_min_fminimizer_alloc (T);

    gsl_function F;
    struct Position pos = {x, y};
    F.function = &functionToMinimize;
    F.params = &pos;

    // On test si le point recherch� est aux extr�mit�s
    // On initialise la valeur minimale de d�part
    double distExtr0,distExtrStep0;
    double m;
    distExtr0 = functionToMinimize(0.0, &pos);
    distExtrStep0 = functionToMinimize(STEP, &pos);
    if(distExtr0<distExtrStep0)
    {
        *t=0;
        return distExtr0;
    }

    double distExtr1,distExtrStep1;
    distExtr1 = functionToMinimize(1.0, &pos);
    distExtrStep1 = functionToMinimize(1.0-STEP, &pos);
    if(distExtr1<distExtrStep1)
    {
        *t=1.0;
        return distExtr1;
    }else{
            if(distExtrStep0==distExtrStep1 && distExtr1==distExtr0) //Cas ou on est au cetre d'un cercle
            {
                *t=0;
                return distExtr0;
            }
        m=(distExtrStep0<distExtrStep1)?STEP:1-STEP;
    }



    gsl_min_fminimizer_set (s,&F, m,0.0,1.0);


    int iter = 0;
    int max_iter = 100;
    int status;
    double a,b;
    do
    {
        iter++;
        status = gsl_min_fminimizer_iterate (s);

        m = gsl_min_fminimizer_x_minimum (s);
        a = gsl_min_fminimizer_x_lower (s);
        b = gsl_min_fminimizer_x_upper (s);

        status
            = gsl_min_test_interval (a, b, 0.001, 0.0);

        if (status == GSL_SUCCESS)
            printf ("Converged:\n");

        printf ("%i [%.7f, %.7f] "
                "%.7f %.7f\n",
                iter, a, b,
                m, b - a);
    }
    while (status == GSL_CONTINUE && iter < max_iter);


    *t = m;
    gsl_min_fminimizer_free (s);

    return functionToMinimize(*t,&pos);
}


double getCourbure(double t)
{
    return courbure(t,NULL);
}

double getDerivCourbure(double t)
{
    return deriv(t,courbure);
}

double getAngle(double t)
{
    return atan2(dy(t,NULL),dx(t,NULL));
}


