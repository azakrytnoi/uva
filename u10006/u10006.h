#pragma once

class U10006
{
public:
    static const char * libname()
    {
        return "u10006";
    }
    U10006() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10006> pu10006;
#endif
