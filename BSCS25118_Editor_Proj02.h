#pragma once
#include <iostream>
#include "BSCS25118_Document_Proj02.h"
using namespace std;

class Editor
{
	Document* doc;
	int currentMode;
	int chapterIdx;
	int sectionIdx;
	int paragraphIdx;
	int lineIdx;
	int columnIdx;
	MyString cmdBuffer;
	Line clipboard;
	MyString textClipboard;
	bool quitRequested;
	MyString lastSearchPattern;
	bool showLineNumbers;
	vector<Document> undo;
	vector<Document> redo;
	bool selecting;
	int selectionStart;

	int lastAction;
	char lastActionChar;
	int lastCursorRow;
	int lastCursorCol;

	static const int action_none = 0;
	static const int action_type_char = 1;
	static const int action_delete_line = 2;
	static const int action_delete_char = 3;
	static const int action_paste_after = 4;
	static const int action_paste_before = 5;
	static const int action_toggle_case = 6;
	static const int half_page = 10;

	bool moveToNextLine();
	bool moveToPrevLine();
	bool LineContainsAt(const MyString& lineText, const MyString& pattern, int startPos, int& matchPos);
	void ReplaceInLine(Line& line, const MyString& oldPat, const MyString& newPat);
	void SaveUndo();

public:
	Editor(Document* d);

	static const int normalMode = 0;
	static const int insertMode = 1;
	static const int commandMode = 2;

	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void MoveUp();
	
	void EnterInsertMode();
	void EnterCommandMode();
	void EnterNormalMode();
	int GetMode() const;

	void TypeChar(char c);
	void Backspace();
	void EnterKey();

	void DeleteCharUnderCursor();
	void DeleteLine();
	void CopyLine();
	void PasteAfter();
	void PasteBefore();
	void GoToFirstLine();
	void GoToLastLine();

	void MoveToLineStart();
	void MoveToLineEnd();
	void DeleteToEndOfLine();
	void ToggleCase();
	

	void MoveWordForward();
	void MoveWordBackward();

	void CmdModeTypeChar(char c);
	void ExecuteCmd();
	bool ShouldQuit() const;

	void SearchForward(const MyString& pattern);
	void SearchBackward(const MyString& pattern);
	void ReplaceAll(const MyString& oldPat, const MyString& newPat);
	bool ShowLineNumbers() const;

	void Render();
	void Undo();
	void Redo();
	void RepeatLastAction();
	void ScrollDown();
	void ScrollUp();
	void TabIndent();
	void ShiftTabUnindent();

	void StartSelectionRight();
	void StartSelectionLeft();
	void ClearSelection();
	void CopySelection();
	void CutSelection();
	void PasteText();

	int GetCursorRow() const;
	int GetCursorCol() const;

	void PrintCursorPos() const; //testing
	void PrintCurrentLine() const; //testing
};