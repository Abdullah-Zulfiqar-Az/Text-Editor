#pragma once
#include <iostream>
#include "BSCS25118_Paragraph_Proj02.h"
#include <vector>
using namespace std;


class Section
{
	vector<Paragraph> paragraphs;

public:
	Section();

	void addParagraph(const Paragraph& p);
	void deleteParagraph(int index);
	int paragraphCount()const;
	
	Paragraph& getParagraph(int index);
	void Print() const;
};