#ifndef MANUALINPUT_H
#define MANUALINPUT_H

#include <QMainWindow>
#include <QPainter>
#include <QtGui>
#include "mainwindow.h"

namespace Ui {
class manualInput;
}

class manualInput : public QMainWindow
{
    Q_OBJECT

public:
    explicit manualInput(QWidget *parent = nullptr);
    ~manualInput();

private slots:

    void drawGrid();

private:
    Ui::manualInput *ui;
};

#endif // MANUALINPUT_H
