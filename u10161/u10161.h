#pragma once

class U10161
{
public:
    static const char * libname()
    {
        return "u10161";
    }
    U10161() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U10161> pu10161;
#endif
