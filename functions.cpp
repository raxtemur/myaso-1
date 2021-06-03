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

double difdif(double f1, double f2, double x1, double x2)
{
    return (f2-f1)/(x2-x1);
}

double Pf1(double x, double x1, double x2, double *C, int i)
{
    return C[4*i+0] + C[4*i+1]*(x-x1) + C[4*i+2]*(x-x1)*(x-x1) + C[4*i+3]*(x-x1)*(x-x1)*(x-x2);
}

double Pf2(double x, double x1, double x2, double *C, int i)
{
    return C[4*i+0] + C[4*i+1]*(x-x1) + C[4*i+2]*(x-x1)*(x-x1) + C[4*i+3]*(x-x1)*(x-x1)*(x-x1);
}

void coeffsErmit(int n, double *X, double *F, double *DF, double *C)
{
    for (int i = 0; i < n; i++)
    {
        C[4*i] = F[i];
        C[4*i+1] = DF[i];
        C[4*i+2] = difdif(DF[i], difdif(F[i], F[i+1], X[i], X[i+1]), X[i], X[i+1]);
        C[4*i+3] = (DF[i] + DF[i+1] - 2*difdif(F[i], F[i+1], X[i], X[i+1]))/((X[i+1] - X[i])*(X[i+1] - X[i]));
    }
}

void coeffsSlpine(int n, double *X, double *F, double *DF, double *C, double x00, double xn1, double fx00, double fxn1)
{
    double *m = new double[3*(n+1)];
    double *b = new double[n+1];
    double k;
    Q_UNUSED(fx00);
    Q_UNUSED(fxn1);

    m[0] = (X[1] - x00)*(X[1] - x00);
    m[1] = (x00 - X[1])*(x00 - X[0]);
    m[2] = 0;
    b[0] = difdif(F[0], F[1], X[0], X[1])*(x00 - X[0])*(2*(x00 - X[0])-3*(X[1]-X[0]));

    m[3*n]     = 0;
    m[3*n + 1] = (xn1 - X[n])*(xn1 - X[n]);
    m[3*n + 2] = (xn1 - X[n-1])*(xn1 - X[n]);
    b[n] = difdif(F[n-1], F[n], X[n-1], X[n])*(xn1 - X[n-1])*(2*(xn1 - X[n-1])-3*(X[n]-X[n-1]));
    //кажется что либо предположения сами по себе плохо работают, либо плохо выведено

    for (int i=1; i<n; i++)
    {
        m[3*i]     = X[i-1] - X[i];
        m[3*i + 1] = 2*(X[i+1] - X[i-1]);
        m[3*i + 2] = X[i] - X[i-1];
        b[i] = 3 * difdif(F[i-1], F[i], X[i-1], X[i]);
    }

    for (int i=0; i<n; i++)
    {
        k = m[3*(i+1)]/m[3*i + 1];
        //m[3*(i+1)] -= k*m[3*i + 1];
        m[3*(i+1)+1] -= k*m[3*i + 2];
        b[i+1] -= k*b[i];
    }

    for (int i=n; i>0; i--)
    {
        k = m[3*(i-1)+2]/m[3*i + 1];
        m[3*(i-1)+2] -= k*m[3*i + 1];
        //m[3*(i-1)+1] -= k*m[3*i];
        b[i-1] -= k*b[i];
    }

    for(int i=0; i<n; i++)
    {
        C[4*i]     = F[i];
        C[4*i + 1] = b[i];
        C[4*i + 2] = (3*difdif(F[i], F[i+1], X[i], X[i+1])- 2*b[i]-b[i+1])/(X[i+1]-X[i]);
        C[4*i + 3] = (b[i] + b[i+1] - 2*difdif(F[i], F[i+1], X[i], X[i+1]))/((X[i+1] - X[i])*(X[i+1] - X[i]));
    }

    
}
