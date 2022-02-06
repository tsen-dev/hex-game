#include <iostream>

#include "hexcell.h"

std::ostream& operator<<(std::ostream& out, const HexCell& hexCell)
{
    out << static_cast<char>(hexCell);
    return out;
}
