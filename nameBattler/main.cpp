#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "title.h"
#include "console.h"
#include "conioex.h"
#include <vector>

struct Status {
    int hp;
    int attack;
    int defense;
    int speed;
};


Status generateStatus(const std::string& studentId) {
    auto getDigit = [](char c) -> int {
        return (c >= '0' && c <= '9') ? (c - '0') : 9; // 非数字は安全側で最弱扱い
    };
    auto invMap = [](int digit, int minVal, int maxVal) -> int {
        // digit=0 -> 最大値、digit=9 -> 最小値 になるように線形逆マップ
        int range = maxVal - minVal;
        int add = ((9 - digit) * range) / 9;
        return minVal + add;
    };

    // 残り4桁（[1],[2],[3],[4]）を上から HP, ATK, DEF, SPD に割り当て
    int d_hp  = (studentId.size() >= 2) ? getDigit(studentId[1]) : 9;
    int d_atk = (studentId.size() >= 3) ? getDigit(studentId[2]) : 9;
    int d_def = (studentId.size() >= 4) ? getDigit(studentId[3]) : 9;
    int d_spd = (studentId.size() >= 5) ? getDigit(studentId[4]) : 9;

    Status s;
    s.hp      = invMap(d_hp,  80, 120); // 80..120（digit小さいほど高HP）
    s.attack  = invMap(d_atk, 20,  40); // 20..40
    s.defense = invMap(d_def, 10,  30); // 10..30
    s.speed   = invMap(d_spd,  5,  20); // 5..20
    return s;
}

int showMenu() {
    const int menuCount = 5;
    const char* menuItems[menuCount][20] = {
        {"1プレイヤー"},
        {"2プレイヤー"},
        {"To Title"},
        {"Enemy List"},
        {"End Game"}
    };
    int cursor = 0;
    while (true) {
        system("cls");
        std::cout << "===== メニュー =====\n";
        for (int i = 0; i < menuCount; ++i) {
            std::cout << (i == cursor ? "> " : "  ");
            std::cout << menuItems[i][0] << std::endl;
        }
        int key = _getch();
        if (key == 0xE0 || key == 0) { // 拡張キー
            key = _getch();
            if (key == 72) {
                cursor = (cursor - 1 + menuCount) % menuCount;
            }
            else if (key == 80) {
                cursor = (cursor + 1) % menuCount;
            }
        }
        else if (key == 13) {
            return cursor + 1;
        }
    }
}

void battle(const Status& s1, const Status& s2) {
    auto calcDamage = [](int atk, int def) -> int {
        // 基礎ダメージ計算（下限1）
        int base = (atk * 2) / 3 - def / 2;
        if (base < 1) base = 1;

        // 乱数補正: 85%〜100%
        int randPercent = 85 + (std::rand() % 16); // 85..100
        int dmg = (base * randPercent) / 100;
        if (dmg < 1) dmg = 1;

        return dmg;
    };
    auto isCritical = []() -> bool {
        // 急所率: 約6.25%（1/16）
        return (std::rand() % 16) == 0;
    };
    auto hits = []() -> bool {
        // 命中率: 95%
        return (std::rand() % 100) < 95;
    };

    int hp1 = s1.hp;
    int hp2 = s2.hp;

    std::cout << "バトル開始！\n";
    std::cout << "ターン順は速度で決定: 1P(" << s1.speed << ") / 2P(" << s2.speed << ")\n";

    // 先攻・後攻の決定（同速は1P先攻）
    bool firstIsP1 = (s1.speed >= s2.speed);

    int turn = 1;
    while (hp1 > 0 && hp2 > 0) {
        std::cout << "---- ターン " << turn << " ----\n";

        auto act = [&](bool attackerIsP1) {
            const Status& atkS = attackerIsP1 ? s1 : s2;
            const Status& defS = attackerIsP1 ? s2 : s1;
            int& defHp = attackerIsP1 ? hp2 : hp1;

            std::cout << (attackerIsP1 ? "1P" : "2P") << "のこうげき！";
            if (!hits()) {
                std::cout << " しかし こうげきは はずれた！\n";
                return;
            }

            bool crit = isCritical();
            int dmg = calcDamage(atkS.attack, defS.defense);
            if (crit) {
                // 急所補正: 1.5倍
                dmg = (dmg * 3) / 2;
                std::cout << " 急所に当たった！";
            }
            std::cout << "\n";

            defHp -= dmg;
            if (defHp < 0) defHp = 0;

            std::cout << (attackerIsP1 ? "2P" : "1P") << "は " << dmg << " のダメージを受けた！ "
                      << "HP: " << defHp << "\n";
        };

        // 先攻
        act(firstIsP1);
        if (hp1 <= 0 || hp2 <= 0) break;

        // 後攻
        act(!firstIsP1);
        if (hp1 <= 0 || hp2 <= 0) break;

        ++turn;
    }

    if (hp1 > hp2) std::cout << "1Pの勝ち！\n";
    else if (hp2 > hp1) std::cout << "2Pの勝ち！\n";
    else std::cout << "引き分け！\n";
}

bool isValidId(const std::string& id) {
    return id.size() == 5 && std::all_of(id.begin(), id.end(), [](char c) {
        return isdigit(static_cast<unsigned char>(c));
        });
}

int main() {
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
        }
        else if (menuChoice == 3) { // To Title
            loadAndDrawTitle("title.txt");
            _getch();
            continue;
        }
        else if (menuChoice == 4) { // Enemy List 仮表示
            std::cout << "敵リストは未実装です。\n";
            std::cout << "何かキーで戻る...\n";
            _getch();
            continue;
        }

        // プレイヤー人数
        int playerCount = (menuChoice == 1) ? 1 : 2;

        std::string id1, id2;
        while (true) {
            std::cout << "1Pの学籍番号（5桁）: ";
            std::cin >> id1;
            if (!isValidId(id1)) continue;
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
                if (!isValidId(id2)) continue;
                if (id2[0] != '5') {
                    std::cout << "貴様は今年の代ではない！\n";
                    continue;
                }
                break;
            }
            s2 = generateStatus(id2);
        }
        else {
            // 1プレイヤー時の仮敵生成（固定IDなど）
            s2 = generateStatus("50000");
        }

        std::cout << "1P: HP=" << s1.hp << " 攻撃=" << s1.attack << " 防御=" << s1.defense << " 速度=" << s1.speed << "\n";
        std::cout << "2P: HP=" << s2.hp << " 攻撃=" << s2.attack << " 防御=" << s2.defense << " 速度=" << s2.speed << "\n";

        battle(s1, s2);

        std::cout << "何かキーでメニューへ...\n";
        _getch();
    }

    return 0;
}

