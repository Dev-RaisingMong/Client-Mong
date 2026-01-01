#pragma once

#include <string>
#include <cstdint>

class NumberFormatter
{
public:
    static std::string Format(int64_t value);

private:
    static constexpr int64_t UNIT_BASE = 1000;
    static constexpr int MAX_UNIT      = 26;
};
