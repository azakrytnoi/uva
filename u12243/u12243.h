#pragma once

class U12243
{
public:
    static const char * libname()
    {
        return "u12243";
    }
    U12243() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U12243> pu12243;
#endif
