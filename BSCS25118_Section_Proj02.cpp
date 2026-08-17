#include <iostream>
#include "BSCS25118_Section_Proj02.h"
using namespace std;


Section::Section() {}

void Section::addParagraph(const Paragraph& p)
{
	paragraphs.push_back(p);
}

void Section::deleteParagraph(int index)
{
	if (index < 0 || index >= paragraphs.size())
	{
		return;
	}

	paragraphs.erase(paragraphs.begin() + index);
}

int Section::paragraphCount()const
{
	return paragraphs.size();
}

Paragraph& Section::getParagraph(int index)
{
	return paragraphs[index];
}

void Section::Print() const
{
	for (int i = 0; i < paragraphs.size(); i++)
	{
		paragraphs[i].Print();
	}
}