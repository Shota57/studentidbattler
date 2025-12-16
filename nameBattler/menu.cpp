#include "menu.h"
#include <iostream>
#include <cstdlib>
#include <conio.h>
#include "console.h"

int showMenu()
{
    const int menuCount = 5;
    const char* menuItems[menuCount] = {
        "1プレイヤー",
        "2プレイヤー",
        "To Title",
        "Enemy List",
        "End Game"
    };

    int cursor = 0;
    while (true) {
        system("cls");

        // メニュー全体をブロック中央表示
        const char* header = "===== メニュー =====";
        PrintCenteredLine(header, (CONSOLE_HEIGHT / 2) - (menuCount / 2) - 2);

        int startRow = (CONSOLE_HEIGHT - menuCount) / 2;
        if (startRow < 0) startRow = 0;

        for (int i = 0; i < menuCount; ++i) {
            std::string line = (i == cursor ? "> " : "  ");
            line += menuItems[i];
            PrintCenteredLine(line.c_str(), startRow + i);
        }

        int key = _getch();
        if (key == 0xE0 || key == 0) {
            key = _getch();
            if (key == 72) {
                cursor = (cursor - 1 + menuCount) % menuCount;
            } else if (key == 80) {
                cursor = (cursor + 1) % menuCount;
            }
        } else if (key == 13) {
            return cursor + 1;
        }
    }
}