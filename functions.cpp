#include"functions.h"

double f_0(double x) { Q_UNUSED(x); return 1; }
double f_1(double x) { return x; }
double f_2(double x) { return x*x; }
double f_3(double x) { return x*x*x; }
double f_4(double x) { return x*x*x*x; }
double f_5(double x) { return exp(x); }
double f_6(double x) { return 1/(25*x*x + 1); }

double Df_0(double x) { Q_UNUSED(x); return 0; }
double Df_1(double x) { Q_UNUSED(x); return 1; }
double Df_2(double x) { return 2*x; }
double Df_3(double x) { return 3*x*x; }
double Df_4(double x) { return 4*x*x*x; }
double Df_5(double x) { return exp(x); }
double Df_6(double x) { return (-50*x)/(25*x*x + 1)/(25*x*x + 1); }



double Pf1(double x, double *C)
{
    Q_UNUSED(C);
    return x;
}

