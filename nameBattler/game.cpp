#include "game.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "title.h"
#include "console.h"
#include "conioex.h"
#include "menu.h"
#include "status.h"
#include "battle.h"
#include "Meibo.h"

static void PromptNameAndId(const char* playerLabel, std::string& outName, std::string& outId)
{
    while (true) {
        std::cout << playerLabel << "の名前: " << std::flush;
        if (!std::getline(std::cin, outName)) {
            std::cin.clear();
            continue;
        }
        if (!isValidName(outName)) {
            std::cout << "名前は空でなく、空白を含めないで入力してください。\n";
            continue;
        }
        break;
    }

    while (true) {
        std::cout << playerLabel << "の学籍番号（5桁）: " << std::flush;
        if (!std::getline(std::cin, outId)) {
            std::cin.clear();
            continue;
        }
        std::string trimmed;
        for (char c : outId) {
            if (c != ' ' && c != '\t' && c != '\r') {
                trimmed.push_back(c);
            }
        }
        outId.swap(trimmed);

        if (!isValidId(outId)) {
            std::cout << "学籍番号は5桁の数字で入力してください。\n";
            continue;
        }
        break;
    }
}

// Enemy List 管理画面（選択して削除）
static void ShowEnemyListManage()
{
    while (true) {
        auto meibo = LoadMeibo();
        int count = static_cast<int>(meibo.size());
        int cursor = 0;

        system("cls");
        if (count == 0) {
            std::cout << "名簿は空です。\n";
            std::cout << "何かキーで戻る...\n";
            _getch();
            return;
        }

        while (true) {
            system("cls");
            std::cout << "===== 名簿一覧（Enterで選択、Escで戻る）=====\n";
            for (int i = 0; i < count; ++i) {
                std::cout << (i == cursor ? "> " : "  ")
                          << meibo[i].first << " (ID: " << meibo[i].second << ")\n";
            }
            std::cout << "\n上下矢印で移動、Enterで削除確認、Escで戻る\n";

            int key = _getch();
            if (key == 0xE0 || key == 0) {
                key = _getch();
                if (key == 72) {
                    cursor = (cursor - 1 + count) % count;
                } else if (key == 80) {
                    cursor = (cursor + 1) % count;
                }
            } else if (key == 13) {
                // 削除確認
                while (true) {
                    system("cls");
                    std::cout << "削除対象:\n";
                    std::cout << "> " << meibo[cursor].first << " (ID: " << meibo[cursor].second << ")\n\n";
                    const char* confirmItems[2] = { "削除する", "キャンセル" };
                    int ccur = 0;
                    // 簡易確認メニュー: 左右矢印 or 上下で移動、Enterで決定
                    while (true) {
                        system("cls");
                        std::cout << "本当に削除しますか？\n\n";
                        for (int i = 0; i < 2; ++i) {
                            std::cout << (i == ccur ? "> " : "  ") << confirmItems[i] << "\n";
                        }
                        int k = _getch();
                        if (k == 0xE0 || k == 0) {
                            k = _getch();
                            if (k == 72 || k == 75) { // Up or Left
                                ccur = (ccur - 1 + 2) % 2;
                            } else if (k == 80 || k == 77) { // Down or Right
                                ccur = (ccur + 1) % 2;
                            }
                        } else if (k == 13) {
                            if (ccur == 0) {
                                // 削除実行
                                if (DeleteMeiboByIndex(static_cast<size_t>(cursor))) {
                                    std::cout << "削除しました。\n";
                                } else {
                                    std::cout << "削除に失敗しました。\n";
                                }
                                std::cout << "何かキーで一覧へ...\n";
                                _getch();
                            }
                            // キャンセルまたは削除後、一覧へ戻る
                            break;
                        } else if (k == 27) { // Esc
                            break;
                        }
                    }
                    break;
                }
                // 一覧を再読み込みして続行
                break;
            } else if (key == 27) { // Esc
                return;
            }
        }
    }
}

void runGame()
{
    system("cls");
    SetCursorVisibility(FALSE);

    loadAndDrawTitle("title.txt");
    _getch();

    while (true) {
        int menuChoice = showMenu();
        system("cls");

        if (menuChoice == 5) {
            std::cout << "終了します。\n";
            break;
        } else if (menuChoice == 3) {
            loadAndDrawTitle("title.txt");
            _getch();
            continue;
        } else if (menuChoice == 4) {
            ShowEnemyListManage();
            continue;
        }

        const int playerCount = (menuChoice == 1) ? 1 : 2;

        std::string name1;
        std::string id1;
        PromptNameAndId("1P", name1, id1);
        if (!AppendToMeibo(name1, id1)) {
            std::cout << "名簿への保存に失敗しました。\n";
        }
        Status s1 = generateStatus(id1);

        std::string name2;
        std::string id2;
        Status s2{};

        if (playerCount == 2) {
            PromptNameAndId("2P", name2, id2);
            if (!AppendToMeibo(name2, id2)) {
                std::cout << "名簿への保存に失敗しました。\n";
            }
            s2 = generateStatus(id2);
        } else {
            auto all = LoadMeibo();
            std::vector<std::pair<std::string, std::string>> candidates;
            candidates.reserve(all.size());
            for (const auto& e : all) {
                if (e.second != id1) { // 学籍番号が異なるもののみ
                    candidates.push_back(e);
                }
            }
            if (candidates.empty()) {
                name2 = "DummyEnemy";
                id2 = "50000";
            } else {
                const size_t idx = static_cast<size_t>(std::rand()) % candidates.size();
                name2 = candidates[idx].first;
                id2 = candidates[idx].second;
            }
            s2 = generateStatus(id2);
            std::cout << "乱数で選ばれた敵: " << name2 << " (ID: " << id2 << ")\n";
        }

        system("cls");
        std::cout << "1P: " << name1 << " (ID: " << id1 << ")  "
                  << "HP=" << s1.hp << " 攻撃=" << s1.attack
                  << " 防御=" << s1.defense << " 速度=" << s1.speed << "\n";
        std::cout << "2P: " << name2 << " (ID: " << id2 << ")  "
                  << "HP=" << s2.hp << " 攻撃=" << s2.attack
                  << " 防御=" << s2.defense << " 速度=" << s2.speed << "\n";

        battle(name1, id1, s1, name2, id2, s2);

        std::cout << "何かキーでメニューへ...\n";
        _getch();
    }
}