#include <QPainter>
#include <stdio.h>
#include<string>

#include "window.h"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 100



Window::Window(QWidget *parent) : QWidget(parent)
{
    a = DEFAULT_A;
    b = DEFAULT_B;
    a0 = DEFAULT_A;
    b0 = DEFAULT_B;
    n = DEFAULT_N;

    func_id = 0;

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

void Window::scale_up()
{
    a *= 2;
    b *= 2;
    update();
}

void Window::scale_down()
{
    a *= 0.5;
    b *= 0.5;
    update();
}

void Window::sourceGraph(bool drawGr, QPainter *painter)
{
    double x1, x2, y1, y2;
    double delta_y, delta_x = (b - a) / 200;

    delta_y = 0.01 * (max_y - min_y);

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
        y1 = f(x1);
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x) {
            y2 = f(x2);
            painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));

            x1 = x2, y1 = y2;
        }
        x2 = b;
        y2 = f(x2);
        painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }

    // draw axis
    pen.setWidth(0);
    pen.setColor("red");
    painter->setPen(pen);
    painter->drawLine(a, 0, b, 0);
    painter->drawLine(0, max_y+delta_y, 0, min_y-delta_y);
}

void Window::calculateMinMax()
{
    double x1, y1;
    double delta_x = (b - a) / n;
    max_y = min_y = 0;
    for (x1 = a0; x1 - b0 < 1.e-6; x1 += delta_x) {
        y1 = f(x1);
        if (y1 < min_y)
            min_y = y1;
        if (y1 > max_y)
            max_y = y1;
    }
}

/// render graph
void Window::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    //double x1, x2, y1, y2;
    //double delta_y, delta_x = (b - a) / n;

    // calculate min and max for current function
    this->calculateMinMax();

    // save current Coordinate System
    painter.save();

    this->sourceGraph(1, &painter);



    // restore previously saved Coordinate System
    painter.restore();

    // render function name
    painter.setPen("blue");
    painter.drawText(0, 20, f_name);
    painter.drawText(0,  40, QString::number(a, 'g', 3));
    painter.drawText(50, 40, QString::number(b, 'g', 3));
    painter.drawText(0,  60, QString::number(min_y, 'g', 3));
    painter.drawText(50,  60, QString::number(max_y, 'g', 3));
}



