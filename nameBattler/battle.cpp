#include "battle.h"
#include <iostream>
#include <cstdlib>
#include <string>

static void WaitEnter(const char* prompt = "Enterで次へ...")
{
    std::cout << prompt << std::endl;
    std::string dummy;
    std::getline(std::cin, dummy);
}

static void DrawBattleHeader(const std::string& name1, const std::string& id1, const Status& s1, int hp1,
                             const std::string& name2, const std::string& id2, const Status& s2, int hp2)
{
    system("cls");
    std::cout << "================ バトル状態 ================\n";
    std::cout << "1P: " << name1 << " (ID: " << id1 << ")\n"
              << "    HP: " << hp1 << " / " << s1.hp
              << "   攻撃: " << s1.attack
              << "   防御: " << s1.defense
              << "   速度: " << s1.speed << "\n";
    std::cout << "2P: " << name2 << " (ID: " << id2 << ")\n"
              << "    HP: " << hp2 << " / " << s2.hp
              << "   攻撃: " << s2.attack
              << "   防御: " << s2.defense
              << "   速度: " << s2.speed << "\n";
    std::cout << "============================================\n\n";
}

void battle(const std::string& name1, const std::string& id1, const Status& s1,
            const std::string& name2, const std::string& id2, const Status& s2)
{
    auto calcDamage = [](int atk, int def) -> int {
        int base = (atk * 2) / 3 - def / 2;
        if (base < 1) base = 1;
        int randPercent = 85 + (std::rand() % 16);
        int dmg = (base * randPercent) / 100;
        if (dmg < 1) dmg = 1;
        return dmg;
    };
    auto isCritical = []() -> bool {
        return (std::rand() % 16) == 0;
    };
    auto hits = []() -> bool {
        return (std::rand() % 100) < 95;
    };

    int hp1 = s1.hp;
    int hp2 = s2.hp;

    DrawBattleHeader(name1, id1, s1, hp1, name2, id2, s2, hp2);
    std::cout << "バトル開始！\n";
    std::cout << "ターン順は速度で決定: 1P(" << s1.speed << ") / 2P(" << s2.speed << ")\n";
    WaitEnter();

    const bool firstIsP1 = (s1.speed >= s2.speed);
    int turn = 1;

    while (hp1 > 0 && hp2 > 0) {
        DrawBattleHeader(name1, id1, s1, hp1, name2, id2, s2, hp2);
        std::cout << "---- ターン " << turn << " ----\n";

        auto act = [&](bool attackerIsP1) {
            const Status& atkS = attackerIsP1 ? s1 : s2;
            const Status& defS = attackerIsP1 ? s2 : s1;
            int& defHp = attackerIsP1 ? hp2 : hp1;

            std::cout << (attackerIsP1 ? "1P" : "2P") << "のこうげき！";
            if (!hits()) {
                std::cout << " しかし こうげきは はずれた！\n";
                DrawBattleHeader(name1, id1, s1, hp1, name2, id2, s2, hp2);
                std::cout << (attackerIsP1 ? "1P" : "2P") << "のこうげき！ しかし はずれた！\n";
                WaitEnter();
                return;
            }

            bool crit = isCritical();
            int dmg = calcDamage(atkS.attack, defS.defense);
            if (crit) {
                dmg = (dmg * 3) / 2;
                std::cout << " 急所に当たった！";
            }
            std::cout << "\n";

            defHp -= dmg;
            if (defHp < 0) defHp = 0;

            DrawBattleHeader(name1, id1, s1, hp1, name2, id2, s2, hp2);
            std::cout << (attackerIsP1 ? "2P" : "1P") << "は " << dmg
                      << " のダメージを受けた！ HP: " << defHp << "\n";
            WaitEnter();
        };

        act(firstIsP1);
        if (hp1 <= 0 || hp2 <= 0) break;

        act(!firstIsP1);
        if (hp1 <= 0 || hp2 <= 0) break;

        ++turn;
        DrawBattleHeader(name1, id1, s1, hp1, name2, id2, s2, hp2);
        WaitEnter("Enterで次のターンへ...");
    }

    DrawBattleHeader(name1, id1, s1, hp1, name2, id2, s2, hp2);
    if (hp1 > hp2) {
        std::cout << "1Pの勝ち！\n";
    } else if (hp2 > hp1) {
        std::cout << "2Pの勝ち！\n";
    } else {
        std::cout << "引き分け！\n";
    }
    WaitEnter("Enterで終了...");
}