#include <iostream>
#include "BSCS25118_Paragraph_Proj02.h"
using namespace std;


Paragraph::Paragraph(){}


void Paragraph::addLine(const Line& l)
{
	lines.push_back(l);
}

void Paragraph::deleteLine(int idx)
{
	if (idx < 0 || idx >= lines.size())
	{
		return;
	}

	lines.erase(lines.begin() + idx);
}

int Paragraph::lineCount() const
{
	return lines.size();
}

Line& Paragraph::getLine(int index)
{
	return lines[index];
}

void Paragraph::Print() const
{
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].Print();
	}
}

void Paragraph::InsertLineAt(int index, const Line& l)
{
	if (index < 0 || index > lines.size())
	{
		return;
	}

	lines.insert(lines.begin() + index, l);
}