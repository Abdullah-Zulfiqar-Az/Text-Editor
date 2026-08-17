#include "BSCS25118_Document_Proj02.h"
using namespace std;


Document::Document(){}

void Document::addChapter(const Chapter& c)
{
	chapters.push_back(c);
}

void Document::deleteChapter(int index)
{
	if (index < 0 || index >= chapters.size())
	{
		return;
	}

	chapters.erase(chapters.begin() + index);
}

int Document::chapterCount() const
{
	return chapters.size();
}

Chapter& Document::getChapter(int index) 
{
	return chapters[index];
}

void Document::Print() const
{
	for (int i = 0; i < chapters.size(); i++)
	{
		chapters[i].Print();
	}
}

void Document::SaveToFile(const char* filename) const
{
	ofstream fout(filename);

	if (!fout)
	{
		return;
	}

	for (int c = 0; c < chapters.size(); c++)
	{
		Chapter chap = chapters[c];

		for (int s = 0; s < chap.sectionCount(); s++)
		{
			Section sec = chap.getSection(s);

			for (int p = 0; p < sec.paragraphCount(); p++)
			{
				Paragraph para = sec.getParagraph(p);

				for (int l = 0; l < para.lineCount(); l++)
				{
					Line line = para.getLine(l);

					MyString text = line.getText();

					for (int t = 0; t < text.length(); t++)
					{
						fout << text.CharacterAt(t);
					}
					fout << endl;
				}
			}
		}
	}
	fout.close();
}

void Document::LoadFromFile(const char* filename)
{
	ifstream fin(filename);

	if (!fin)
	{
		return;
	}

	chapters.clear();
	Paragraph para;
	char buffer[1000];

	while (fin.getline(buffer, 1000))
	{
		Line line(buffer);
		para.addLine(line);
	}

	Section sec;
	sec.addParagraph(para);

	Chapter chap;
	chap.addSection(sec);

	chapters.push_back(chap);

	fin.close();
}