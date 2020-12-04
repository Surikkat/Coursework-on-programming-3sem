#include "mainwindow.h"
#include "ui_mainwindow.h"

//Конструктор
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileName = "file.txt";//Файл данных
    pm = new QPixmap(QSize(ui->label->width(), ui->label->height()));//Создает растровое изображение заданного размера QSize
    painter = new QPainter(pm);//Создаём "рисовалку"
    painter->fillRect(painter->viewport(), QColor(255,255,255));//Заполняет весь видимый прямоугольник белым цветом
    ui->label->setPixmap(*pm);//Обновляем растровое изображение
    haveData = false;//Полученны ли данные?
    timer = new QTimer(this);//Создание таймера
}

//Обработка нажатия на кнопку загрузки файла
void MainWindow::on_loadFile_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath(), "txt-files (*.txt)");//Получаем файл данных выбранный пользователем
    ui->start->setEnabled(true);//Делаем кнопку start активной
    ui->step_by_step->setEnabled(true);//Делаем кнопку step_by_step активной
}

//Обработка нажатия на кнопку старта
void MainWindow::on_start_clicked()
{
    ui->loadFile->setEnabled(false);//Делаем кнопку loadFile не активной
    ui->stop->setEnabled(true);//Делаем кнопку stop активной
    connect(timer, SIGNAL(timeout()), this, SLOT(on_step_by_step_clicked()));//Когда таймер истекает автоматически нажимаем на кнопку step_by_step
    timer->start(10);//Задаём таймеру интервал 10 миллисекунд
}

//Обработка нажатия на кнопку СТОП
void MainWindow::on_stop_clicked()
{
    timer->stop();//Останавливаем таймер
}

//Обработка нажатия на кнопку step_by_step
void MainWindow::on_step_by_step_clicked()
{
    ui->loadFile->setEnabled(false);//Делаем кнопку loadFile не активной
    ui->textEdit_A->setEnabled(false);//Делаем строку ввода textEdit_A не активной
    ui->textEdit_C->setEnabled(false);//Делаем строку ввода textEdit_C не активной
    ui->textEdit_L->setEnabled(false);//Делаем строку ввода textEdit_L не активной

    //Если данные не полученны, то пытаемся их получить
    if(!haveData) { getData(); haveData = true; }
    draw();//Рисуем поле
    logic();//Обрабатываем логику
}

//Рисование "сетки"
void MainWindow::drawGrid()
{
    painter->fillRect(painter->viewport(), QColor(255,255,255));//Заполняем весь видимый прямоугольник белым цветом
    painter->setPen(QColor(0, 0, 0));//Устанавливаем перо чёрного цвета

    //Ресуем клетки
    for(int i = 0; i < sizeGrid; i++)
    {
        painter->drawLine(0, i*(ui->label->height())/sizeGrid, ui->label->width(), i*(ui->label->height())/sizeGrid);//Для каждой "строки" рисуем линию от левого края строки до правого края
        painter->drawLine(i*(ui->label->width())/sizeGrid, 0, i*(ui->label->width())/sizeGrid, ui->label->height());//Для каждого "столбца" рисуем вертикальную линию
    }
    ui->label->setPixmap(*pm);//Обновляем растровое изображение
}

//Рисование живых и мёртвых клеток
void MainWindow::draw()
{
    painter->setPen(Qt::gray);//Устанавливаем перу серый цвет

    //Процесс отметки мертвых и живых клеток
    for(int i = 0; i < sizeGrid; i++)
        for(int j = 0; j < sizeGrid; j++)
        {
            //Если соответствующий элемент массива "мертвый" устанавливаем чёрный цвет
            if (array[i][j] == 0) painter->setBrush (Qt::black);
            //Если соответствующий элемент массива "живой", то устанавливаем красный цвет
            else{
                painter->setBrush (QColor(255, 200, 0));}

            //Закрашиваем клетки
            painter->drawRect(i*(ui->label->width())/sizeGrid, j*(ui->label->height())/sizeGrid, (ui->label->width())/sizeGrid + 1, (ui->label->height())/sizeGrid + 1);
        }
    //Обновляем растровое изображение
    ui->label->setPixmap(*pm);
}

//Получение данных
void MainWindow::getData()
{
    //Получаем количество "живых" соседей, при котором клетка остается "в живых", в виде строки
    sL = ui->textEdit_L->toPlainText();
    //Получаем количество "живых" соседей при котором "мертвая" клетка становится "живой", в виде строки
    sA = ui->textEdit_A->toPlainText();

    //Переносим все цифры из строки в вектор
    for(int i = 0; i < sL.length(); i++)
    {
        iL.push_back(sL[i].toLatin1()-48);//Цифры в Latin1 имеют номера от 48 до 57. При переводе строки в цифры мы получаем номер символа в этой кодировке(т.е. число от 48 до 57), для того чтобы получить искомое число отнимаю 48
    }

    //Переносим все цифры из строки в вектор
    for(int i = 0; i < sA.length(); i++)
    {
        iA.push_back(sA[i].toLatin1()-48);//Цифры в Latin1 имеют номера от 48 до 57. При переводе строки в цифры мы получаем номер символа в этой кодировке(т.е. число от 48 до 57), для того чтобы получить искомое число отнимаю 48
    }

    iC = (ui->textEdit_C->toPlainText()).toInt();//Получаю продолжительность жизни клетки

    QVector<QString> str;//Строки, которые будут хранить данные из файла
    QFile file(fileName);//Файл с данными
    file.open(QIODevice::ReadOnly);//Открыть файл с данными для чтения
    QTextStream in(&file);//Читаю из файла
    //Пока файл не закончился
    while (!in.atEnd())
        str.push_back(in.readLine());//Заполняю строку данными из файла
    file.close();//Закрываю файл
    sizeGrid = str.size();//Получаю размер поля

    array = new int * [sizeGrid];//Массив с данными о клетках
    newArray = new int * [sizeGrid];//Массив с новыми данными о клетках

    //Делаю массивы двумерными
    for(int i = 0; i < sizeGrid; i++)
    {
        array[i] = new int[sizeGrid];
        newArray[i] = new int[sizeGrid];
    }

    for(int i = 0; i < sizeGrid; i++)
    {
        QStringList list = str[i].split(" ");//Разбиваю строку на подстроки, после каждого пробела
        //Для каждого элемента из строки
        for(int j = 0; j < sizeGrid; j++)
        {
            if(list[j].toInt() >= iC){array[j][i]=iC+1;}//iC+1 - это метка, что клетка живая(не умирающая и не мёртвая)
            else array[j][i] = list[j].toInt();//Заношу умирающие и мёртвые клетки в массив
        }
    }
}

//Смена поколений
void MainWindow::switchArray()
{
    for(int i = 0; i < sizeGrid; i++)
        for(int j = 0; j < sizeGrid; j++)
            array[i][j] = newArray[i][j];//Меняем местами массивы
}

//Логика
void MainWindow::logic()
{
    int alive;//Живые клетки и рожденные клетки

    //Перебираем все элементы поля
    for(int i = 0; i < sizeGrid; i++)
    {
        for(int j = 0; j < sizeGrid; j++)
        {
            alive = 0;//Живых соседей

            //Выясняем сколько живых соседей у клетки
            if(i != 0 && j != 0 && array[i - 1][j - 1] == iC + 1) { alive++; }
            if(i != 0 && array[i - 1][j] == iC + 1) { alive++; }
            if(i != 0 && j != sizeGrid - 1 && array[i - 1][j + 1] == iC + 1) { alive++; }
            if(j != 0 && array[i][j - 1] == iC + 1) { alive++; }
            if(j != sizeGrid - 1 && array[i][j + 1] == iC + 1) { alive++; }
            if(i != sizeGrid - 1 && j != 0 && array[i + 1][j - 1] == iC + 1) { alive++; }
            if(i != sizeGrid - 1 && array[i + 1][j] == iC + 1) { alive++; }
            if(i != sizeGrid - 1 && j != sizeGrid - 1 && array[i + 1][j + 1] == iC + 1) { alive++; }

            bool test1 = false;//Достаточно ли у клетки соседей, чтобы она умерла

            for(int k = 0; k < iL.size(); k++)
                //Если количество живых соседей у клетки соответствует хотя бы одному из значений при которых клетка не умирает
                if (alive == iL[k])
                {
                    test1 = true;//Тест пройден
                    break;
                }

            if ((test1 && array[i][j] == iC+1)|| array[i][j] == 0)
                newArray[i][j] = array[i][j];//Если тест пройден и клетка не умирающая или клетка уже мёртвая, то ничего не происходит
            else
                newArray[i][j] = array[i][j] - 1;//Если тест не пройден и клетка умирающая, но не мёртвая, то отнимаем у неё жизнь

            bool test2 = false;//Достаточно ли у клетки соседей, чтобы она родилась

            for(int k = 0; k < iA.size(); k++)
                //Если количество живых соседей у клетки соответствует хотя бы одному из значений при которых клетка рождается
                if (alive == iA[k])
                {
                    test2 = true;//то тест пройден
                    break;
                }

            //Если тест пройден и клетка свободная(мёртвая)
            if (test2 == true && array[i][j] == 0)
                newArray[i][j] = iC+1;//То оживляем её с меткой живой клетки
            //Если тест не пройден, или клетка не свободна
            else if(array[i][j] == 0)
                newArray[i][j] = 0;//то ничего не происходит
        }

    }

    switchArray();//Производим смену массивов
}

//Деструктор
MainWindow::~MainWindow()
{
    delete ui;//Удаляю интерфейс
    delete timer;//Удаляю таймер
    delete painter;//удаляю рисовалку
    delete pm;//Удаляю изображение

    //Удаляю оба массива с данными
    for(int i = 0; i < sizeGrid; i++)
    {
        delete [] array[i];
        delete [] newArray[i];
    }
    delete [] array;
    delete [] newArray;
}

