#pragma once
#include <string>
#include <vector>
#include <utility>

extern const char* kMeiboFilePath;

bool AppendToMeibo(const std::string& name, const std::string& id);
std::vector<std::pair<std::string, std::string>> LoadMeibo();
std::pair<std::string, std::string> PickRandomEnemy();
void DiscardLine();

// 指定インデックスの1件を削除（0-based、範囲外なら false）
bool DeleteMeiboByIndex(size_t index);