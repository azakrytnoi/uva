#pragma once

class U10038
{
public:
    static const char * libname()
    {
        return "u10038";
    }
    U10038() {}

    void operator()() const;
};

#ifdef POPULATE_CACHE
populate <U10038> pu10038;
#endif
