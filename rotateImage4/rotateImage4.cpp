#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

const float pi = 3.141592;
int r, c;

struct pixelRGB
{
    int blue = -1;
    int green = -1;
    int red = -1;
};
pixelRGB** pixelNew;

void descriptionProgram();//описание программы
void openCVtoMatrix(Mat image, pixelRGB**pixel);//перезапись, полученной цветовой палитры через openCV, в матрицу
void rotateImage(pixelRGB **pixel, int angle, float rad, int rows, int cols);//поворот изображения на заданный угол
Mat MatrixToOpenCV(Mat image, pixelRGB** pixelNew, int color);//перезапись полученной матрицы в OpenCV
string testingNameImage(string name);//ввод корректного наименования изображения
string testingImageFormat(string type);//проверка на корректность формата изображения

int main()
{
    setlocale(LC_ALL, "rus");

    descriptionProgram();

    string nameImage;
    cout << "Введите наименование изображения: ";
    getline(cin, nameImage);
    Mat loadImage = imread(nameImage, IMREAD_COLOR);
    while (loadImage.empty() || loadImage.size().width > 1200 || loadImage.size().height > 1200)
    {
        if (loadImage.empty())
            cout << "Изображение не найдено!" << endl;
        else
            cout << "Разрешение изображения превышает допустимое значение" << endl;
        cout << "Наименование должно соотвествовать типу - << имя.формат >>. Изображение не должно превышать 1200 х 1200 пикселей." << endl;
        cout << "Повторите ввод: ";
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        getline(cin, nameImage);
        loadImage = imread(nameImage, IMREAD_COLOR);
    }
    cout << endl;
    int rows = loadImage.rows;
    int cols = loadImage.cols;
    pixelRGB** pixel = new pixelRGB * [rows];//строки
    for (int i = 0; i < rows; i++)
        pixel[i] = new pixelRGB[cols];//столбцы

    openCVtoMatrix(loadImage, pixel);

    char direction;
    cout << "Задайте направление поворота. Против часовой << - >>, по часовой << + >>: ";
    cin >> direction;
    while (direction != '-' && direction != '+')
    {
        cout << "Неверный ввод!Введите << - >> или << + >>: ";
        cin.clear();
        cin.ignore(1251, '\n');
        cin >> direction;
    }
    cin.ignore(cin.rdbuf()->in_avail());
    
    int angle;
    cout << "Введите угол наклона изображения: ";
    while (!(cin >> angle && angle >= 0 && angle <= 360))
    {
        cout << "Неверный ввод!Введите угол от 0 до 360 градусов: ";
        cin.clear();
        cin.ignore(1251, '\n');
    }
    cin.ignore(cin.rdbuf()->in_avail());

    if(direction == '+')
        angle = 360 - angle;
    float rad = (angle * (pi / 180));

    rotateImage(pixel, angle, rad, rows, cols);

    int color = -1;
    if (angle != 0 && angle != 90 && angle != 180 && angle != 270 && angle != 360)
    {
        cout << "Если желаете изменить цвет фона на черный, то введите << 0 >>. Иначе, введите << 1 >> ." << endl;
        cout << "*По умолчанию цвет фона изображения будет белым." << endl;
        cout << "Ввод: ";
        cin >> color;
        while (cin.fail() || (color != 0 && color != 1))
        {
            cout << "Неверный ввод!Введите << 0 >> или << 1 >> : ";
            cin.clear();
            cin.ignore(1251, '\n');
            cin >> color;
        }
        cin.ignore(cin.rdbuf()->in_avail());
        if (color == 0)
            cout << "Цвет фона изображения изменен на черный" << endl;
        else
            cout << "Цвет фона - белый" << endl;
    } 
    cout << "Для продолжения закройте изображение (Нажмите крестик)" << endl;

    Mat rotateImage = MatrixToOpenCV(loadImage, pixelNew, color);

    imshow("Повернутое изображение", rotateImage);
    waitKey(0);

    int num;
    cout << "Желаете сохранить полученное изображение на компьютер? Нажмите << 1 >> - да, << 2 >> - нет :";
    cin >> num;
    while (num != 1 && num != 2)
    {
        cout << "Неверный ввод!Нажмите << 1 >> - да, << 2 >> - нет: ";
        cin.clear();
        cin.ignore(1251, '\n');
        cin >> num;
    }
    cin.ignore(cin.rdbuf()->in_avail());
    if (num == 1)
    {
        cout << "Введите наименование изображения без указания формата (<< имя >>): ";
        cin >> nameImage;
        nameImage = testingNameImage(nameImage);
        cout << endl;
        string format;
        cout << "Введите расширение изображения ( << формат >> ): ";
        cin >> format;
        format = testingImageFormat(format);
        nameImage = nameImage + "." + format;
        imwrite(nameImage, rotateImage);
        cout << "Изображение успешно сохранено на компьютер!" << endl;
    }

    cout << endl << "Выход из программы" << endl;

    // удаление двумерного динамического массива
    for (int i = 0; i < rows; i++)
        delete[]pixel[i];
    for (int i = 0; i < r; i++)
        delete[]pixelNew[i];

    return 0;
}
void descriptionProgram()
{
    cout << "========== Поворот изображения на произвольный угол ==========" << endl;
    cout << "Данная программа осуществляет поворот изображения на угол от 0 до 360 градусов." << endl;
    cout << "Вам будет необходимо ввести точное наименование изображения (с его форматом), разрешение которого не превышает 1200 х 1200 пикселей.";
    cout << " Пример ввода: <<image.jpg>>." << endl;
    cout << "Далее Вы будете должны выбрать направление наклона изображения (по/против часовой), а после ввести угол наклона изображения от 0 до 360 градусов." << endl;
    cout << "Далее (если угол наклона не является 0/90/180/270/360) будет предложено выбрать цвет фона изображения (белый или черный)." << endl;
    cout << "Измененное изображение будет выведено на экран. При желании вы сможете сохранить полученное изображение." << endl;
    cout << "=============================================================" << endl << endl;
}

void openCVtoMatrix(Mat image,pixelRGB** pixel)
{
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            pixel[i][j].blue = image.at<Vec3b>(i, j)[0];
            pixel[i][j].green = image.at<Vec3b>(i, j)[1];
            pixel[i][j].red = image.at<Vec3b>(i, j)[2];
        }
    }
}

void rotateImage(pixelRGB **pixel, int angle, float rad, int rows, int cols)
{
    //поиск максимальных и минимальных граничных значений при вращении на произвольный угол
    float maxRow, maxCol, minRow, minCol;
    int centRow = rows / 2;
    int centCol = cols / 2;
    if (angle >= 0 && angle <= 90)//maxRow:pixel[rows][0],maxCol: pixel[rows][cols],minRow: pixel[0][cols], minCol: pixel[0][0]
    {
        maxRow = centRow + (rows - centRow) * cos(rad) - (0 - centCol) * sin(rad);
        maxCol = centCol + (cols - centCol) * cos(rad) + (rows - centRow) * sin(rad);

        minRow = centRow + (0 - centRow) * cos(rad) - (cols - centCol) * sin(rad);
        minCol = centCol + (0 - centCol) * cos(rad) + (0 - centRow) * sin(rad);

    }
    else if (angle > 90 && angle <= 180)//maxRow:pixel[0][0],maxCol: pixel[rows][0], minRow: pixel[rows][cols], minCol: pixel[0][cols]
    {
        maxRow = centRow + (0 - centRow) * cos(rad) - (0 - centCol) * sin(rad);
        maxCol = centCol + (0 - centCol) * cos(rad) + (rows - centRow) * sin(rad);

        minRow = centRow + (rows - centRow) * cos(rad) - (cols - centCol) * sin(rad);
        minCol = centCol + (cols - centCol) * cos(rad) + (0 - centRow) * sin(rad);
    }
    else if (angle > 180 && angle <= 270)//maxRow:pixel[0][cols],maxCol: pixel[0][0], minRow: pixel[rows][0], minCol: pixel[rows][cols]
    {
        maxRow = centRow + (0 - centRow) * cos(rad) - (cols - centCol) * sin(rad);
        maxCol = centCol + (0 - centCol) * cos(rad) + (0 - centRow) * sin(rad);

        minRow = centRow + (rows - centRow) * cos(rad) - (0 - centCol) * sin(rad);
        minCol = centCol + (cols - centCol) * cos(rad) + (rows - centRow) * sin(rad);
    }
    else //от 270 (не вкл.) до 360(вкл.) maxRow:pixel[rows][cols],maxCol: pixel[0][cols], minRow: pixel[0][0], minCol: pixel[rows][0]
    {
        maxRow = centRow + (rows - centRow) * cos(rad) - (cols - centCol) * sin(rad);
        maxCol = centCol + (cols - centCol) * cos(rad) + (0 - centRow) * sin(rad);

        minRow = centRow + (0 - centRow) * cos(rad) - (0 - centCol) * sin(rad);
        minCol = centCol + (0 - centCol) * cos(rad) + (rows - centRow) * sin(rad);
    }
    //размер матрицы
    r = maxRow - minRow + 1;
    c = maxCol - minCol + 1;

    pixelNew = new pixelRGB * [r];//строки
    for (int i = 0; i < r; i++)
        pixelNew[i] = new pixelRGB[c];//столбцы
    
    //необходимо условие, если количество строк или столбцов будет меньше исходных размеров (для того, чтобы пробежать все пиксели. Например, это поисходит при поворотах на 90 и 270 градусов)
    int r1, c1;
    if (r < rows)
        r1 = rows;
    else
        r1 = r;
    if (c < cols)
        c1 = cols;
    else
        c1 = c;
    for (int i = 0; i < r1; i++)
    {
        for (int j = 0; j < c1; j++)
        {
            if (i < rows && j < cols)
            {
                float x = centRow + (i - centRow) * cos(rad) - (j - centCol) * sin(rad);
                float y = centCol + (j - centCol) * cos(rad) + (i - centRow) * sin(rad);
                int newI = x - minRow;
                int newJ = y - minCol;

                pixelNew[newI][newJ] = pixel[i][j];
                
                //сглаживание - убираем точки между пикселями
                if (angle >= 0 && angle <= 180)
                {
                    if (newJ > 0 && pixelNew[newI][newJ - 1].blue == -1 && pixelNew[newI][newJ - 1].green == -1 && pixelNew[newI][newJ - 1].red == -1)
                    {

                        //если предыдущее значение пустой пиксель(или несколько), то сдвигаем текущее значение в этот пустой пиксель
                        if (i > 0 && j > 0)//это нужно, чтобы пиксели не сдвигались в начало, т.е. мы будем игнорировать некоторые строчки и столбцы и оставлять их
                            while (pixelNew[newI][newJ - 1].blue == -1 && pixelNew[newI][newJ - 1].green == -1 && pixelNew[newI][newJ - 1].red == -1)
                            {
                                newJ--;
                                if (pixelNew[newI + 1][newJ + 1].blue != -1 && pixelNew[newI + 1][newJ + 1].green != -1 && pixelNew[newI + 1][newJ + 1].red != -1)
                                    break;
                            }
                        pixelNew[newI][newJ] = pixel[i][j];

                    }

                }
                else
                {
                    if (newJ + 1 < c1 && pixelNew[newI][newJ + 1].blue == -1 && pixelNew[newI][newJ + 1].green == -1 && pixelNew[newI][newJ + 1].red == -1)
                    {
                        //если следующее значение пустой пиксель(или несколько), то сдвигаем текущее значение в этот пустой пиксель
                        if (i > 0 && j > 0)//это нужно, чтобы пиксели не сдвигались в начало, т.е. мы будем игнорировать некоторые строчки и столбцы и оставлять их
                            while (pixelNew[newI][newJ + 1].blue == -1 && pixelNew[newI][newJ + 1].green == -1 && pixelNew[newI][newJ + 1].red == -1)
                            {
                                newJ++;
                                if (pixelNew[newI - 1][newJ - 1].blue != -1 && pixelNew[newI - 1][newJ - 1].green != -1 && pixelNew[newI - 1][newJ - 1].red != -1)
                                    break;
                            }
                        pixelNew[newI][newJ] = pixel[i][j];

                    }
                }
                
            }

        }
    }
}
Mat MatrixToOpenCV(Mat image, pixelRGB** pixelNew, int color)
{
    Mat rot(r, c, image.type());
    rot = Scalar(0, 0, 0);
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (color == 0 && pixelNew[i][j].blue == -1 && pixelNew[i][j].green == -1 && pixelNew[i][j].red == -1)
            {
                pixelNew[i][j].blue = color;
                pixelNew[i][j].green = color;
                pixelNew[i][j].red = color;
            }
            rot.at<Vec3b>(i, j)[0] = pixelNew[i][j].blue;
            rot.at<Vec3b>(i, j)[1] = pixelNew[i][j].green;
            rot.at<Vec3b>(i, j)[2] = pixelNew[i][j].red;
        }

    }
    
    return rot;
}
string testingNameImage(string name)
{
    bool flag = false;

    while (flag == false)
    {
        flag = true;
        for (int i = 0; i < name.length(); i++)
            if (name[i] >= 'А' && name[i] <= 'я')
            {
                flag = false;
                break;
            }
        if (flag == false)
        {
            cout << "Некорректное имя!Введите наименование, используя латинские буквы: ";
            cin.clear();
            cin.ignore(1251, '\n');
            cin >> name;
        }
    }
    cin.ignore(cin.rdbuf()->in_avail());

    return name;
}

string testingImageFormat(string type)
{
    while (type != "bmp" && type != "jpeg" && type != "jpg" && type != "png")
    {
        cout << "Неверный ввод!Введите один из допустимых расширений - bmp, jpeg, jpg, png : ";
        cin.clear();
        cin.ignore(1251, '\n');
        cin >> type;
    }
    cin.ignore(cin.rdbuf()->in_avail());
    return type;
}

