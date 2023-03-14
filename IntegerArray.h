#pragma once
#include <exception>

class MyException : public std::exception
{
public:
	MyException(const char *msg, int dataState) : std::exception(msg)
	{
		this->dataState = dataState;
	}
	int GetDataState()
	{
		return dataState;
	}
private:
	int dataState;
};

class IntegerArray  // Создаю класс моего контейнера
{
private:
	int _length{};  // Размер
	int* _data{};   // Указатель на первый элемент
public:
	IntegerArray() = default;  //  Конструктор без параметров (по умолчанию)
	IntegerArray(int length) : _length(length)  // Конструктор с параметром
	{

		if (length < 0)  // Если размер контейнера меньше нуля (ноль - допустимое значение для размера контейнера)
		{
			throw MyException("bad_length : Размер контейнера меньше нуля!", 1);  // то кидаю исключение на размер контейнера с кодом ошибки 1
		}

		if (length > 0) // Если размер контейнера больше нуля (на равенство нулю не проверяю, потому что если он равен нулю, то проблем с памятью не будет)
		{
			_data = new int[length] {};  // Выделяю память
			if (_data == nullptr) throw MyException("Не удалось выделить память при вводе нового размера контейнера!", 2);  // Делаю проверку, и если не удалось выделить память, кидаю исключение с кодом ошибки 2
		}
	}

	~IntegerArray()  // Деструктор
	{
		delete[] _data;  // Очистка памяти
	}

	void erase()  // Метод стирания контейнера
	{
		if (_data != nullptr)
		{
			delete[] _data;
			_data = nullptr;
			_length = 0;
		}
	}

	int& operator[](int index)  // Перегружаю оператор "квадратные скобки"
	{
		if(index >= 0 && index <= _length) // Если значение индекса находится в пределах контейнера
		{
		return _data[index]; // возвращаю значение элемента контейнера по этому индексу
		}
		throw MyException("bad_range : Индекс за пределами допустимых значений!", 3); // Иначе (если индекс находится за пределами нужных значений) кидаю исключение с кодом ошибки 3
	}

	int getLength() const
	{
		return _length;
	}

	void reallocate(int newLength) // Функция изменения размера контейнера с уничтожением всех элементов. Здесь исключений не кидаю, потому что логика такая - при значении нового размера контейнера <= 0 происходит удаление (erase) контейнера.
	{
		erase();  // Удаление всех существующих элементов
		if (newLength <= 0) return; // Если контейнер пустой - конец работы функции
		_data = new int[newLength]; // Выделяю память под контейнер нового размера
		_length = newLength;
	}

	void resize(int newLength)  // Функция изменения размера контейнера с сохранением всех элементов
	{
		if (newLength == _length) return;  // Если контейнер уже имеет нужную длину - конец работы функции
		if (newLength <= 0)  // Если переданное новое значение размера контейнера <=0 
		{
			erase();  // Стираем контейнер
			return;
		}
		int* data{ new int[newLength] };  // Выделяю память
		if (data == nullptr) throw MyException("Не удалось выделить память при перезаписи элементов контейнера в новый контейнер с введённым размером!", 4);  // Делаю проверку, и если не удалось выделить память, кидаю исключение с кодом ошибки 4
		if (_length > 0)
		{
			int elementsToCopy
			{
				(newLength > _length) ? _length : newLength
			};
			for (int index{ 0 }; index < elementsToCopy; ++index)
			{
				data[index] = _data[index];
			}
		}
		delete[] _data;
		_data = data;
		_length = newLength;
	}

	IntegerArray(const IntegerArray& a)   // Конструктор копирования
	{
		reallocate(a.getLength());
		for (int index{ 0 }; index < _length; ++index)
			_data[index] = a._data[index];
	}

	void insertBefore(int value, int index) // Функция вставки (параметры - встявляемое значение и индекс элемента, перед которым нужно это значение вставить)
	{
		if (index >= 0 && index <= _length) // Если индекс находится в границах контейнера, то выполняю инструкции
		{
			int* data{ new int[_length + 1] };
			for (int before{ 0 }; before < index; ++before)
			{
				data[before] = _data[before];
			}
			data[index] = value;
			for (int after{ index }; after < _length; ++after)
			{
				data[after + 1] = _data[after];
			}
			delete[] _data;
			_data = data;
			++_length;
		} throw MyException("bad_range : Выход за границы контейнера!", 5);  // Если нет, то кидаю исключение  с кодом ошибки 5
	}

	void remove(int index)  // Функция удаления элемента контейнера по индексу
	{
		if (index >= 0 && index <= _length) // Если индекс находится в границах контейнера, то выполняю инструкции
		{
			if (_length == 1) // Если размер контейнера равен 1
			{
				erase();  // то удаление элемента по индексу равносильно стиранию этого контейнера
				return;
			}
			int* data{ new int[_length - 1] };
			if (data == nullptr) throw MyException("Не удалось выделить память!", 7);  // Делаю проверку, и если не удалось выделить память, кидаю исключение с кодом ошибки 7
			for (int before{ 0 }; before < index; ++before)
			{
				data[before] = _data[before];
			}
			for (int after{ index + 1 }; after < _length; ++after)
			{
				data[after - 1] = _data[after];
			}
			delete[] _data;
			_data = data;
			--_length;
		} throw MyException("bad_range : Выход за границы контейнера!", 6);  // Если нет, то кидаю исключение  с кодом ошибки 6
	}
};