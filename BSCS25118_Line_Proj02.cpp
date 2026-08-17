#include "BSCS25118_Line_Proj02.h"
#include <iostream>
using namespace std;


Line::Line()
{
	text = MyString("");
}

Line::Line(const char* txt)
{
	text = MyString(txt);
}

void Line::insertChar(int pos, char c)
{
	text.InsertAt(pos, c);
}

void Line::deleteChar(int pos)
{
	text.RemoveAt(pos);
}

void Line::toggleCase(int pos)
{
	if (pos < 0 || pos >= text.length())
	{
		return;
	}

	char c = text.CharacterAt(pos);

	if (c >= 'a' && c <= 'z')
	{
		text.ReplaceAt(pos, c - 'a' + 'A');
	}
	else if (c >= 'A' && c <= 'Z')
	{
		text.ReplaceAt(pos, c - 'A' + 'a');
	}
}

int Line::length() const
{
	return text.length();
}

char Line::charAt(int pos)const
{
	return text.CharacterAt(pos);
}

MyString Line::getText()const
{
	return text;
}

void Line::Print() const
{
	text.Print();
}

Line::Line(const MyString& text)
{
	this->text = text;
}

void Line::SetContent(const MyString& text)
{
	this->text = text;
}