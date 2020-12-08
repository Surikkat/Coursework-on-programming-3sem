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

    void core();//Ядро программы
    void drawGrid();//Рисование сетки
    void getData();//Получение данных
    void draw();//Рисование клеток
    void switchArray();//Смена поколения

    void on_loadFile_clicked();//Нажатие на кнопку загрузки файла
    void on_step_by_step_clicked();//Пошаговая симуляция
    void on_start_clicked();//Нажатие на кнопку запуска симуляции
    void on_stop_clicked();//Остановка симуляции

private:

    Ui::MainWindow *ui;
    int ** array;//Массив, хранящий все значения карты
    int ** newArray;//Новый массив, хранящий значения новой карты сл поколения
    QString sAlive;//количество "живых" соседей, при котором клетка остается "в живых".
    QString sRevival;//количество "живых" соседей при котором "мертвая" клетка становится "живой"
    int alive;//Количество соседей, при которых клетка остаётся живой
    int revival;//Количество соседей для оживления мёртвой клетки
    QTimer *timer;//Время каждого шага
    QPixmap *pm;//Растровое изображение (поле)
    QPainter *painter;//Рисовалка
    QString fileName;//Имя файла ввода данных
    int lifetime;//Время жизни клетки
    int sizeGrid;//Размер поля
    bool haveData;//Полученны ли данные?
};

#endif // MAINWINDOW_H
