#include "status.h"
#include <cctype>

static int getDigit(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    return 9;
}

static int invMap(int digit, int minVal, int maxVal)
{
    const int range = maxVal - minVal;
    const int add = ((9 - digit) * range) / 9;
    return minVal + add;
}

bool isValidName(const std::string& name)
{
    if (name.empty()) {
        return false;
    }
    for (char c : name) {
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            return false;
        }
    }
    return true;
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
    const int d_hp  = (studentId.size() >= 2) ? getDigit(studentId[1]) : 9;
    const int d_atk = (studentId.size() >= 3) ? getDigit(studentId[2]) : 9;
    const int d_def = (studentId.size() >= 4) ? getDigit(studentId[3]) : 9;
    const int d_spd = (studentId.size() >= 5) ? getDigit(studentId[4]) : 9;

    Status s;
    s.hp      = invMap(d_hp,  80, 120);
    s.attack  = invMap(d_atk, 20,  40);
    s.defense = invMap(d_def, 10,  30);
    s.speed   = invMap(d_spd,  5,  20);
    return s;
}