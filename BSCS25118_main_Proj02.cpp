#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "BSCS25118_Line_Proj02.h"
#include "BSCS25118_MyString_Asm04.h"
#include "BSCS25118_Paragraph_Proj02.h"
#include "BSCS25118_Section_Proj02.h"
#include "BSCS25118_Chapter_Proj02.h"
#include "BSCS25118_Document_Proj02.h"
#include "BSCS25118_Editor_Proj02.h"
using namespace std;

void gotoRowCol2(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOutput, scrn);
}

void RenderFooter(int mode)
{
	gotoRowCol2(33, 0);

	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOutput, 11);   // bright cyan, same as your startup title

	if (mode == 0)
	{
		cout << " h/j/k/l move | w/b word | dd cut | yy copy | p paste | i insert | : cmd | u undo ";
	}
	else if (mode == 1)
	{
		cout << " Esc normal | Tab indent | Ctrl+C/X/P copy/cut/paste | Ctrl+Left/Right select ";
	}
	else if (mode == 2)
	{
		cout << " :w save | :q quit | :wq save+quit | /pattern search | %s/old/new/g replace ";
	}

	SetConsoleTextAttribute(hOutput, 7);   // back to default
}

void keyLoop(Editor& e)
{
	bool running = true;
	bool lastKeyWasY = false;
	bool lastKeyWasD = false;
	bool lastKeyWasG = false;

	while (running && !e.ShouldQuit())
	{
		char key = _getch();

		if (e.GetMode() == 0) //Normal Mode
		{
			if (key == 'd')
			{
				if (lastKeyWasD)
				{
					e.DeleteLine();
					lastKeyWasD = false;
				}
				else
				{
					lastKeyWasD = true;
				}
			}
			else if (key == 'y')
			{
				if (lastKeyWasY)
				{
					e.CopyLine();
					lastKeyWasY = false;
				}
				else
				{
					lastKeyWasY = true;
				}
			}
			else if (key == 'g')
			{
				if (lastKeyWasG)
				{
					e.GoToFirstLine();
					lastKeyWasG = false;
				}
				else
				{
					lastKeyWasG = true;
				}
			}
			else if (key == 'G')
			{
				e.GoToLastLine();
			}
			else if (key == 'x')
			{
				e.DeleteCharUnderCursor();
			}
			else if (key == 'p')
			{
				e.PasteAfter();
			}
			else if (key == 'P')
			{
				e.PasteBefore();
			}
			else if (key == 'h')
			{
				e.MoveLeft();
			}
			else if (key == 'l')
			{
				e.MoveRight();
			}
			else if (key == 'j')
			{
				e.MoveDown();
			}
			else if (key == 'k')
			{
				e.MoveUp();
			}
			else if (key == 'i')
			{
				e.EnterInsertMode();
			}
			else if (key == ':')
			{
				e.EnterCommandMode();
			}
			else if (key == '0')
			{
				e.MoveToLineStart();
			}
			else if (key == '$')
			{
				e.MoveToLineEnd();
			}
			else if (key == 'D')
			{
				e.DeleteToEndOfLine();
			}
			else if (key == '~')
			{
				e.ToggleCase();
			}
			else if (key == 'w')
			{
				e.MoveWordForward();
			}
			else if (key == 'b')
			{
				e.MoveWordBackward();
			}
			else if (key == 'u')
			{
				e.Undo();
			}
			else if (key == 18)
			{
				e.Redo();
			}
			else if (key == '.')
			{
				e.RepeatLastAction();
			}
			else if (key == 4)
			{
				e.ScrollDown();
			}
			else if (key == 21)
			{
				e.ScrollUp();
			}

			if (key != 'd')
			{
				lastKeyWasD = false;
			}

			if (key != 'y')
			{
				lastKeyWasY = false;
			}

			if (key != 'g')
			{
				lastKeyWasG = false;
			}
		}
		else if (e.GetMode() == 1) //Insert mode
		{
			if (key == 27)
			{
				e.ClearSelection();
				e.EnterNormalMode();
			}
			else if (key == 8)
			{
				e.Backspace();
			}
			else if (key == 13)   
			{
				e.EnterKey();
			}
			else if (key == 9)
			{
				e.TabIndent();
			}
			else if (key == 20)   //Ctrl+T (substitute for Shift+Tab as couldn't figure out how to implement)
			{
				e.ShiftTabUnindent();
			}
			else if (key == 0 || key == -32)
			{
				char special = _getch();
				if (special == 83) // del
				{
					e.DeleteCharUnderCursor();
				}
				else if (special == 115) // Ctrl + left arr
				{
					e.StartSelectionLeft();
				}
				else if (special == 116) //Ctrl + right arr
				{
					e.StartSelectionRight(); 
				}
				
			}
			else if (key == 3)
			{
				e.CopySelection();
			}
			else if (key == 24)
			{
				e.CutSelection();
			}
			else if (key == 16)  //CTRL + P
			{
				e.PasteText();
			}
			else
			{
				e.TypeChar(key);
			}
			
		}
		else if (e.GetMode() == 2) //cmd mode
		{
			if (key == 27)   
			{
				e.EnterNormalMode();
			}
			else if (key == 13)   //enter
			{
				e.ExecuteCmd();
			}
			else
			{
				e.CmdModeTypeChar(key);
			}
		}
		

		//e.PrintCursorPos(); //testing123
		//e.PrintCurrentLine(); //testing123
		e.Render();
		RenderFooter(e.GetMode());
		gotoRowCol2(e.GetCursorRow(), e.GetCursorCol());
	}

}

void SetConsoleSettings()
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTitleA("OOPX Text Editor - Project 02");

	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize.X = 0;
	fontInfo.dwFontSize.Y = 18;
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;
	wcscpy_s(fontInfo.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(hOutput, FALSE, &fontInfo);

	SMALL_RECT windowSize;
	windowSize.Left = 0;
	windowSize.Top = 0;
	windowSize.Right = 99;
	windowSize.Bottom = 34;

	COORD bufferSize;
	bufferSize.X = 100;
	bufferSize.Y = 35;

	SetConsoleScreenBufferSize(hOutput, bufferSize);
	SetConsoleWindowInfo(hOutput, TRUE, &windowSize);

	SetConsoleTextAttribute(hOutput, 7);   // light grey text on black background
}

void ShowStartupMenu(Document& doc)
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hOutput, 11);   // bright cyan
	cout << "=====================================" << endl;
	cout << "        AZ's TEXT EDITOR" << endl;
	cout << "=====================================" << endl;
	SetConsoleTextAttribute(hOutput, 0x07);    // back to default
	cout << endl;

	cout << "  1. New File" << endl;
	cout << "  2. Open Existing File" << endl;
	cout << "  3. Quit" << endl;
	cout << endl;
	cout << "Choose an option: ";

	int choice;
	cin >> choice;
	cin.ignore();

	if (choice == 2)
	{
		doc.LoadFromFile("document.txt");
	}
	else if (choice == 3)
	{
		exit(0);
	}

	if (doc.chapterCount() == 0)
	{
		Paragraph para;
		para.addLine(Line(""));

		Section sec;
		sec.addParagraph(para);

		Chapter chap;
		chap.addSection(sec);

		doc.addChapter(chap);
	}
}


int main()
{

	Document doc;
	Chapter chap;
	Section sec;
	Paragraph para0;
	Paragraph para1;

	ShowStartupMenu(doc);
	SetConsoleSettings();

	//para0.addLine(Line("Hello"));
	//para0.addLine(Line("World"));
	//para1.addLine(Line("Bye"));

	//sec.addParagraph(para0);
	//sec.addParagraph(para1);
	//chap.addSection(sec);
	//doc.addChapter(chap);

	Editor e(&doc);

	keyLoop(e);
	
	return 0;
}