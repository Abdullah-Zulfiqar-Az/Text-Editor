#pragma once
#include <iostream>
#include "BSCS25118_Line_Proj02.h"
#include <vector>
using namespace std;

class Paragraph
{
	vector<Line> lines;

public:
	Paragraph();

	void addLine(const Line& l);
	void deleteLine(int idx);
	int lineCount() const;
	void InsertLineAt(int index, const Line& l);

	Line& getLine(int index);
	void Print() const;
};