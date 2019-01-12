#include "defs.h"

#include <iostream>

void ERR_RET(const std::string &s)
{
    std::cerr << s << std::endl << std::flush;
    abort();
}

void ERR_RET2(const std::string& s, int t)
{
    std::cerr << s << " " << t << std::endl << std::flush;
    abort();
}

