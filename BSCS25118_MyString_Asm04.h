#pragma once
#include <iostream>
using namespace std;


class MyString {
private:

	int size;
	char* Cs;

public:
	MyString();
	MyString(char c);
	MyString(const char* p);
	MyString(int n, int c);
	MyString(int n);
	void ReplaceFirst(char c);
	MyString(const MyString& other);
	~MyString();
	MyString itos(int num) const;
	char CharacterAt(int i) const;
	void ReplaceAt(int i, char S);
	int length() const;
	int stoi() const;
	MyString trim() const;
	void InsertAt(int pos, char c);
	void RemoveAt(int pos);
	bool isEqual(const MyString& M) const;
	bool isLess(const MyString& M)const;
	bool isGreater(const MyString& M)const;
	MyString* Split(char delim, int& count) const;
	MyString* Tokenize(const char* delim, int& count) const;
	MyString Substring(int start, int len) const;
	bool IsDelim(char c, const char* delim) const;
	MyString& operator=(const MyString& other);
	void clear();
	void ToUpper();
	void ToLower();
	void Print() const;
};