#pragma once
#include <map>

#include "ParserCommandInfo.h"

template <typename T>
class Command {
public:
    virtual ~Command() = default;

    virtual void execute(T data) = 0;
};
