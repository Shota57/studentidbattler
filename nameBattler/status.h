#pragma once
#include <string>

struct Status {
    int hp;
    int attack;
    int defense;
    int speed;
};

bool isValidId(const std::string& id);
Status generateStatus(const std::string& studentId);
