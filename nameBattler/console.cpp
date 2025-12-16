#include "console.h"
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <string.h>

void SetCursorVisibility(BOOL visible) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// カーソルを先頭に戻す
void MoveCursorToTop() {
	COORD coord = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 追加: 汎用カーソル移動
void SetCursorPosition(int col, int row) {
	if (col < 0) col = 0;
	if (row < 0) row = 0;
	if (col >= CONSOLE_WIDTH) col = CONSOLE_WIDTH - 1;
	if (row >= CONSOLE_HEIGHT) row = CONSOLE_HEIGHT - 1;
	COORD coord = { static_cast<SHORT>(col), static_cast<SHORT>(row) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 追加: 指定行に中央表示
void PrintCenteredLine(const char* text, int row) {
	if (!text) return;
	int len = static_cast<int>(strlen(text));
	int col = (CONSOLE_WIDTH - len) / 2;
	if (col < 0) col = 0;
	SetCursorPosition(col, row);
	std::cout << text;
}

// 追加: ブロック中央表示（垂直・水平）
void PrintCenteredBlock(const char* const* lines, int count) {
	if (!lines || count <= 0) return;
	int startRow = (CONSOLE_HEIGHT - count) / 2;
	if (startRow < 0) startRow = 0;
	for (int i = 0; i < count; ++i) {
		PrintCenteredLine(lines[i], startRow + i);
	}
}

// 以降、既存の関数はそのまま
bool CL11Startup() {
	if (!changeConsoleSize(CONSOLE_WIDTH, CONSOLE_HEIGHT)) {
		return FALSE;
	}
	HideConsoleTitleBar();

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	HDC screen = GetDC(0);
	int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
	int dpiY = GetDeviceCaps(screen, LOGPIXELSY);
	ReleaseDC(0, screen);

	setConsoleFont(CONSOLE_WIDTH, CONSOLE_HEIGHT, screenWidth, screenHeight, dpiX, dpiY);
	centerConsoleWindow(screenWidth, screenHeight);
	return TRUE;
}

void setConsoleFont(int width, int height, int screenWidth, int screenHeight, int dpiX, int dpiY) {
	int fontSizeX = (screenWidth / width) * (dpiX / 96);
	int fontSizeY = (screenHeight / height) * (dpiY / 96);

	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX), 0, {0, 0}, FF_DONTCARE, FW_NORMAL, L"MS Gothic" };
	DISPLAY_DEVICE dd = { sizeof(dd) };
	EnumDisplayDevices(NULL, 0, &dd, 0);
	DEVMODE dm = { sizeof(dm) };
	EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm);

	if (dm.dmPelsHeight > dm.dmPelsWidth) {
		cfi.dwFontSize.X = static_cast<SHORT>(fontSizeX);
		cfi.dwFontSize.Y = static_cast<SHORT>(floor(fontSizeX * 2));
	}
	else {
		cfi.dwFontSize.X = static_cast<SHORT>(floor(fontSizeY / 2));
		cfi.dwFontSize.Y = static_cast<SHORT>(fontSizeY);
	}

	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
	SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width) - 1, static_cast<SHORT>(height) - 1 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
}

void centerConsoleWindow(int screenWidth, int screenHeight) {
	HWND consoleWindow = GetConsoleWindow();
	RECT rect;
	GetWindowRect(consoleWindow, &rect);
	MoveWindow(consoleWindow, (screenWidth - (rect.right - rect.left)) / 2, (screenHeight - (rect.bottom - rect.top)) / 2, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}

bool CL11Cleanup() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(hOut, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
	SetConsoleMode(hIn, ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_MOUSE_INPUT);

	COORD bufferSize = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
	SetConsoleScreenBufferSize(hOut, bufferSize);
	CONSOLE_CURSOR_INFO cursorInfo = { 25, TRUE };
	SetConsoleCursorInfo(hOut, &cursorInfo);
	SMALL_RECT windowSize = { 0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1 };
	SetConsoleWindowInfo(hOut, TRUE, &windowSize);

	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX), 0, {0, 16}, FF_DONTCARE, FW_NORMAL, L"Consolas" };
	SetCurrentConsoleFontEx(hOut, FALSE, &cfi);

	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	ShowConsoleTitleBar();

	return TRUE;
}

bool changeConsoleSize(int x, int y) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SMALL_RECT rectConsoleSize = { 0, 0, 1, 1 };

	SetConsoleWindowInfo(hStdout, TRUE, &rectConsoleSize);
	if (!SetConsoleScreenBufferSize(hStdout, coord)) {
		return FALSE;
	}
	rectConsoleSize.Right = x - 1;
	rectConsoleSize.Bottom = y - 1;
	SetConsoleWindowInfo(hStdout, TRUE, &rectConsoleSize);

	return TRUE;
}

void HideConsoleTitleBar() {
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) {
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_CAPTION;
		SetWindowLong(hwnd, GWL_STYLE, style);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

void ShowConsoleTitleBar() {
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) {
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		style |= WS_CAPTION;
		SetWindowLong(hwnd, GWL_STYLE, style);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

void clearInputBuffer() {
	while (_kbhit()) {
		rewind(stdin);
		(void)getchar();
	}
}

void EnableVTMode() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

void SetTextColorRGB(int r, int g, int b) {
	printf("\x1b[38;2;%d;%d;%dm", r, g, b);
}

void SetBackgroundColorRGB(int r, int g, int b) {
	printf("\x1b[48;2;%d;%d;%dm", r, g, b);
}