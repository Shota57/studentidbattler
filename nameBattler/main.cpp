#include <cstdlib>
#include <ctime>
#include "game.h"
#include "console.h"

int main()
{
    SetBackgroundColorRGB(255, 255, 255);
    SetTextColorRGB(0, 0, 0);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    // 画面全体の初期化（必要ならコンソール初期化も呼ぶ）
    MoveCursorToTop();
    // CL11Startup(); // 必要ならサイズ/フォント/中央配置を初期化

    runGame();
    return 0;
}