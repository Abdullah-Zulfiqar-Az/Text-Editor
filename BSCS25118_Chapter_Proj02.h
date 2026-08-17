#pragma once
#include <iostream>
#include "BSCS25118_Section_Proj02.h"
#include <vector>
using namespace std;

class Chapter
{
	vector<Section> sections;

public:

	Chapter();

	void addSection(const Section& s);
	void deleteSection(int index);

	int sectionCount() const;
	Section& getSection(int index);
	void Print() const;
};