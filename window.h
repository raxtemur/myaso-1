#ifndef WINDOW_H
#define WINDOW_H

#include"functions.h"

#include <QWidget>


class Window : public QWidget
{
    Q_OBJECT

  private:
    int func_id;
    QString f_name, mode_name;
    double a, b, a0, b0;
    double min_y, max_y;
    double func_max, func_min;
    int n, mode, acc_mode;
    double p;

    double (*f)(double);
    double (*df)(double);

    double *X, *F, *DF;
    double *coeffs1, *coeffs2;
    QAction *action;

  public:
    Window(QWidget *parent);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    int parse_command_line(int argc, char *argv[]);

  public slots:
    void change_func();
    void scale_up();
    void scale_down();
    void increase_accuracy();
    void decrease_accuracy();
    void increase_distrub();
    void decrease_distrub();
    void change_mode();
    void change_acc_mode();

  protected:
    void paintEvent(QPaintEvent *event);
    void calculateMinMax();
    void sourceGraph(bool drawGr, QPainter *painter);
    void approximationGraph1(QPainter *painter);
    void approximationGraph2(QPainter *painter);
    void initGrid();
    double fp(double x);
};

#endif
