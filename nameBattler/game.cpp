#include "game.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include "title.h"
#include "console.h"
#include "conioex.h"
#include "menu.h"
#include "status.h"
#include "battle.h"

void runGame()
{
    system("cls");
    SetCursorVisibility(FALSE);

    loadAndDrawTitle("title.txt");
    _getch();

    while (true) {
        int menuChoice = showMenu();
        system("cls");

        if (menuChoice == 5) { // End Game
            std::cout << "終了します。\n";
            break;
        } else if (menuChoice == 3) { // To Title
            loadAndDrawTitle("title.txt");
            _getch();
            continue;
        } else if (menuChoice == 4) { // Enemy List 仮表示
            std::cout << "敵リストは未実装です。\n";
            std::cout << "何かキーで戻る...\n";
            _getch();
            continue;
        }

        const int playerCount = (menuChoice == 1) ? 1 : 2;

        std::string id1, id2;
        while (true) {
            std::cout << "1Pの学籍番号（5桁）: ";
            std::cin >> id1;
            if (!isValidId(id1)) {
                continue;
            }
            if (id1[0] != '5') {
                std::cout << "貴様は今年の代ではない！\n";
                continue;
            }
            break;
        }
        Status s1 = generateStatus(id1);

        Status s2{};
        if (playerCount == 2) {
            while (true) {
                std::cout << "2Pの学籍番号（5桁）: ";
                std::cin >> id2;
                if (!isValidId(id2)) {
                    continue;
                }
                if (id2[0] != '5') {
                    std::cout << "貴様は今年の代ではない！\n";
                    continue;
                }
                break;
            }
            s2 = generateStatus(id2);
        } else {
            // 1プレイヤー時の仮敵生成（固定IDなど）
            s2 = generateStatus("50000");
        }

        std::cout << "1P: HP=" << s1.hp << " 攻撃=" << s1.attack
                  << " 防御=" << s1.defense << " 速度=" << s1.speed << "\n";
        std::cout << "2P: HP=" << s2.hp << " 攻撃=" << s2.attack
                  << " 防御=" << s2.defense << " 速度=" << s2.speed << "\n";

        battle(s1, s2);

        std::cout << "何かキーでメニューへ...\n";
        _getch();
    }
}