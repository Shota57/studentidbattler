#include "console.h"
#include "title.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// const char* 版は文字列版へ委譲
void loadAndDrawTitle(const char* filename) {
    loadAndDrawTitle(std::string(filename));
}

// ファイル内容をブロック中央表示
void loadAndDrawTitle(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) {
        const char* err[] = { "タイトルファイルを開けません:", filename.c_str() };
        PrintCenteredBlock(err, 2);
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }
    if (lines.empty()) return;

    int startRow = (CONSOLE_HEIGHT - static_cast<int>(lines.size())) / 2;
    if (startRow < 0) startRow = 0;

    for (size_t i = 0; i < lines.size(); ++i) {
        PrintCenteredLine(lines[i].c_str(), startRow + static_cast<int>(i));
    }
}