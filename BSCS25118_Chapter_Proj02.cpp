#include <iostream>
#include "BSCS25118_Chapter_Proj02.h"
using namespace std;

Chapter::Chapter(){}

void Chapter::addSection(const Section& s)
{
	sections.push_back(s);
}

void Chapter::deleteSection(int index)
{
	if (index < 0 || index >= sections.size())
	{
		return;
	}

	sections.erase(sections.begin() + index);
}

int Chapter::sectionCount() const
{
	return sections.size();
}

Section& Chapter:: getSection(int index) 
{
	return sections[index];
}

void Chapter::Print() const
{
	for (int i = 0; i < sections.size(); i++)
	{
		sections[i].Print();
	}
}