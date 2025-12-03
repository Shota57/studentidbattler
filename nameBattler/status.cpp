#include "status.h"
#include <cctype>

static int getDigit(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    // 非数字は安全側で最弱扱い
    return 9;
}

static int invMap(int digit, int minVal, int maxVal)
{
    // digit=0 -> 最大値、digit=9 -> 最小値
    const int range = maxVal - minVal;
    const int add = ((9 - digit) * range) / 9;
    return minVal + add;
}

bool isValidId(const std::string& id)
{
    if (id.size() != 5) {
        return false;
    }
    for (char c : id) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

Status generateStatus(const std::string& studentId)
{
    // 先頭（万の位）の '5' は無視し、残り4桁を HP/ATK/DEF/SPD に対応
    const int d_hp  = (studentId.size() >= 2) ? getDigit(studentId[1]) : 9;
    const int d_atk = (studentId.size() >= 3) ? getDigit(studentId[2]) : 9;
    const int d_def = (studentId.size() >= 4) ? getDigit(studentId[3]) : 9;
    const int d_spd = (studentId.size() >= 5) ? getDigit(studentId[4]) : 9;

    Status s;
    s.hp      = invMap(d_hp,  80, 120); // 80..120（digit小さいほど高HP）
    s.attack  = invMap(d_atk, 20,  40); // 20..40
    s.defense = invMap(d_def, 10,  30); // 10..30
    s.speed   = invMap(d_spd,  5,  20); // 5..20
    return s;
}