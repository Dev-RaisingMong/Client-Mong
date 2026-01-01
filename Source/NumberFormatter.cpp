#include "NumberFormatter.h"
#include <cmath>
#include <sstream>
#include <iomanip>

std::string NumberFormatter::Format(int64_t value)
{
    if (value < UNIT_BASE)
        return std::to_string(value);

    double number  = static_cast<double>(value);
    int unit_index = 0;

    while (number >= UNIT_BASE && unit_index < MAX_UNIT)
    {
        number /= UNIT_BASE;
        unit_index++;
    }

    char unit_char = static_cast<char>('A' + unit_index - 1);

    std::ostringstream oss;
    if (std::fmod(number, 1.0) == 0.0)
        oss << static_cast<int64_t>(number);
    else
        oss << std::fixed << std::setprecision(2) << number;

    oss << unit_char;
    return oss.str();
}
