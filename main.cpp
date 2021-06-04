
#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>

#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow *window = new QMainWindow;
    QMenuBar *tool_bar = new QMenuBar(window);
    Window *graph_area = new Window(window);
    QAction *action;

    if (graph_area->parse_command_line(argc, argv)) {
        qWarning("Wrong input arguments!");
        //       QMessageBox::warning (0, "Wrong input arguments!",
        //                             "Wrong input arguments!");
        return -1;
    }

    if (1) {
        action = tool_bar->addAction("&Change mode", graph_area,
                                     SLOT(change_mode()));
        action->setShortcut(QString("Ctrl+0"));

        action = tool_bar->addAction("&Change function", graph_area,
                                     SLOT(change_func()));
        action->setShortcut(QString("Ctrl+1"));

        action = tool_bar->addAction("&X-Scale up", graph_area,
                                     SLOT(scale_up()));
        action->setShortcut(QString("Ctrl+2"));

        action = tool_bar->addAction("&XScale down", graph_area,
                                     SLOT(scale_down()));
        action->setShortcut(QString("Ctrl+3"));

        action = tool_bar->addAction("&N*=2", graph_area,
                                     SLOT(increase_accuracy()));
        action->setShortcut(QString("Ctrl+4"));

        action = tool_bar->addAction("&N*=0.5", graph_area,
                                     SLOT(decrease_accuracy()));
        action->setShortcut(QString("Ctrl+5"));

        action = tool_bar->addAction("&p+=0.1", graph_area,
                                     SLOT(increase_distrub()));
        action->setShortcut(QString("Ctrl+6"));

        action = tool_bar->addAction("&p-=0.1", graph_area,
                                     SLOT(decrease_distrub()));
        action->setShortcut(QString("Ctrl+7"));

        action = tool_bar->addAction("&acc_mode", graph_area,
                                     SLOT(change_acc_mode()));
        action->setShortcut(QString("Ctrl+9"));

        action = tool_bar->addAction("E&xit", window, SLOT(close()));
        action->setShortcut(QString("Ctrl+X"));
    }


    tool_bar->setMaximumHeight(30);

    window->setMenuBar(tool_bar);
    window->setCentralWidget(graph_area);
    window->setWindowTitle("Graph");

    window->show();
    return app.exec();
}
