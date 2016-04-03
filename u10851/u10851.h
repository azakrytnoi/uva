#pragma once

class U10851
{
public:
    static const char * libname()
    {
        return "u10851";
    }
    U10851() {}

    void operator()();
};

#ifdef POPULATE_CACHE
populate <U10851> pu10851;
#endif
