#pragma once
#include <windows.h>    // Windows APIを使うためのヘッダ

const int CONSOLE_WIDTH = 80;
const int CONSOLE_HEIGHT = 26;


//=====================================
// 関数プロトタイプ宣言
//=====================================
void SetCursorVisibility(BOOL visible);		// カーソルの表示/非表示を設定する関数
void MoveCursorToTop();                     // カーソルを先頭に戻す関数
bool CL11Startup();                         // コンソールの初期化関数
void setConsoleFont(int width, int height, int screenWidth, int screenHeight, int dpiX, int dpiY); // コンソールフォントの設定関数
void centerConsoleWindow(int screenWidth, int screenHeight);    // コンソールウィンドウを中央に配置する関数
bool CL11Cleanup();                         // コンソールのクリーンアップ関数
bool changeConsoleSize(int x, int y);       // コンソールサイズを変更する関数
void HideConsoleTitleBar();                 // コンソールのタイトルバーを隠す関数
void ShowConsoleTitleBar();                 // コンソールのタイトルバーを表示する関数
void clearInputBuffer();                   // キーボード入力バッファのクリア関数
void EnableVTMode();						// VTシーケンスモードを有効化
void SetTextColorRGB(int r, int g, int b);	//コンソールウィンドウの表示色をRGB形式で変更
void SetBackgroundColorRGB(int r, int g, int b);	//コンソールウィンドウの背景色をRGB形式で変更

// 追加: 汎用カーソル移動と中央表示ヘルパー
void SetCursorPosition(int col, int row);   // カーソル位置を設定（0-based）
void PrintCenteredLine(const char* text, int row); // 指定行に中央揃えで表示
void PrintCenteredBlock(const char* const* lines, int count); // 複数行をブロック中央表示