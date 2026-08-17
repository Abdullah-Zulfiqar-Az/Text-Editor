#pragma once
#include <iostream>
#include "BSCS25118_Chapter_Proj02.h"
#include <vector>
#include <fstream>
using namespace std;


class Document
{
	vector<Chapter> chapters;

public:
	Document();

	void addChapter(const Chapter& c);
	void deleteChapter(int index);
	int chapterCount() const;

	Chapter& getChapter(int index);
	void SaveToFile(const char* filename) const;
	void LoadFromFile(const char* filename);
	void Print() const;
};