#include "Meibo.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>

const char* kMeiboFilePath = "meibo.txt";

static bool NeedsNewlineBeforeAppend(const char* path)
{
    std::ifstream fin(path, std::ios::binary);
    if (!fin) return false;
    fin.seekg(0, std::ios::end);
    const std::streamoff size = fin.tellg();
    if (size <= 0) return false;
    fin.seekg(-1, std::ios::end);
    char last = '\0';
    fin.read(&last, 1);
    return last != '\n';
}

bool AppendToMeibo(const std::string& name, const std::string& id)
{
    std::ofstream fout(kMeiboFilePath, std::ios::app | std::ios::binary);
    if (!fout) {
        std::cerr << "名簿ファイルへ書き込めません: " << kMeiboFilePath << "\n";
        return false;
    }
    if (NeedsNewlineBeforeAppend(kMeiboFilePath)) {
        fout << '\n';
    }
    fout << name << ' ' << id << '\n';
    return true;
}

std::vector<std::pair<std::string, std::string>> LoadMeibo()
{
    std::vector<std::pair<std::string, std::string>> list;
    std::ifstream fin(kMeiboFilePath);
    if (!fin) return list;

    std::string name;
    std::string id;
    while (fin >> name >> id) {
        list.emplace_back(name, id);
        int ch;
        while ((ch = fin.get()) != '\n' && ch != EOF) {}
    }
    return list;
}

std::pair<std::string, std::string> PickRandomEnemy()
{
    const auto list = LoadMeibo();
    if (list.empty()) {
        return std::make_pair(std::string("DummyEnemy"), std::string("50000"));
    }
    const size_t idx = static_cast<size_t>(std::rand()) % list.size();
    return list[idx];
}

void DiscardLine()
{
    int ch;
    while ((ch = std::cin.get()) != '\n' && ch != EOF) {}
}

bool DeleteMeiboByIndex(size_t index)
{
    auto list = LoadMeibo();
    if (index >= list.size()) return false;

    const std::string tmpPath = std::string(kMeiboFilePath) + ".tmp";
    std::ofstream fout(tmpPath, std::ios::binary);
    if (!fout) {
        std::cerr << "一時ファイルを作成できません: " << tmpPath << "\n";
        return false;
    }

    for (size_t i = 0; i < list.size(); ++i) {
        if (i == index) continue;
        fout << list[i].first << ' ' << list[i].second << '\n';
    }
    fout.close();

    if (std::remove(kMeiboFilePath) != 0) {
        std::cerr << "既存名簿の削除に失敗しました: " << kMeiboFilePath << "\n";
        std::remove(tmpPath.c_str());
        return false;
    }
    if (std::rename(tmpPath.c_str(), kMeiboFilePath) != 0) {
        std::cerr << "一時ファイルの置換に失敗しました: " << tmpPath << "\n";
        return false;
    }
    return true;
}