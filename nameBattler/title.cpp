#include "console.h"
#include "title.h"
#include <fstream>
#include <iostream>
#include <string>

// const char* 版: 既存呼び出しと互換。文字列版へ委譲
void loadAndDrawTitle(const char* filename) {
    loadAndDrawTitle(std::string(filename));
}

// ファイル内容をそのまま表示
void loadAndDrawTitle(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "タイトルファイルを開けません: " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(fin, line)) {
        std::cout << line << '\n';
    }
}
