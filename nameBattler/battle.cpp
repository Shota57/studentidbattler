#include "battle.h"
#include <iostream>
#include <cstdlib>

void battle(const Status& s1, const Status& s2)
{
    auto calcDamage = [](int atk, int def) -> int {
        int base = (atk * 2) / 3 - def / 2;
        if (base < 1) {
            base = 1;
        }
        int randPercent = 85 + (std::rand() % 16); // 85..100
        int dmg = (base * randPercent) / 100;
        if (dmg < 1) {
            dmg = 1;
        }
        return dmg;
    };
    auto isCritical = []() -> bool {
        return (std::rand() % 16) == 0; // 約6.25%
    };
    auto hits = []() -> bool {
        return (std::rand() % 100) < 95; // 95%
    };

    int hp1 = s1.hp;
    int hp2 = s2.hp;

    std::cout << "バトル開始！\n";
    std::cout << "ターン順は速度で決定: 1P(" << s1.speed << ") / 2P(" << s2.speed << ")\n";

    const bool firstIsP1 = (s1.speed >= s2.speed);
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
                dmg = (dmg * 3) / 2; // 1.5倍
                std::cout << " 急所に当たった！";
            }
            std::cout << "\n";

            defHp -= dmg;
            if (defHp < 0) {
                defHp = 0;
            }

            std::cout << (attackerIsP1 ? "2P" : "1P") << "は " << dmg
                      << " のダメージを受けた！ HP: " << defHp << "\n";
        };

        act(firstIsP1);
        if (hp1 <= 0 || hp2 <= 0) break;

        act(!firstIsP1);
        if (hp1 <= 0 || hp2 <= 0) break;

        ++turn;
    }

    if (hp1 > hp2) {
        std::cout << "1Pの勝ち！\n";
    } else if (hp2 > hp1) {
        std::cout << "2Pの勝ち！\n";
    } else {
        std::cout << "引き分け！\n";
    }
}