#include <iostream>
#include "IntegerArray.h"

int main()
{
    setlocale(LC_ALL, "ru");

    IntegerArray* container = nullptr;
    try
    {
        container = new IntegerArray(50); // Создаю контейнер (объект класса IntegerArray) размером 50
    }
    catch (MyException &ex)  // Если в качестве размера контейнера передам отрицательное число, то перехвачу исключение по размеру контейнера (не может быть меньше нуля, код ошибки 1). Если число будет слишком большое, то перехвачу исключение по выделению памяти с кодом ошибки 2.
    {
        std::cout << "Exception! " << ex.what() << std::endl;
        std::cout << "Код ошибки: " << ex.GetDataState() << std::endl;
    }

    try
    {
        (* container)[51];  // Обращаюсь с помощью перегруженного оператора "[]" к 51-му элементу контейнера
    }
    catch (MyException &ex)  // перехватываю исключение с кодом ошибки 3
    {
        std::cout << "Exception! " << ex.what() << std::endl;
        std::cout << "Код ошибки: " << ex.GetDataState() << std::endl;
    }

    try
    {
        (* container).resize(200000000);  // Изменяю размер контейнера с сохранением всех элементов, и если размер слишком большой,
    }
    catch (MyException& ex)  // перехватываю исключение по выделению памяти с кодом ошибки 4
    {
        std::cout << "Exception! " << ex.what() << std::endl;
        std::cout << "Код ошибки: " << ex.GetDataState() << std::endl;
    }

    try
    {
        (* container).insertBefore(2, 55);  // Вставляю в контейнер элемент со значением 2 перед элементом с индексом 55
    }
    catch (MyException& ex)  // и перехватываю исключение с кодом ошибки 5 (т.к. индекс элемента уже будет находиться за пределами допустимых значений)
    {
        std::cout << "Exception! " << ex.what() << std::endl;
        std::cout << "Код ошибки: " << ex.GetDataState() << std::endl;
    }

    try
    {
        (* container).remove(55);  // Удаляю елемент контейнера с индексом 55
    }
    catch (MyException& ex)  // и перехватываю исключение с кодом ошибки 6, т.к. контейнер размером 50. Если не удастся выделить память, также перехватится исключение (с кодом ошибки 7).
    {
        std::cout << "Exception! " << ex.what() << std::endl;
        std::cout << "Код ошибки: " << ex.GetDataState() << std::endl;
    }

    return 0;
}