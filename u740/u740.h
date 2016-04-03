#pragma once

class U740
{
public:
    static const char * libname()
    {
        return "u740";
    }
    U740() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U740> pu740;
#endif
