#ifndef WINDOW_H
#define WINDOW_H

#include"functions.h"

#include <QWidget>


class Window : public QWidget
{
    Q_OBJECT

  private:
    int func_id;
    QString f_name;
    double a, b, a0, b0;
    double min_y, max_y;
    int n, mode;

    double (*f)(double);
    double (*df)(double);

    double *X, *F, *DF;
    double *coeffs1;
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

  protected:
    void paintEvent(QPaintEvent *event);
    void calculateMinMax();
    void sourceGraph(bool drawGr, QPainter *painter);
    void approximationGraph1(QPainter *painter);
    void initGrid();
};

#endif
