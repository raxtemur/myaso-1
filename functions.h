#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include<QtGlobal>
#include<math.h>

double f_0(double x);
double f_1(double x);
double f_2(double x);
double f_3(double x);
double f_4(double x);
double f_5(double x);
double f_6(double x);

double Df_0(double x);
double Df_1(double x);
double Df_2(double x);
double Df_3(double x);
double Df_4(double x);
double Df_5(double x);
double Df_6(double x);

double Pf1(double x, double *C);
double Pf2(double x, double *C);//C - coeffs

#endif // FUNCTIONS_H
