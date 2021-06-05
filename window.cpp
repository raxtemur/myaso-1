#include <QPainter>
#include <stdio.h>
#include<string>

#include "window.h"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 8
#define DEFAULT_MODE 0
#define DEF_STEPS 128
#define MODES 4

Window::Window(QWidget *parent) : QWidget(parent)
{
    a = DEFAULT_A;
    b = DEFAULT_B;
    a0 = DEFAULT_A;
    b0 = DEFAULT_B;
    n = DEFAULT_N;
    mode = DEFAULT_MODE;
    p = 0.1;
    acc_mode = 1;

    func_id = 0;

    //initGrid();


    change_func();
}

QSize Window::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize Window::sizeHint() const
{
    return QSize(1000, 1000);
}

int Window::parse_command_line(int argc, char *argv[])
{
    if (argc == 1)
        return 0;

    if (argc == 2)
        return -1;

    if (sscanf(argv[1], "%lf", &a0) != 1 || sscanf(argv[2], "%lf", &b0) != 1 ||
        b0 - a0 < 1.e-6 || (argc > 3 && sscanf(argv[3], "%d", &n) != 1) || n <= 0)
        return -2;

    a = a0;
    b = b0;
    return 0;
}

/// change current function for drawing
void Window::change_func()
{
    func_id = (func_id + 1) % 7;

    switch (func_id) {
    case 0:
        f_name = "f (x) = 1";
        f = f_0;
        df = Df_0;
        break;
    case 1:
        f_name = "f (x) = x";
        f = f_1;
        df = Df_1;
        break;
    case 2:
        f_name = "f (x) = x*x";
        f = f_2;
        df = Df_2;
        break;
    case 3:
        f_name = "f (x) = x*x*x";
        f = f_3;
        df = Df_3;
        break;
    case 4:
        f_name = "f (x) = x*x*x*x";
        f = f_4;
        df = Df_4;
        break;
    case 5:
        f_name = "f (x) = exp(x)";
        f = f_5;
        df = Df_6;
        break;
    case 6:
        f_name = "f (x) = 1/(25*x*x + 1)";
        f = f_6;
        df = Df_6;
        break;
    }

    update();
}

double Window::fp(double x)
{
    double x0 = (b+a)/2;
    if (p && (abs(x - x0) < 1.e-5))
    {
        return f(x) + p*max_y;
    }
    else
    {
        return f(x);
    }
}

void Window::change_mode()
{
    mode = (mode + 1) % MODES;
    update();
}

void Window::scale_up()
{
    if (fabs(b-a) < 10000)
    {
        a *= 2;
        b *= 2;

    }
    update();
}

void Window::scale_down()
{
    if (fabs(b-a) > 0.1)
    {
        a *= 0.5;
        b *= 0.5;
    }
    update();
}

void Window::increase_accuracy()
{
    if (n < 2048)
    {
        n*=2;
    }
    update();
}

void Window::decrease_accuracy()
{
    if (n > 4)
    {
        n*=0.5;
    }
    update();
}

void Window::increase_distrub()
{
    if (p < 1)
    {
        p+=0.1;
        update();
    }
}

void Window::decrease_distrub()
{
    if (p > -1)
    {
        p-=0.1;
        update();
    }
}

void Window::change_acc_mode()
{
    acc_mode = (acc_mode + 1)%2;
    update();
}

void Window::sourceGraph(bool drawGr, QPainter *painter)
{
    double x1, x2, y1, y2;
    double delta_y, delta_x = (b - a) / DEF_STEPS;

    delta_y = 0.1 * (max_y - min_y);

    // make Coordinate Transformations
    painter->translate(0.5 * width(), 0.5 * height());
    painter->scale(width() / (b - a), -height() / (max_y - min_y + 2*delta_y));
    painter->translate(-0.5 * (a + b), -0.5 * (min_y + max_y));

    QPen pen("black");
    pen.setWidth(0);
    painter->setPen(pen);

    // draw approximated line for graph
    if (drawGr)
    {
        x1 = a;
        y1 = fp(x1);
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x) {
            y2 = fp(x2);
            painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));

            x1 = x2, y1 = y2;
        }
        x2 = b;
        y2 = fp(x2);
        painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }

    // draw axis
    pen.setWidth(0);
    pen.setColor("red");
    painter->setPen(pen);
    painter->drawLine(a, 0, b, 0);
    painter->drawLine(0, max_y+delta_y, 0, min_y-delta_y);
}

void Window::initGrid()
{
    double delta_x = (b - a) / n;
    X = new double[n + 1];
    F = new double[n + 1];
    DF = new double[n + 1];
    coeffs1 = new double[4*n];
    coeffs2 = new double[4*n];
    for (int i = 0; i < n + 1; i++)
    {
        X[i] = a + i*delta_x;
        F[i] = fp(X[i]);
        DF[i] = df(X[i]);
    }
}

void Window::calculateMinMax()
{
    double x1, y1;
    double delta_x = (b - a) / DEF_STEPS;
    max_y = min_y = 0;
    for (x1 = a0; x1 - b0 < 1.e-6; x1 += delta_x) {
        y1 = fp(x1);
        if (y1 < min_y)
            min_y = y1;
        if (y1 > max_y)
            max_y = y1;
    }
    max_y = (max_y - min_y);
    min_y = -max_y;
}

void Window::approximationGraph1(QPainter *painter)
{
    double x1, x2, y1, y2, fy1, fy2;
    double delta_x = (b - a) / DEF_STEPS;

    coeffsErmit(n, X, F, DF, coeffs1);

    QPen pen("green");
    pen.setWidth(0.1);
    painter->setPen(pen);

    for (int i = 0; i < n; i++)
    {
        x1 = X[i];
        y1 = Pf1(x1, X[i], X[i+1], coeffs1, i);
        if (acc_mode) {fy1 = fp(x1);}
        for (x2 = x1 + delta_x; x2 - X[i+1] < 1.e-6; x2 += delta_x) {
            y2 = Pf1(x2, X[i], X[i+1], coeffs1, i);
            painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
            if (acc_mode)
            {
                fy2 = fp(x2);
                painter->drawLine(QPointF(x1, fy1 - y1), QPointF(x2, fy2 -y2));
                fy1 = fy2;
            }
            x1 = x2, y1 = y2;

        }
        x2 = X[i+1];
        y2 = Pf1(x2, X[i], X[i+1], coeffs1, i);
        painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }
}


void Window::approximationGraph2(QPainter *painter)
{
    double x1, x2, y1, y2, fy1, fy2;;
    double delta_x = (b - a) / DEF_STEPS;

    double x00, xn1, fx00, fxn1;
    x00 = a-delta_x;
    xn1 = b+delta_x;
    fx00 = f(x00);
    fxn1 = f(b+delta_x);
    coeffsSlpine(n, X, F, DF, coeffs2, x00, xn1, fx00, fxn1);

    QPen pen("orange");
    pen.setWidth(0);
    painter->setPen(pen);

    for (int i = 0; i < n; i++)
    {
        x1 = X[i];
        y1 = Pf2(x1, X[i], X[i+1], coeffs2, i);
        if (acc_mode) {fy1 = fp(x1);}
        for (x2 = x1 + delta_x; x2 - X[i+1] < 1.e-6; x2 += delta_x) {
            y2 = Pf2(x2, X[i], X[i+1], coeffs2, i);
            painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
            if (acc_mode)
            {
                fy2 = fp(x2);
                painter->drawLine(QPointF(x1, fy1 - y1), QPointF(x2, fy2 -y2));
                fy1 = fy2;
            }
            x1 = x2, y1 = y2;
        }
        x2 = X[i+1];
        y2 = Pf2(x2, X[i], X[i+1], coeffs2, i);
        painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }
}


/// render graph
void Window::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    // calculate min and max for current function
    this->calculateMinMax();

    // grid for calculations
    this->initGrid();

    // save current Coordinate System
    painter.save();

    //sourse code
    this->sourceGraph(1, &painter);

    //calculate coefss
    if (mode == 0 || mode == 2)
    {
        this->approximationGraph1(&painter);
    }

    if (mode == 1 || mode == 2)
    {
        this->approximationGraph2(&painter);
    }

    // restore previously saved Coordinate System
    painter.restore();

    // render function name
    painter.setPen("blue");
    painter.drawText(0, 20, f_name);

    painter.drawText(0, 40, "a, b:");
    painter.drawText(40,  40, QString::number(a, 'g', 3));
    painter.drawText(100, 40, QString::number(b, 'g', 3));
    painter.drawText(160, 40, "n:");
    painter.drawText(180, 40, QString::number(n));

    painter.drawText(0, 60, "min, max:");
    painter.drawText(70,  60, QString::number(min_y, 'g', 3));
    painter.drawText(120,  60, QString::number(max_y, 'g', 3));

    painter.drawText(0, 80, "mode:");
    painter.drawText(50, 80, QString::number(mode));

    painter.drawText(0, 100, "p:");
    painter.drawText(50, 100, QString::number(p));

}



