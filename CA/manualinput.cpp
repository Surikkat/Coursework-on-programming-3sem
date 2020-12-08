#include "manualinput.h"
#include "ui_manualinput.h"

manualInput::manualInput(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::manualInput)
{

}

manualInput::~manualInput()
{
    delete ui;
}


