#include "menu.h"
#include <iostream>
#include <cstdlib>
#include <conio.h> 

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
        std::cout << "===== メニュー =====\n";
        for (int i = 0; i < menuCount; ++i) {
            std::cout << (i == cursor ? "> " : "  ");
            std::cout << menuItems[i] << "\n";
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