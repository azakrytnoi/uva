#pragma once

class U10260
{
public:
    static const char * libname()
    {
        return "u10260";
    }
    U10260() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U10260> pu10260;
#endif
