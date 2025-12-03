#include "console.h"
#include "title.h"
#include <fstream>
#include <iostream>
#include <string>

void loadAndDrawTitle(const char* filename) {
    loadAndDrawTitle(std::string(filename));
}

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