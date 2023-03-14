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

class IntegerArray  // ������ ����� ����� ����������
{
private:
	int _length{};  // ������
	int* _data{};   // ��������� �� ������ �������
public:
	IntegerArray() = default;  //  ����������� ��� ���������� (�� ���������)
	IntegerArray(int length) : _length(length)  // ����������� � ����������
	{

		if (length < 0)  // ���� ������ ���������� ������ ���� (���� - ���������� �������� ��� ������� ����������)
		{
			throw MyException("bad_length : ������ ���������� ������ ����!", 1);  // �� ����� ���������� �� ������ ���������� � ����� ������ 1
		}

		if (length > 0) // ���� ������ ���������� ������ ���� (�� ��������� ���� �� ��������, ������ ��� ���� �� ����� ����, �� ������� � ������� �� �����)
		{
			_data = new int[length] {};  // ������� ������
			if (_data == nullptr) throw MyException("�� ������� �������� ������ ��� ����� ������ ������� ����������!", 2);  // ����� ��������, � ���� �� ������� �������� ������, ����� ���������� � ����� ������ 2
		}
	}

	~IntegerArray()  // ����������
	{
		delete[] _data;  // ������� ������
	}

	void erase()  // ����� �������� ����������
	{
		if (_data != nullptr)
		{
			delete[] _data;
			_data = nullptr;
			_length = 0;
		}
	}

	int& operator[](int index)  // ���������� �������� "���������� ������"
	{
		if(index >= 0 && index <= _length) // ���� �������� ������� ��������� � �������� ����������
		{
		return _data[index]; // ��������� �������� �������� ���������� �� ����� �������
		}
		throw MyException("bad_range : ������ �� ��������� ���������� ��������!", 3); // ����� (���� ������ ��������� �� ��������� ������ ��������) ����� ���������� � ����� ������ 3
	}

	int getLength() const
	{
		return _length;
	}

	void reallocate(int newLength) // ������� ��������� ������� ���������� � ������������ ���� ���������. ����� ���������� �� �����, ������ ��� ������ ����� - ��� �������� ������ ������� ���������� <= 0 ���������� �������� (erase) ����������.
	{
		erase();  // �������� ���� ������������ ���������
		if (newLength <= 0) return; // ���� ��������� ������ - ����� ������ �������
		_data = new int[newLength]; // ������� ������ ��� ��������� ������ �������
		_length = newLength;
	}

	void resize(int newLength)  // ������� ��������� ������� ���������� � ����������� ���� ���������
	{
		if (newLength == _length) return;  // ���� ��������� ��� ����� ������ ����� - ����� ������ �������
		if (newLength <= 0)  // ���� ���������� ����� �������� ������� ���������� <=0 
		{
			erase();  // ������� ���������
			return;
		}
		int* data{ new int[newLength] };  // ������� ������
		if (data == nullptr) throw MyException("�� ������� �������� ������ ��� ���������� ��������� ���������� � ����� ��������� � �������� ��������!", 4);  // ����� ��������, � ���� �� ������� �������� ������, ����� ���������� � ����� ������ 4
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

	IntegerArray(const IntegerArray& a)   // ����������� �����������
	{
		reallocate(a.getLength());
		for (int index{ 0 }; index < _length; ++index)
			_data[index] = a._data[index];
	}

	void insertBefore(int value, int index) // ������� ������� (��������� - ����������� �������� � ������ ��������, ����� ������� ����� ��� �������� ��������)
	{
		if (index >= 0 && index <= _length) // ���� ������ ��������� � �������� ����������, �� �������� ����������
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
		} throw MyException("bad_range : ����� �� ������� ����������!", 5);  // ���� ���, �� ����� ����������  � ����� ������ 5
	}

	void remove(int index)  // ������� �������� �������� ���������� �� �������
	{
		if (index >= 0 && index <= _length) // ���� ������ ��������� � �������� ����������, �� �������� ����������
		{
			if (_length == 1) // ���� ������ ���������� ����� 1
			{
				erase();  // �� �������� �������� �� ������� ����������� �������� ����� ����������
				return;
			}
			int* data{ new int[_length - 1] };
			if (data == nullptr) throw MyException("�� ������� �������� ������!", 7);  // ����� ��������, � ���� �� ������� �������� ������, ����� ���������� � ����� ������ 7
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
		} throw MyException("bad_range : ����� �� ������� ����������!", 6);  // ���� ���, �� ����� ����������  � ����� ������ 6
	}
};