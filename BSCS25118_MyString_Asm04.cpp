#include <iostream>
#include "BSCS25118_MyString_Asm04.h"
using namespace std;


MyString::MyString()
{
	size = 0;
	Cs = new char[1];
	Cs[0] = '\0';

}

MyString::MyString(char c)
{
	size = 1;
	Cs = new char[2];
	Cs[0] = c;
	Cs[1] = '\0';
}

MyString::MyString(const char* p)
{
	size = 0;
	while (p[size] != '\0')
	{
		size++;
	}

	Cs = new char[size + 1];

	for (int i = 0; i < size; i++)
	{
		Cs[i] = p[i];
	}
	Cs[size] = '\0';
}


MyString::MyString(int n, int c)
{
	size = n;
	Cs = new char[size + 1];

	for (int i = 0; i < size; i++)
	{
		Cs[i] = c;
	}
	Cs[size] = '\0';
}

MyString:: ~MyString()
{
	delete[] Cs;

}

void MyString::ReplaceFirst(char c)
{
	if (size > 0)
	{
		Cs[0] = c;
	}
}

MyString::MyString(const MyString& other)
{
	size = other.size;
	Cs = new char[size + 1];

	for (int i = 0; i <= size; i++)
	{
		Cs[i] = other.Cs[i];
	}

}

MyString MyString::itos(int num) const
{
	if (num == 0)
	{
		return MyString("0");
	}

	bool negative = false;
	int digits = 0;

	if (num < 0)
	{
		negative = true;
		num = -num;
	}
	int temp = num;

	while (temp > 0)
	{
		temp = temp / 10;
		digits++;
	}

	if (negative)
	{
		digits++;
	}

	char* Arr = new char[digits + 1];
	Arr[digits] = '\0';

	int idx = digits - 1;

	while (num > 0)
	{
		Arr[idx] = (num % 10) + '0';
		num = num / 10;
		idx--;
	}

	if (negative == true)
	{
		Arr[0] = '-';
	}

	MyString result(Arr);
	delete[] Arr;
	return result;
}

MyString::MyString(int n)
{
	MyString temp = itos(n);
	size = temp.size;
	Cs = new char[size + 1];

	for (int i = 0; i < size; i++)
	{
		Cs[i] = temp.Cs[i];
	}
}

void MyString::Print() const
{
	for (int i = 0; i < size; i++)
	{
		cout << Cs[i];
	}
	cout << endl;
}

int MyString::length() const
{
	return size;
}

int MyString::stoi() const
{
	int result = 0;
	int idx = 0;
	bool negative = false;

	if (Cs[0] == '-')
	{
		negative = true;
		idx = 1;
	}

	while (idx < size)
	{
		if (Cs[idx] >= '0' && Cs[idx] <= '9')
		{
			result = result * 10 + (Cs[idx] - '0');
		}
		idx++;
	}

	if (negative)
	{
		result = -result;
	}
	return result;
}

char MyString::CharacterAt(int i) const
{
	if (i >= 0 && i < size)
	{
		return Cs[i];
	}
}

void MyString::ReplaceAt(int i, char S)
{
	if (i >= 0 && i < size)
	{
		Cs[i] = S;
	}
}

MyString MyString::trim() const
{
	int front = 0;
	int end = size - 1;

	while (Cs[front] == ' ' && front < size)
	{
		front++;
	}

	while (end >= front && Cs[end] == ' ')
	{
		end--;
	}

	int newSize = end - front + 1;

	char* temp = new char[newSize + 1];

	for (int i = 0; i < newSize; i++)
	{
		temp[i] = Cs[front + i];
	}
	temp[newSize] = '\0';

	MyString result(temp);
	delete[] temp;
	return result;

}

bool MyString::isEqual(const MyString& M) const
{
	if (size != M.size)
	{
		return false;
	}

	for (int i = 0; i < size; i++)
	{
		if (Cs[i] != M.Cs[i])
		{
			return false;
		}
	}
	return true;
}

bool MyString:: isLess(const MyString& M)const
{
	int minSize = 0;

	if (size < M.size)
	{
		minSize = size;
	}
	else
	{
		minSize = M.size;
	}

	for (int i = 0; i < minSize; i++)
	{
		if (Cs[i] != M.Cs[i])
		{
			if (Cs[i] < M.Cs[i])
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	if (size < M.size)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool MyString::isGreater(const MyString& M)const
{
	int minSize = 0;

	if (size < M.size)
	{
		minSize = size;
	}
	else
	{
		minSize = M.size;
	}

	for (int i = 0; i < minSize; i++)
	{
		if (Cs[i] != M.Cs[i])
		{
			if (Cs[i] > M.Cs[i])
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	if (size > M.size)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MyString:: clear()
{
	delete[] Cs;
	size = 0;
	Cs = new char[1];
	Cs[0] = '\0';
}

void MyString::ToUpper()
{
	for (int i = 0; i < size; i++)
	{
		if (Cs[i] >= 'a' && Cs[i] <= 'z')
		{
			Cs[i] = Cs[i] - 'a' + 'A';
		}
	}
}

void MyString::ToLower()
{
	for (int i = 0; i < size; i++)
	{
		if (Cs[i] >= 'A' && Cs[i] <= 'Z')
		{
			Cs[i] = Cs[i] - 'A' + 'a';
		}
	}
}

MyString* MyString::Split(char delim, int& count)const
{
	count = 1;
	for (int i = 0; i <= size; i++)
	{
		if (Cs[i] == delim)
		{
			count++;
		}
	}

	MyString* ptr = new MyString[count];
	int idx = 0;
	int start = 0;

	for (int i = 0; i <= size; i++)
	{
		if (Cs[i] == delim || Cs[i] == '\0')
		{
			int len = i - start;
			//char* arr = new char[len + 1];
			delete[] ptr[idx].Cs;
			ptr[idx].Cs = new char[len + 1];
			for (int j = 0; j < len; j++)
			{
				//arr[j] = Cs[start + j];
				ptr[idx].Cs[j] = Cs[start + j];
			}
			//arr[len] = '\0';
			ptr[idx].Cs[len] = '\0';
			ptr[idx].size = len;

			//ptr[idx] = MyString(arr);
			//delete[] arr;
			
			idx++;
			start = i + 1;
		}
	}
	return ptr;
}

bool MyString::IsDelim(char c, const char* delim) const
{
	int i = 0;
	while (delim[i] != '\0')
	{
		if (c == delim[i])
		{
			return true;
		}
		i++;
	}
	return false;
}

MyString* MyString::Tokenize(const char* delim, int& count) const
{
	count = 0;
	int i = 0;

	while (i < size)
	{
		if (IsDelim(Cs[i], delim))
		{
			i++;
			continue;
		}
		count++;

		while (i < size && !IsDelim(Cs[i], delim))
		{
			i++;
		}
	}

	MyString* ptr = new MyString[count];
	int idx = 0;
	i = 0;

	while (i < size)
	{
		if (IsDelim(Cs[i], delim))
		{
			i++;
			continue;
		}

		int start = i;
		while (i < size && !IsDelim(Cs[i], delim))
		{
			i++;
		}
		int len = i - start;

		delete[] ptr[idx].Cs;
		ptr[idx].Cs = new char[len + 1];
		for (int j = 0; j < len; j++)
		{
			ptr[idx].Cs[j] = Cs[start + j];
		}

		ptr[idx].Cs[len] = '\0';
		ptr[idx].size = len;
		idx++;
	}

	return ptr;
}

void MyString::InsertAt(int pos, char c)
{
	if (pos < 0 || pos > size)
	{
		return;
	}

	char* temp = new char[size + 2];

	for (int i = 0; i < pos; i++)
	{
		temp[i] = Cs[i];
	}

	temp[pos] = c;

	for (int i = pos; i < size; i++)
	{
		temp[i + 1] = Cs[i];
	}

	temp[size + 1] = '\0';

	delete[] Cs;
	Cs = temp;
	size = size + 1;
}

void MyString::RemoveAt(int pos)
{
	if (pos < 0 || pos >= size)
	{
		return;
	}

	char* temp = new char[size];

	for (int i = 0; i < pos; i++)
	{
		temp[i] = Cs[i];
	}

	for (int i = pos + 1; i < size; i++)
	{
		temp[i - 1] = Cs[i];
	}

	temp[size - 1] = '\0';

	delete[] Cs;
	Cs = temp;
	size = size - 1;
}

MyString& MyString::operator=(const MyString& other)
{
	if (this == &other)
	{
		return *this;
	}

	delete[] Cs;
	size = other.size;
	Cs = new char[size+1];

	for (int i = 0; i < size; i++)
	{
		Cs[i] = other.Cs[i];
	}

	Cs[size] = '\0';
	return *this;

}

MyString MyString::Substring(int start, int len) const
{
	char* temp = new char[len + 1];

	for (int i = 0; i < len; i++)
	{
		temp[i] = Cs[start + i];
	}
	temp[len] = '\0';

	MyString result(temp);
	delete[] temp;
	return result;
}
