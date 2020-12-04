#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void drawGrid();//Рисование сетки
    void draw();//Рисование рисунка
    void getData();//Получение данных
    void switchArray();//Переключить массив
    void logic();//Логика

    void on_loadFile_clicked();//Нажатие на кнопку загрузки файла
    void on_start_clicked();//Нажатие на кнопку запуска симуляции
    void on_stop_clicked();//Остановка симуляции
    void on_step_by_step_clicked();//Пошаговая симуляция

private:

    Ui::MainWindow *ui;
    QTimer *timer;//Таймер каждого шага
    QPixmap *pm;//Растровое изображение (поле)
    QPainter *painter;//Рисовалка
    QString fileName;//Имя файла ввода данных
    QVector<int> iL;//Количество соседей, при которых клетка остаётся живой
    QVector<int> iA;//Количество соседей для оживления мёртвой клетки
    int ** array;//Массив, хранящий все значения карты
    int ** newArray;//Новый массив, обновляющий значения карты
    QString sL, sA;//sL-количество "живых" соседей, при котором клетка остается "в живых". sA-количество "живых" соседей при котором "мертвая" клетка становится "живой"
    int iC;//Время жизни клетки
    int sizeGrid;//Размер поля
    bool haveData;//Полученны ли данные?
};

#endif // MAINWINDOW_H
