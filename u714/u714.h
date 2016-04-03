#pragma once

class  U714
{
public:
    static const char * libname()
    {
        return "u714";
    }
    U714();
    void operator()();
};
#ifdef POPULATE_CACHE
populate <U714> pu714;
#endif

