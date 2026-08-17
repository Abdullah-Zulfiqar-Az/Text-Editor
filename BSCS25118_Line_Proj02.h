#pragma once
#include <iostream>
#include "BSCS25118_MyString_Asm04.h"
using namespace std;


class Line {
	MyString text;

public:
	Line();
	Line(const char* txt);

	void insertChar(int pos, char c);
	void deleteChar(int pos);
	void toggleCase(int pos);

	int length() const;
	char charAt(int pos)const;
	MyString getText() const;
	Line(const MyString& text);
	void SetContent(const MyString& text);

	void Print() const;
};