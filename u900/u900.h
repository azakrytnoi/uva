#pragma once

class  U900
{
public:
    static const char * libname()
    {
        return "u900";
    }
    U900();

    void operator()();
};
#ifdef POPULATE_CACHE
populate <U900> pu900;
#endif

