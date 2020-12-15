#include "mainwindow.h"
#include "ui_mainwindow.h"

//Конструктор
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileName = "file.txt";//Файл данных
    pm = new QPixmap(QSize(ui->label->width(), ui->label->height()));//Создает растровое изображение заданного размера QSize
    painter = new QPainter(pm);//Создаём "рисовалку"
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

    //Если данные не полученны, то пытаемся их получить
    if(!haveData) {
        getData();
        haveData = true;
    }

    draw();//Графический вывод
    core();//Ядро программы(логика выполнения)
}

//Рисование "сетки"
void MainWindow::drawGrid()
{
    //Рисуем клетки
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
    painter->setPen(QColor(105, 105, 105));//Устанавливаем перу тёмно-серый цвет
    drawGrid();//Рисуем сетку

    //Процесс отметки мертвых и живых клеток
    for(int i = 0; i < sizeGrid; i++)
        for(int j = 0; j < sizeGrid; j++)
        {
            //Если соответствующий элемент массива "мертвый" устанавливаем чёрный цвет
            if (array[i][j] == 0) painter->setBrush (QColor(0, 0, 0));
            //Если соответствующий элемент массива "живой", то устанавливаем жёлтый цвет
            else{
                painter->setBrush (QColor(255, 255, 0));}

            //Закрашиваем клетки
            painter->drawRect(i*(ui->label->width())/sizeGrid, j*(ui->label->height())/sizeGrid, (ui->label->width())/sizeGrid + 1, (ui->label->height())/sizeGrid + 1);
        }
    //Обновляем растровое изображение
    ui->label->setPixmap(*pm);
}

//Получение данных
void MainWindow::getData()
{
    alive1 = 3;//Количество клеток для жизни/рождения новой клетки
    alive2 = 2;//Количество клеток для жизни

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
            if(list[j].toInt() == 1){array[j][i]=1;}// метка, что клетка живая
            else array[j][i] = 0;//Заношу умирающие и мёртвые клетки в массив
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
void MainWindow::core()
{
    int Malive;//Живые клетки

    //Перебираем все элементы поля
    for(int i = 0; i < sizeGrid; i++)
    {
        for(int j = 0; j < sizeGrid; j++)
        {
            Malive = 0;//Живых соседей

            //Выясняем сколько живых соседей у клетки
            if(i != 0 && j != 0 && array[i - 1][j - 1] == 1 ) { Malive++; }
            if(i != 0 && array[i - 1][j] == 1 ) { Malive++; }
            if(i != 0 && j != sizeGrid - 1 && array[i - 1][j + 1] == 1 ) { Malive++; }
            if(j != 0 && array[i][j - 1] == 1 ) { Malive++; }
            if(j != sizeGrid - 1 && array[i][j + 1] == 1 ) { Malive++; }
            if(i != sizeGrid - 1 && j != 0 && array[i + 1][j - 1] == 1 ) { Malive++; }
            if(i != sizeGrid - 1 && array[i + 1][j] == 1 ) { Malive++; }
            if(i != sizeGrid - 1 && j != sizeGrid - 1 && array[i + 1][j + 1] == 1 ) { Malive++; }

            bool test1 = false;//Достаточно ли у клетки соседей, чтобы она не умерла

            //Если количество живых соседей у клетки соответствует значению при котором клетка не умирает
            if ((Malive == alive1) || (Malive ==alive2))
            {
                test1 = true;//Тест пройден
            }

            if ((test1 && array[i][j] == 1)|| array[i][j] == 0)
                newArray[i][j] = array[i][j];//Если тест пройден и клетка не умирающая или клетка уже мёртвая, то ничего не происходит
            else
                newArray[i][j] = 0;//Если тест не пройден, то убиваем клетку

            bool test2 = false;//Достаточно ли у клетки соседей, чтобы она родилась

            //Если количество живых соседей у клетки соответствует значению при котором клетка рождается
            if (Malive == alive1)
            {
                test2 = true;//то тест пройден
            }

            //Если тест пройден и клетка мёртвая
            if (test2 && array[i][j] == 0)
                newArray[i][j] = 1;//То оживляем её с меткой живой клетки
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



