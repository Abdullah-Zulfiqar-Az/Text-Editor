#include "BSCS25118_Editor_Proj02.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

Editor::Editor(Document* d)
{
	doc = d;
	currentMode = normalMode;

	chapterIdx = 0;
	sectionIdx = 0;
	paragraphIdx = 0;
	lineIdx = 0;
	columnIdx = 0;
	cmdBuffer = "";
	clipboard = Line("");
	quitRequested = false;
	showLineNumbers = false;
	lastSearchPattern = "";
	lastAction = action_none;
}

bool Editor::moveToNextLine()
{
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);

	if (lineIdx + 1 < para.lineCount())
	{
		lineIdx++;
		columnIdx = 0;
		return true;
	}

	if (paragraphIdx + 1 < sec.paragraphCount())
	{
		paragraphIdx++;
		lineIdx = 0;
		columnIdx = 0;
		return true;
	}

	if (sectionIdx + 1 < chap.sectionCount())
	{
		sectionIdx++;
		paragraphIdx = 0;
		lineIdx = 0;
		columnIdx = 0;
		return true;
	}

	if (chapterIdx + 1 < doc->chapterCount())
	{
		chapterIdx++;
		sectionIdx = 0;
		paragraphIdx = 0;
		lineIdx = 0;
		columnIdx = 0;
		return true;
	}

	return false;
}

bool Editor::moveToPrevLine()
{
	if (lineIdx - 1 >= 0)
	{
		lineIdx--;
		columnIdx = 0;
		return true;
	}

	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);

	if (paragraphIdx - 1 >= 0)
	{
		paragraphIdx--;
		Paragraph& prevPara = sec.getParagraph(paragraphIdx);
		lineIdx = prevPara.lineCount() - 1;
		columnIdx = 0;
		return true;
	}

	if (sectionIdx - 1 >= 0)
	{
		sectionIdx--;
		Section& prevSec = chap.getSection(sectionIdx);
		paragraphIdx = prevSec.paragraphCount() - 1;
		Paragraph& prevPara = sec.getParagraph(paragraphIdx);
		lineIdx = prevPara.lineCount() - 1;
		columnIdx = 0;
		return true;
	}

	if (chapterIdx - 1 >= 0)
	{
		chapterIdx--;
		Chapter& prevChap = doc->getChapter(chapterIdx);
		sectionIdx = prevChap.sectionCount() - 1;
		Section& prevSec = chap.getSection(sectionIdx);
		paragraphIdx = prevSec.paragraphCount() - 1;
		Paragraph& prevPara = sec.getParagraph(paragraphIdx);
		lineIdx = prevPara.lineCount() - 1;
		columnIdx = 0;
		return true;
	}

	return false;
}

void Editor::MoveUp()
{
	moveToPrevLine();
}

void Editor::MoveDown()
{
	moveToNextLine();
}

void Editor::MoveRight()
{
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	if (columnIdx + 1 < line.length())
	{
		columnIdx++;
	}
}

void Editor::MoveLeft()
{
	if (columnIdx - 1 >= 0)
	{
		columnIdx--;
	}
}

void Editor::EnterInsertMode()
{
	SaveUndo();
	currentMode = insertMode;

	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	if (columnIdx < line.length())
	{
		columnIdx++;
	}
}

void Editor::EnterCommandMode()
{
	currentMode = commandMode;
	cmdBuffer.clear();
}

void Editor::EnterNormalMode()
{
	currentMode = normalMode;
}

int Editor::GetMode() const
{
	return currentMode;
}

void Editor::TypeChar(char c)
{

	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& l = para.getLine(lineIdx);

	l.insertChar(columnIdx, c);
	columnIdx++;

	lastAction = action_type_char;
	lastActionChar = c;
}

void Editor::PrintCursorPos() const //testing
{
	cout << "chapter=" << chapterIdx
		<< " section=" << sectionIdx
		<< " paragraph=" << paragraphIdx
		<< " line=" << lineIdx
		<< " col=" << columnIdx << endl;
}

void Editor::PrintCurrentLine() const //testing
{
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	line.Print();
}

void Editor::Backspace()
{

	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);

	if (columnIdx > 0)
	{
		Line& line = para.getLine(lineIdx);
		line.deleteChar(columnIdx - 1);
		columnIdx--;
		return;
	}

	if (lineIdx == 0)
	{
		return; //do later
	}

	Line& prevLine = para.getLine(lineIdx - 1);
	Line& currLine = para.getLine(lineIdx);

	MyString prevText = prevLine.getText();
	MyString currText = currLine.getText();
	int mergePoint = prevText.length();

	MyString merged = prevText;

	for (int i = 0; i < currText.length(); i++)
	{
		merged.InsertAt(merged.length(), currText.CharacterAt(i));
	}
	
	prevLine.SetContent(merged);
	para.deleteLine(lineIdx);
	lineIdx--;
	columnIdx = mergePoint;
}

void Editor:: EnterKey()
{
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	MyString fullText = line.getText();

	MyString beforeTxt("");
	MyString afterTxt("");

	for (int i = 0; i < columnIdx; i++)
	{
		beforeTxt.InsertAt(i, fullText.CharacterAt(i));
	}

	for (int i = columnIdx; i < fullText.length(); i++)
	{
		afterTxt.InsertAt(afterTxt.length(), fullText.CharacterAt(i));
	}

	line.SetContent(beforeTxt);

	Line newLine(afterTxt);
	para.InsertLineAt(lineIdx + 1, newLine);

	lineIdx++;
	columnIdx = 0;
}

void Editor::DeleteCharUnderCursor()
{
	SaveUndo();
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	if (columnIdx >= line.length())
	{
		return;
	}

	line.deleteChar(columnIdx);
	lastAction = action_delete_char;
}

void Editor::DeleteLine()
{
	SaveUndo();
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	clipboard = line;

	para.deleteLine(lineIdx);

	if (lineIdx >= para.lineCount() && lineIdx > 0)
	{
		lineIdx--;
	}

	columnIdx = 0;

	lastAction = action_delete_line;
}

void Editor::CopyLine()
{
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	clipboard = line;
}

void Editor::PasteAfter()
{
	SaveUndo();
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);

	para.InsertLineAt(lineIdx + 1, clipboard);
	lineIdx++;
	columnIdx = 0;

	lastAction = action_paste_after;
}

void Editor::PasteBefore()
{
	SaveUndo();
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);

	para.InsertLineAt(lineIdx, clipboard);
	columnIdx = 0;

	lastAction = action_paste_before;
}

void Editor::GoToFirstLine()
{
	chapterIdx = 0;
	sectionIdx = 0;
	paragraphIdx = 0;
	lineIdx = 0;
	columnIdx = 0;
}

void Editor::GoToLastLine()
{
	chapterIdx = doc->chapterCount() - 1;
	Chapter& chap = doc->getChapter(chapterIdx);

	sectionIdx = chap.sectionCount() - 1;
	Section& sec = chap.getSection(sectionIdx);

	paragraphIdx = sec.paragraphCount() - 1;
	Paragraph& para = sec.getParagraph(paragraphIdx);
	lineIdx = para.lineCount() - 1;
	columnIdx = 0;
}

void Editor::MoveToLineStart()
{
	columnIdx = 0;
}

void Editor::MoveToLineEnd()
{
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	if (line.length() > 0)
	{
		columnIdx = line.length() - 1;
	}
	else
	{
		columnIdx = 0;
	}
}

void Editor::DeleteToEndOfLine()
{
	SaveUndo();
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	while (line.length() > columnIdx)
	{
		line.deleteChar(columnIdx);
	}
}

void Editor::ToggleCase()
{
	SaveUndo();
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	line.toggleCase(columnIdx);
	if (columnIdx + 1 < line.length())
	{
		columnIdx++;
	}

	lastAction = action_toggle_case;
}

void Editor::MoveWordForward()
{
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	int pos = columnIdx;
	int len = line.length();

	while (pos < len && line.charAt(pos) != ' ')
	{
		pos++;
	}

	while (pos < len && line.charAt(pos) == ' ')
	{
		pos++;
	}

	if (pos < len)
	{
		columnIdx = pos;
	}

}

void Editor::MoveWordBackward()
{
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	int pos = columnIdx;

	if (pos > 0)
	{
		pos--;
	}

	while (pos > 0 && line.charAt(pos) == ' ')
	{
		pos--;
	}

	while (pos > 0 && line.charAt(pos - 1) != ' ')
	{
		pos--;
	}

	columnIdx = pos;
}

void Editor::CmdModeTypeChar(char c)
{
	cmdBuffer.InsertAt(cmdBuffer.length(), c);
}

bool Editor::ShouldQuit() const
{
	return quitRequested;
}

void Editor::ExecuteCmd()
{
	if (cmdBuffer.length() > 0 && cmdBuffer.CharacterAt(0) == '/')
	{
		MyString pattern = cmdBuffer.Substring(1, cmdBuffer.length() - 1);
		SearchForward(pattern);
	}
	else if (cmdBuffer.length() > 0 && cmdBuffer.CharacterAt(0) == '?')
	{
		MyString pattern = cmdBuffer.Substring(1, cmdBuffer.length() - 1);
		SearchBackward(pattern);
	}
	else if (cmdBuffer.length() > 2 && cmdBuffer.CharacterAt(0) == '%' && cmdBuffer.CharacterAt(1) == 's' && cmdBuffer.CharacterAt(2) == '/')
	{
		int count;
		MyString* pieces = cmdBuffer.Tokenize("/", count);

		if (count >= 3)
		{
			ReplaceAll(pieces[1], pieces[2]);
		}
		delete[] pieces;
	}
	else if (cmdBuffer.isEqual(MyString("n")))
	{
		SearchForward(lastSearchPattern);
	}
	else if (cmdBuffer.isEqual(MyString("N")))
	{
		SearchBackward(lastSearchPattern);
	}
	else if (cmdBuffer.isEqual(MyString("set number")))
	{
		showLineNumbers = true;
	}
	else if (cmdBuffer.isEqual(MyString("set nonumber")))
	{
		showLineNumbers = false;
	}
	else if (cmdBuffer.isEqual(MyString("w")))
	{
		doc->SaveToFile("document.txt");
	}
	else if (cmdBuffer.isEqual(MyString("q")))
	{
		quitRequested = true;
	}
	else if (cmdBuffer.isEqual(MyString("wq")))
	{
		doc->SaveToFile("document.txt");
		quitRequested = true;
	}
	else if (cmdBuffer.isEqual(MyString("q!")))
	{
		quitRequested = true;
	}
	cmdBuffer.clear();
	currentMode = normalMode;
}

bool Editor::LineContainsAt(const MyString& lineText, const MyString& pattern, int startPos, int& matchPos)
{
	int lineLen = lineText.length();
	int patLen = pattern.length();

	for (int i = startPos; i <= lineLen - patLen; i++)
	{
		bool match = true;

		for (int j = 0; j < patLen; j++)
		{
			if (lineText.CharacterAt(i + j) != pattern.CharacterAt(j))
			{
				match = false;
				break;
			}
		}

		if (match)
		{
			matchPos = i;
			return true;
		}
	}
	return false;
}

void Editor::SearchForward(const MyString& pattern)
{
	int c = chapterIdx;
	int s = sectionIdx;
	int p = paragraphIdx;
	int l = lineIdx;
	int startCol = columnIdx + 1;

	lastSearchPattern = pattern;

	while (c < doc->chapterCount())
	{
		Chapter& chap = doc->getChapter(c);

		while (s < chap.sectionCount())
		{
			Section& sec = chap.getSection(s);

			while (p < sec.paragraphCount())
			{
				Paragraph& para = sec.getParagraph(p);

				while (l < para.lineCount())
				{
					Line& line = para.getLine(l);
					MyString text = line.getText();

					int matchPos;

					if (LineContainsAt(text, pattern, startCol, matchPos))
					{
						chapterIdx = c;
						sectionIdx = s;
						paragraphIdx = p;
						lineIdx = l;
						columnIdx = matchPos;
						return;
					}
					startCol = 0;
					l++;
				}
				l = 0;
				p++;
			}
			p = 0;
			s++;
		}
		s = 0;
		c++;
	}
}

void Editor::ReplaceInLine(Line& line, const MyString& oldPat, const MyString& newPat)
{
	int searchStart = 0;

	while (true)
	{
		MyString text = line.getText();
		int matchPos;

		if (!LineContainsAt(text, oldPat, searchStart, matchPos))
		{
			break;
		}

		for (int i = 0; i < oldPat.length(); i++)
		{
			line.deleteChar(matchPos);
		}

		for (int i = 0; i < newPat.length(); i++)
		{
			line.insertChar(matchPos + i, newPat.CharacterAt(i));
		}

		searchStart = matchPos + newPat.length();
	}
}

void Editor::ReplaceAll(const MyString& oldPat, const MyString& newPat)
{
	SaveUndo();

	for (int c = 0; c < doc->chapterCount(); c++)
	{
		Chapter& chap = doc->getChapter(c);

		for (int s = 0; s < chap.sectionCount(); s++)
		{
			Section& sec = chap.getSection(s);

			for (int p = 0; p < sec.paragraphCount(); p++)
			{
				Paragraph& para = sec.getParagraph(p);

				for (int l = 0; l < para.lineCount(); l++)
				{
					Line& line = para.getLine(l);
					ReplaceInLine(line, oldPat, newPat);
				}
			}
		}
	}
}

void Editor::SearchBackward(const MyString& pattern)
{
	lastSearchPattern = pattern;

	int c = chapterIdx;
	int s = sectionIdx;
	int p = paragraphIdx;
	int l = lineIdx;

	bool firstLine = true;

	while (c >= 0)
	{
		Chapter& chap = doc->getChapter(c);
		while (s >= 0)
		{
			Section& sec = chap.getSection(s);
			while (p >= 0)
			{
				Paragraph& para = sec.getParagraph(p);
				while (l >= 0)
				{
					Line& line = para.getLine(l);
					MyString text = line.getText();

					int searchEnd;
					if (firstLine) 
					{
						searchEnd = columnIdx - 1;
					}
					else 
					{
						searchEnd = text.length() - 1;
					}

					firstLine = false;
					int matchPos = -1;

					for (int i = 0; i <= searchEnd && i <= text.length() - pattern.length(); i++)
					{
						int check;
						if (LineContainsAt(text, pattern, i, check) && check == i)
						{
							matchPos = i;
						}
					}

					if (matchPos != -1)
					{
						chapterIdx = c;
						sectionIdx = s;
						paragraphIdx = p;
						lineIdx = l;
						columnIdx = matchPos;
						return;
					}
					l--;
				}
				if (p - 1 >= 0) 
				{
					l = sec.getParagraph(p - 1).lineCount() - 1;
				}
				else 
				{
					l = -1;
				}
				p--;
			}
			if (s - 1 >= 0)
			{
				p = chap.getSection(s - 1).paragraphCount() - 1;
			}
			else
			{
				p = -1;
			}
			s--;
		}
		if (c - 1 >= 0)
		{
			s = doc->getChapter(c - 1).sectionCount() - 1;
		}
		else
		{
			s = -1;
		}
		c--;
	}
}


void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOutput, scrn);
}


void Editor::Render() 
{
	system("cls");

	int screenRow = 0;
	int cursorRow = 0;
	int cursorCol = 0;

	for (int c = 0; c < doc->chapterCount(); c++)
	{
		Chapter& chap = doc->getChapter(c);
		for (int s = 0; s < chap.sectionCount(); s++)
		{
			Section& sec = chap.getSection(s);
			for (int p = 0; p < sec.paragraphCount(); p++)
			{
				Paragraph& para = sec.getParagraph(p);
				for (int l = 0; l < para.lineCount(); l++)
				{
					Line& line = para.getLine(l);

					int numberWidth = 0;

					if (showLineNumbers)
					{
						MyString numStr(screenRow + 1);
						cout << numStr.CharacterAt(0);
						numberWidth = numStr.length() + 1;

						for (int i = 1; i < numStr.length(); i++)
						{
							cout << numStr.CharacterAt(i);
						}
						cout << " ";
					}

					line.Print();

					if (c == chapterIdx && s == sectionIdx && p == paragraphIdx && l == lineIdx)
					{
						cursorRow = screenRow;
						cursorCol = columnIdx + numberWidth;
					}
					screenRow++;
				}
			}
		}
	}
	cout << endl;

	if (currentMode == normalMode)
	{
		cout << "================== NORMAL ================";
	}
	else if (currentMode == insertMode)
	{
		cout << "================= INSERT ================";
	}
	else if (currentMode == commandMode)
	{
		cout << "================ COMMAND ===============" << endl;
		cout << ":";
		cmdBuffer.Print();
	}

	gotoRowCol(cursorRow, cursorCol);

	lastCursorRow = cursorRow;
	lastCursorCol = cursorCol;
}

void Editor::SaveUndo()
{
	undo.push_back(*doc);
	redo.clear();
}

void Editor::Undo()
{
	if (undo.size() == 0)
	{
		return;
	}

	redo.push_back(*doc);

	int newSize = undo.size() - 1;
	Document snapshot = undo[newSize];
	undo.pop_back();

	*doc = snapshot;
}

void Editor::Redo()
{
	if (redo.size() == 0)
	{
		return;
	}

	undo.push_back(*doc);
	
	int newSize = redo.size() - 1;
	Document snapshot = redo[newSize];
	redo.pop_back();

	*doc = snapshot;
}

void Editor::RepeatLastAction()
{
	if (lastAction == action_type_char)
	{
		TypeChar(lastActionChar);
	}
	else if (lastAction == action_delete_line)
	{
		DeleteLine();
	}
	else if (lastAction == action_delete_char)
	{
		DeleteCharUnderCursor();
	}
	else if (lastAction == action_paste_after)
	{
		PasteAfter();
	}
	else if (lastAction == action_paste_before)
	{
		PasteBefore();
	}
	else if (lastAction == action_toggle_case)
	{
		ToggleCase();
	}
}

void Editor::ScrollDown()
{
	for (int i = 0; i < half_page; i++)
	{
		if (!moveToNextLine())
		{
			break;
		}
	}
}

void Editor::ScrollUp()
{
	for (int i = 0; i < half_page; i++)
	{
		if (!moveToPrevLine())
		{
			break;
		}
	}
}

void Editor::TabIndent()
{
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	for (int i = 0; i < 4; i++)
	{
		line.insertChar(columnIdx, ' ');
		columnIdx++;
	}
}

void Editor::ShiftTabUnindent()
{
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	int removed = 0;

	while (removed < 4 && columnIdx > 0 && line.charAt(columnIdx - 1) == ' ')
	{
		line.deleteChar(columnIdx - 1);
		columnIdx--;
		removed++;
	}
}

void Editor::StartSelectionRight()
{
	if (!selecting)
	{
		selecting = true;
		selectionStart = columnIdx;
	}
	
	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	if (columnIdx < line.length())
	{
		columnIdx++;
	}
}

void Editor::StartSelectionLeft()
{
	if (!selecting)
	{
		selecting = true;
		selectionStart = columnIdx;
	}
	MoveLeft();
}

void Editor::ClearSelection()
{
	selecting = false;
}

void Editor::CopySelection()
{
	if (!selecting)
	{
		return;
	}

	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);
	MyString text = line.getText();
	int start = 0;
	int end = 0;

	if (selectionStart < columnIdx)
	{
		start = selectionStart;
		end = columnIdx;
	}
	else
	{
		start = columnIdx;
		end = selectionStart;
	}

	textClipboard = text.Substring(start, end - start);
}

void Editor::CutSelection()
{
	if (!selecting)
	{
		return;
	}

	SaveUndo();

	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);
	MyString text = line.getText();
	int start = 0;
	int end = 0;

	if (selectionStart < columnIdx)
	{
		start = selectionStart;
		end = columnIdx;
	}
	else
	{
		start = columnIdx;
		end = selectionStart;
	}

	textClipboard = text.Substring(start, end - start);

	for (int i = start; i < end; i++)
	{
		line.deleteChar(start);
	}

	columnIdx = start;
	selecting = false;

}

void Editor::PasteText()
{
	SaveUndo();

	Chapter& chap = doc->getChapter(chapterIdx);
	Section& sec = chap.getSection(sectionIdx);
	Paragraph& para = sec.getParagraph(paragraphIdx);
	Line& line = para.getLine(lineIdx);

	for (int i = 0; i < textClipboard.length(); i++)
	{
		line.insertChar(columnIdx, textClipboard.CharacterAt(i));
		columnIdx++;
	}

}

int Editor::GetCursorRow() const
{
	return lastCursorRow;
}

int Editor::GetCursorCol() const
{
	return lastCursorCol;
}